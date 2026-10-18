"""Iterative function discovery: Ghidra entries + call targets + code pointers + gap scanning.
Writes raw/functions.json (entries with evidence, extents, pools, call edges) and raw/cover.bin."""
import json, collections, struct
from common import *
from walk import load_ghidra, walk, valid

gfun, comp, gcalls = load_ghidra()
ev = collections.defaultdict(set)
for e in gfun: ev[e].add('ghidra')
for e in (0x8C0100A4, 0x8C0231D2, 0x8C010000): ev[e].add('anchor')
for s, t in gcalls.items():
    ev[t].add('call')

# fixed data region (no gap scanning): strings/tables region found by profiling
NOSCAN = [(0x8C0A77B2, 0x8C0D2480), (0x8C1151DC, 0x8C11A4A0), (0x8C11BB0E, 0x8C121F60)]

def run_walks(entries):
    return {e: walk(e, entries, comp) for e in sorted(entries)}

WEAKCODE = bytearray(SIZE)
def coverage(res):
    cov = bytearray(SIZE)
    WEAKCODE[:] = bytes(SIZE)
    for e, r in res.items():
        weak = not (ev[e] & {'ghidra', 'call', 'bsr', 'anchor'})
        for a in r['ins']:
            cov[a - BASE] = 1; cov[a - BASE + 1] = 1
            if weak: WEAKCODE[a - BASE] = 1
    for e, r in res.items():
        for p, n in list(r['pools'].items()) + list(r['tables'].items()):
            if inimg(p):
                for i in range(n):
                    if cov[p - BASE + i] == 0: cov[p - BASE + i] = 2
    return cov

def gaps(cov, lo=BASE, hi=END):
    out = []; i = lo - BASE
    while i < hi - BASE:
        if cov[i] == 0:
            j = i
            while j < hi - BASE and cov[j] == 0: j += 1
            out.append((BASE + i, BASE + j)); i = j
        else:
            i += 1
    return out

def prologue(p):
    w = w16(p)
    return (w & 0xFF0F) == 0x2F06 or w == 0x4F22 or (w & 0xFF0F) == 0xFF0B or ((w >> 8) == 0x7F and (w & 0x80))

def clean(p, entries, cov, gap_end):
    if p in entries: return None
    r = walk(p, entries | {p}, comp)
    if r['bad'] or not r['ins']: return None
    ins = r['ins']
    if min(ins) < p: return None
    pro = prologue(p)
    for a in ins:
        if cov[a - BASE] and not (pro and cov[a - BASE] == 1 and WEAKCODE[a - BASE]): return None
    for t, n in r['pools'].items():
        if not inimg(t): return None
        if t in ins or (n == 4 and (t + 2) in ins): return None
        if n == 4 and t % 4: return None
    if r['bsr']:
        for s, t in r['bsr']:
            if not inimg(t): return None
    return r

STRONG = {'ghidra', 'call', 'bsr', 'anchor'}
def strength(e):
    k = ev[e]
    return 2 if k & {'call', 'bsr', 'anchor'} else 1 if k & {'ghidra', 'tailbra'} else 0

DROPS = {}
def prune(res, blocked):
    srt = sorted(res)
    drop = set(); hard = set()
    ext = {e: (e, max(r['ins']) + 2) if r['ins'] else (e, e) for e, r in res.items()}
    for e in srt:
        r = res[e]
        if not r['ins'] or e in r['bad']:
            drop.add(e)
    live = [e for e in srt if e not in drop]
    for i, e in enumerate(live):
        s, t = ext[e]
        j = i + 1
        while j < len(live) and live[j] < t:
            x = live[j]
            se, sx = strength(e), strength(x)
            if sx < se or (sx == se and not (prologue(x) and not prologue(e))):
                drop.add(x); loser, winner = x, e
            else:
                drop.add(e); loser, winner = e, x
            DROPS[loser] = DROPS.get(loser, 0) + 1
            if strength(winner) > strength(loser) or DROPS[loser] >= 3:
                hard.add(loser)
            j += 1
    drop -= {0x8C0100A4, 0x8C0231D2, 0x8C010000}
    blocked |= (hard & drop)
    return drop

it = 0
entries = set(ev)
blocked = set()
while True:
    it += 1
    res = run_walks(entries)
    d = prune(res, blocked)
    if d:
        entries -= d
        print('iter', it, 'pruned', len(d))
        res = run_walks(entries)
    # add bsr targets
    new = set()
    for e, r in res.items():
        for s, t in r['bsr']:
            if t not in entries and inimg(t) and t not in blocked:
                new.add(t); ev[t].add('bsr')
        for t in r['tails']:
            if t and t not in entries and inimg(t) and t not in blocked:
                new.add(t); ev[t].add('tailbra')
    cov = coverage(res)
    # code pointers: any 4-aligned word in image whose value hits an uncovered 2-aligned address in a gap start region
    gl = gaps(cov)
    gapstart = {}
    for s, e in gl:
        gapstart[s] = e
    ptr_hits = collections.Counter()
    for o in range(0, SIZE - 3, 4):
        v = struct.unpack_from('<I', DATA, o)[0]
        if BASE <= v < END and v % 2 == 0 and cov[v - BASE] == 0:
            ptr_hits[v] += 1
    for v in ptr_hits:
        if any(a <= v < b for a, b in NOSCAN): continue
        # find gap containing v
        if v in blocked: continue
        r = clean(v, entries | new, cov, END)
        if r is not None:
            k, _ = classify(v)
            new.add(v); ev[v].add('codeptr')
    if new - entries and it < 40:
        new -= entries
        print('iter', it, 'entries', len(entries), 'new(seeded)', len(new), sorted(hex(x) for x in new)[:3])
        entries |= new
        continue
    # gap scan (only once seeds are exhausted)
    for s, e in gl:
        if any(a <= s < b for a, b in NOSCAN): continue
        if e - s < 4: continue
        p = s
        while p < e - 2 and w16(p) in (0x0009, 0x0000):
            p += 2
        while p < e - 2:
            if p in new: break
            if p in blocked:
                p += 2; continue
            r = clean(p, entries | new, cov, e)
            if r is not None:
                new.add(p); ev[p].add('gapscan'); break
            p += 2
    new -= entries
    print('iter', it, 'entries', len(entries), 'new', len(new))
    if not new or it > 50: break
    entries |= new

res = run_walks(entries)
d = prune(res, blocked)
entries -= d
res = run_walks(entries)
print('final pruned', len(d), 'blocked total', len(blocked))
cov = coverage(res)
open(os.path.join(RAW, 'cover.bin'), 'wb').write(bytes(cov))
out = {}
for e, r in res.items():
    out[h(e)] = dict(start=e, end=max(r['ins']) + 2, ninsn=len(r['ins']), evidence=sorted(ev[e]),
                    pools={h(k): v for k, v in r['pools'].items()}, tables={h(k): v for k, v in r['tables'].items()},
                    bsr=[t for s, t in r['bsr']], tails=[t for t in r['tails'] if t], jmp_tail=None in r['tails'],
                    jsr=[gcalls.get(s) for s in r['jsrs']], bad=r['bad'])
json.dump(out, open(os.path.join(RAW, 'functions.json'), 'w'))
c = collections.Counter(cov)
print('coverage bytes: none %d code %d pool %d' % (c[0], c[1], c[2]))
