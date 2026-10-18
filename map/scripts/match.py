"""Propagating DC<->MAT3 function matcher (BinDiff-style).
Evidence: anchors, rare strings, rare float constants, call-sequence alignment from matched pairs (callees and
callers), neighbour order within a source file, size plausibility. -> raw/matches.json"""
import json, math, struct, collections, sys
from common import *

LO, HI = 0x8C010000, 0x8C0A77B2
D = json.load(open(os.path.join(RAW, 'dc_features.json')))
M = json.load(open(os.path.join(RAW, 'mat3_full.json')))
dfs = sorted([f for f in D.values() if LO <= f['start'] < HI], key=lambda f: f['start'])
daddr = [f['start'] for f in dfs]
dpos = {a: i for i, a in enumerate(daddr)}
dsize = {f['start']: f['end'] - f['start'] for f in dfs}
dcalls = {f['start']: [c[2] for c in f['calls'] if c[0] in ('bsr', 'jsr', 'bra', 'jmp')] for f in dfs}
dcallers = collections.defaultdict(list)
for f in dfs:
    for t in dcalls[f['start']]:
        if t is not None:
            dcallers[t].append(f['start'])
dstr = {f['start']: set(f['strings']) for f in dfs}


def as_float(v):
    x = struct.unpack('<f', struct.pack('<I', v & 0xFFFFFFFF))[0]
    if x != x or x == 0:
        return None
    if not (1e-5 < abs(x) < 1e7):
        return None
    if BASE <= v < 0x8D000000:
        return None
    return x


dflt = {}
for f in dfs:
    s = set()
    for p, v in f['pools']:
        if v > 0xFFFF:
            x = as_float(v)
            if x is not None:
                s.add(x)
    dflt[f['start']] = s

mnames = [m['file'] + ':' + m['name'] for m in M]
mby = {k: m for k, m in zip(mnames, M)}
mfile_list = collections.defaultdict(list)
for k, m in zip(mnames, M):
    mfile_list[m['file']].append(k)
mfidx = {}
for fl in mfile_list.values():
    for j, k in enumerate(fl):
        mfidx[k] = j
name2keys = collections.defaultdict(list)
for k, m in zip(mnames, M):
    name2keys[m['name']].append(k)
allm = set(name2keys)
mcalls = {k: [c for c in m['calls'] if c in allm] for k, m in zip(mnames, M)}
mcallers = collections.defaultdict(list)
for k, m in zip(mnames, M):
    for c in mcalls[k]:
        for k2 in name2keys[c]:
            mcallers[k2].append(k)
mstr = {k: set(m['strings']) for k, m in zip(mnames, M)}
mflt = {k: set(m['floats']) - {0.0} for k, m in zip(mnames, M)}
strfreq = collections.Counter(s for k in mnames for s in mstr[k])
fltfreq = collections.Counter(x for k in mnames for x in mflt[k])


def feq(a, b):
    return abs(a - b) <= 2e-6 * max(abs(a), abs(b))


def sim(d, k):
    m = mby[k]
    sc = 0.0
    ev = []
    ds, ms = dstr[d], mstr[k]
    common = ds & ms
    for s in common:
        sc += 4.0 if strfreq[s] <= 2 else 2.0 if strfreq[s] <= 5 else 0.7
    if common:
        ev.append('str%d' % len(common))
    if ds and ms and not common:
        sc -= 2
    if ds and not ms and m['has_src']:
        sc -= 1
    if ms and not ds:
        sc -= 1
    nf = 0
    for x in mflt[k]:
        if any(feq(x, y) for y in dflt[d]):
            nf += 1
            sc += 2.0 if fltfreq[x] <= 3 else 1.0 if fltfreq[x] <= 15 else 0.3
    if nf:
        ev.append('flt%d' % nf)
    if m['gc_size'] > 0:
        r = math.log(max(dsize[d], 4) / (0.75 * m['gc_size']))
        sc -= 1.2 * abs(r)
    if m['has_src']:
        sc -= 0.15 * min(abs(len(dcalls[d]) - len(mcalls[k])), 12)
    return sc, ev


match, rmatch, score = {}, {}, {}
evid = collections.defaultdict(list)
ANCH = {0x8C0100A4: 'RetraceCallback', 0x8C0100AE: 'InitializeTimer', 0x8C0100FA: 'PauseTimer', 0x8C010102: 'ResumeTimer',
        0x8C01010A: 'UpdateFrameTimer', 0x8C010176: 'SetCountdownTimer', 0x8C010194: 'AddCountdownTime',
        0x8C0101A8: 'GetCountdownTimeRemaining', 0x8C0101FC: 'CountdownTimeOut',
        0x8C022CA4: 'drivetrain', 0x8C022D94: 'whatslips', 0x8C022EFC: 'autoshift', 0x8C022F96: 'transmission',
        0x8C022FEC: 'find_best_gear', 0x8C023048: 'upshift', 0x8C023074: 'downshift', 0x8C0230B0: 'enginetorque',
        0x8C0231D2: 'interp', 0x8C0231E6: 'engine', 0x8C03A1B8: 'InitLL', 0x8C03A1BE: 'ClearLL', 0x8C03A1C8: 'InsertLLE',
        0x8C03A28E: 'RemoveLLE', 0x8C026B00: 'BodyVector', 0x8C026B8C: 'WorldVector', 0x8C09BC04: 'bsearch'}
for a, n in ANCH.items():
    ks = name2keys.get(n)
    if ks and a in dpos:
        match[a] = ks[0]
        rmatch[ks[0]] = a
        evid[a] = ['anchor']
        score[a] = 99.0


def align_calls(tlist, nlist):
    n1, n2 = len(tlist), len(nlist)
    if n1 == 0 or n2 == 0 or n1 * n2 > 40000:
        return []
    S = [[0.0] * (n2 + 1) for _ in range(n1 + 1)]
    for i in range(1, n1 + 1):
        t = tlist[i - 1]
        for j in range(1, n2 + 1):
            nm = nlist[j - 1]
            best = max(S[i - 1][j], S[i][j - 1])
            if t is not None:
                if t in match:
                    if mby[match[t]]['name'] == nm:
                        best = max(best, S[i - 1][j - 1] + 3)
                elif not any(k in rmatch for k in name2keys[nm]):
                    best = max(best, S[i - 1][j - 1] + 1)
            S[i][j] = best
    i, j = n1, n2
    pairs = []
    while i > 0 and j > 0:
        if S[i][j] == S[i - 1][j]:
            i -= 1
            continue
        if S[i][j] == S[i][j - 1]:
            j -= 1
            continue
        pairs.append((tlist[i - 1], nlist[j - 1]))
        i -= 1
        j -= 1
    return pairs


def candidates():
    votes = collections.defaultdict(lambda: collections.defaultdict(float))
    why = collections.defaultdict(lambda: collections.defaultdict(set))
    for d, k in list(match.items()):
        for t, nm in align_calls(dcalls[d], mcalls[k]):
            if t in dpos and t not in match:
                for k2 in name2keys[nm]:
                    if k2 not in rmatch:
                        votes[t][k2] += 2
                        why[t][k2].add('callee-of:' + mby[k]['name'])
        for c in dcallers.get(d, []):
            if c in match or c not in dpos:
                continue
            for k2 in mcallers.get(k, []):
                if k2 not in rmatch:
                    votes[c][k2] += 1
                    why[c][k2].add('caller-of:' + mby[k]['name'])
        i = dpos[d]
        fl = mfile_list[mby[k]['file']]
        j = mfidx[k]
        for di, dj in ((1, 1), (-1, -1), (1, 2), (-1, -2), (2, 1), (-2, -1)):
            ii, jj = i + di, j + dj
            if 0 <= ii < len(daddr) and 0 <= jj < len(fl):
                a2, k2 = daddr[ii], fl[jj]
                if a2 not in match and k2 not in rmatch:
                    votes[a2][k2] += 3 if (abs(di) == 1 and abs(dj) == 1) else 1.5
                    why[a2][k2].add('next-to:' + mby[k]['name'])
    for d in daddr:
        if d in match or not dstr[d]:
            continue
        for s in dstr[d]:
            if 0 < strfreq.get(s, 0) <= 2:
                for k in mnames:
                    if s in mstr[k] and k not in rmatch:
                        votes[d][k] += 2
                        why[d][k].add('str')
    return votes, why


THRESH = float(os.environ.get('MATCH_THRESH', '4.0'))
for rnd in range(80):
    votes, why = candidates()
    cand = []
    for d, kv in votes.items():
        for k, v in kv.items():
            s, ev = sim(d, k)
            cand.append((v + s, d, k, v, s, sorted(why[d][k]) + ev))
    cand.sort(reverse=True)
    bestalt = collections.defaultdict(list)
    for c in cand:
        bestalt[c[1]].append(c)
    added = 0
    for tot, d, k, v, s, ev in cand:
        if tot < THRESH:
            break
        if d in match or k in rmatch:
            continue
        alts = [x for x in bestalt[d] if x[2] != k and x[2] not in rmatch]
        if alts and alts[0][0] > tot - 1.0:
            continue
        match[d] = k
        rmatch[k] = d
        score[d] = tot
        evid[d] = ev
        added += 1
    if __name__ == '__main__': print('round', rnd, 'added', added, 'total', len(match))
    if not added:
        break

out = {}
for d, k in match.items():
    m = mby[k]
    out[h(d)] = dict(name=m['name'], file=m['file'], score=round(score[d], 2), evidence=evid[d],
                     dc_size=dsize[d], gc_size=m['gc_size'])
json.dump(out, open(os.path.join(RAW, 'matches.json'), 'w'), indent=0)
fc = collections.Counter(v['file'] for v in out.values())
print(len(out), 'matched;', fc.most_common(200))
