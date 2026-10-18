"""Build map/units.json and map/functions.csv from the reviewed unit spec, the function list, literal-pool
coverage and the per-unit MAT3 DP alignment."""
import json, csv, collections, datetime, re, bisect
import segdp
import match as MT
from common import *
from unit_spec import SPEC

F = json.load(open(os.path.join(RAW, 'functions.json')))
FEAT = json.load(open(os.path.join(RAW, 'dc_features.json')))
ATOMS = json.load(open(os.path.join(RAW, 'atoms.json')))['atoms']
cov = open(os.path.join(RAW, 'cover.bin'), 'rb').read()
fs = sorted(F.values(), key=lambda f: f['start'])
fstarts = [f['start'] for f in fs]
atom_starts = set(a['start'] for a in ATOMS)

ANCHOR_UNITS = {'ll': (0x8C03A1B8, 0x8C03A35E), 'drivetra': (0x8C022CA4, 0x8C0232F8), 'timer': (0x8C0100A4, 0x8C01020E)}
KNOWN_NAMES = {0x8C09B680: '__divls', 0x8C09B82C: '__modls', 0x8C09B7DC: '__divws', 0x8C09B9AC: '__modws',
               0x8C09BC04: 'bsearch', 0x8C010000: '_start_stub', 0x8C0D2480: 'startup_entry_8C0D2480',
               0x8C0100A4: 'RetraceCallback'}

# ---------------------------------------------------------------- SDK region 2: tag atoms by strings
TAGS = [(re.compile(r'KAMUI|^km'), 'kamui2'),
        (re.compile(r'KATANA_FLASH'), 'shinobi_flash'),
        (re.compile(r'ADDRESS CHECKER|FR0-7'), 'shinobi_debug'),
        (re.compile(r'E81012'), 'cri_adx_fs'),
        (re.compile(r'MPS Version'), 'cri_mps'),
        (re.compile(r'MPV Version'), 'cri_mpv'),
        (re.compile(r'SFD|mwSfd|mwPly|E2\d\d\d|E9\d+|W2000'), 'cri_sofdec'),
        (re.compile(r'mwRna|E12[0-3]\d'), 'cri_mw_rna_snd'),
        (re.compile(r'mwRnv2'), 'cri_mw_rnv'),
        (re.compile(r'mwMng'), 'cri_mw_mng'),
        (re.compile(r'CD001'), 'gdfs'),
        (re.compile(r'sg_dpldr'), 'shinobi_dpldr'),
        (re.compile(r'\(no device\)|Lib Handle'), 'shinobi'),
        (re.compile(r'FATAL ERROR'), 'sdk_fatal'),
        (re.compile(r'^-{10}'), 'startup_banner')]


def sdk2_units():
    lo, hi = 0x8C0D2480, END
    units = []
    for a in ATOMS:
        if not (lo <= a['start'] < hi):
            continue
        tag = None
        for f in fs[a['first']:a['last'] + 1]:
            for s in FEAT[h(f['start'])]['strings']:
                for rx, t in TAGS:
                    if rx.search(s):
                        tag = t; break
                if tag: break
            if tag: break
        if tag and (not units or units[-1][1] != tag):
            units.append([a['start'], tag])
        elif not units:
            units.append([a['start'], 'startup'])
    return units


# ---------------------------------------------------------------- unit list (code + data)
DATA_UNITS = [
    (0x8C0A77B4, 'section_table', 'startup', 'high', 'C section start: startup section table (C/D/B bounds: D 0x8C0B8C20, '
     'B 0x8C1224C0..0x8C1A9014) followed by build date/time strings. Referenced only by SDK startup code.'),
    (0x8C0A783C, 'C_game', 'data', 'medium', 'game constant data (C section: strings, tables) referenced only by game-region code; '
     'per-unit split not attempted. End = first library-referenced word.'),
    (0x8C0B3734, 'C_libs', 'data', 'medium', 'library constant data (Audio64 / C runtime strings and tables).'),
    (0x8C0B8C20, 'D_game', 'data', 'high', 'D section start (section table). Game initialized data: timer.c D at 0x8C0B8C24 (calibrated), '
     'gWaveFilenames table 0x8C0C44C0. End = first library-referenced word (medium).'),
    (0x8C0D231C, 'D_libs', 'data', 'medium', 'library initialized data up to the second code region.'),
]
ALL = []
for s, n, src, kind, conf, notes in SPEC:
    ALL.append(dict(start=s, name=n, source=src, kind=kind, confidence=conf, notes=notes))
for s, n, kind, conf, notes in DATA_UNITS:
    ALL.append(dict(start=s, name=n, source=None, kind=kind, confidence=conf, notes=notes))
tagged = sdk2_units()
SDKNOTE = {'startup': 'Shinobi/syStart startup reached from the entry stub (0x8C0D2480) and adjacent objects.',
           'kamui2': 'Kamui2 16,3,4,0 (banner "KAMUI2 Copyright (C) NEC").', 'gdfs': 'GD file system (ISO9660 "CD001").',
           'shinobi': 'Shinobi 1.68 objects (peripheral / library handle).', 'shinobi_flash': 'Shinobi flash-memory objects (KATANA_FLASH).',
           'shinobi_debug': 'Shinobi debug/exception reporting (ADDRESS CHECKER TRAP).', 'shinobi_dpldr': 'Shinobi DP loader (sg_dpldr.bin).',
           'cri_sofdec': 'CRI Sofdec / MW player (mwPly*, SFD 1.31).', 'cri_mw_mng': 'CRI middleware manager (mwMng).',
           'cri_mw_rnv': 'CRI MW renderer (mwRnv2).', 'cri_mw_rna_snd': 'CRI MW audio (mwRna / mwSnd).', 'cri_adx_fs': 'CRI ADX file system (E8101xxx).',
           'cri_mps': 'CRI MPS 1.31.', 'cri_mpv': 'CRI MPV 1.31 (MPEG video).', 'sdk_fatal': 'objects with "***FATAL ERROR" diagnostics.',
           'startup_banner': 'startup banner print.'}
for s, t in tagged:
    ALL.append(dict(start=s, name='sdk_%s_%08X' % (t, s), source=None, kind='startup' if t == 'startup' else 'sdk', confidence='low',
                    notes='Coarse SDK unit, first atom tagged by strings: ' + SDKNOTE.get(t, t) + ' Untagged following atoms are absorbed; object boundaries inside are not resolved.'))
# data blocks inside the second region (no code coverage)
for s, n, notes in [(0x8C1151DC, 'sdk_data_8C1151DC', 'data block inside the second code region referenced only by SDK code (pointer tables, constants).'),
                    (0x8C11BB0E, 'sdk_data_8C11BB0E', 'data block (high printable ratio, packed records); not referenced by decoded code: audio/driver or font payload (unknown).'),
                    (0x8C121F60, 'sdk_code_8C121F60', 'small code block after the data (SDK, unknown object).')]:
    ALL.append(dict(start=s, name=n, source=None, kind='data' if 'data' in n else 'sdk', confidence='low', notes=notes))
# code after the data blocks (between 0x8C11A4A0 and 0x8C11BB0E) gets its own coarse unit
ALL.append(dict(start=0x8C11A4A0, name='sdk_code_8C11A4A0', source=None, kind='sdk', confidence='low', notes='code between SDK data blocks (unknown object).'))
ALL.sort(key=lambda u: u['start'])
# remove SDK tagged starts that fall inside the data/code blocks we defined explicitly
clean = []
for u in ALL:
    if clean and u['start'] == clean[-1]['start']:
        continue
    clean.append(u)
ALL = clean
for i, u in enumerate(ALL):
    u['end'] = ALL[i + 1]['start'] if i + 1 < len(ALL) else END

warnings = []
for u in ALL:
    if u['kind'] not in ('data',) and u['start'] < 0x8C0A77B2 and u['start'] not in atom_starts and u['start'] != BASE:
        warnings.append('unit %s start %s is not an atom start' % (u['name'], h(u['start'])))

# ---------------------------------------------------------------- functions + pools per unit
def runs(lo, hi, kind):
    out = []; i = lo
    while i < hi:
        if cov[i - BASE] == kind:
            j = i
            while j < hi and cov[j - BASE] == kind: j += 1
            out.append([i, j]); i = j
        else:
            i += 1
    return out


def pool_runs(lo, hi):
    """Pool/table runs inside [lo,hi): merge runs separated by <= 4 uncovered bytes (half-used .w slots,
    alignment) and extend each run backwards over an uncovered 0000/0009 alignment pad."""
    rr = runs(lo, hi, 2)
    merged = []
    for s0, e0 in rr:
        while s0 - 2 >= lo and cov[s0 - 2 - BASE] == 0 and w16(s0 - 2) in (0, 9) and (s0 - 2) % 4 == 2:
            s0 -= 2
        if merged and s0 - merged[-1][1] <= 4 and all(cov[x - BASE] == 0 for x in range(merged[-1][1], s0)):
            merged[-1][1] = e0
        else:
            merged.append([s0, e0])
    return merged


def dp_names(u, funcs):
    """DP-align unit functions with the MAT3 file; returns {addr: (name, conf, evidence)}."""
    out = {}
    if not u['source']:
        return out
    fname = u['source'].split('/')[-1]
    ds = [f['start'] for f in funcs if f['start'] in MT.dpos]
    if not ds:
        return out
    sc, pairs = segdp.align(ds, fname)
    acc = set()
    for i, (d, k, s) in enumerate(pairs):
        if not (d and k):
            continue
        g = MT.match.get(d)
        ev = []
        if g == k:
            ev.append('global-match(%s)' % '+'.join(MT.evid.get(d, [])[:3]))
        ev.append('dp=%.1f' % s)
        if MT.evid.get(d) == ['anchor']:
            conf = 'high'
        elif g == k and s >= 6:
            conf = 'high'
        elif s >= 3 or g == k:
            conf = 'medium'
        elif s >= 0.5:
            conf = 'low'
        else:
            continue
        out[d] = (MT.mby[k]['name'], conf, ';'.join(ev))
        acc.add(i)
    # order runs: up to 3 consecutive unaccepted DC/MAT3 pairs (MAT3-only gaps allowed inside) bounded on both sides by
    # accepted pairs or the unit edge, each with a plausible size ratio -> low confidence by order + size
    idx = [i for i, (d, k, s) in enumerate(pairs) if not (k and not d)]   # drop MAT3-only entries
    seq = [pairs[i] + (i,) for i in idx]
    j = 0
    while j < len(seq):
        if seq[j][3] in acc:
            j += 1; continue
        k2 = j
        while k2 < len(seq) and seq[k2][3] not in acc:
            k2 += 1
        run = seq[j:k2]
        left_ok = j == 0 or seq[j - 1][3] in acc
        right_ok = k2 == len(seq) or seq[k2][3] in acc
        if left_ok and right_ok and len(run) <= 3 and all(d and k for d, k, s, i in run):
            ok = True
            for d, k, s, i in run:
                gs = MT.mby[k]['gc_size']
                r = MT.dsize[d] / (0.75 * gs) if gs > 0 else 0
                if not (0.45 <= r <= 2.2):
                    ok = False
            if ok:
                for d, k, s, i in run:
                    r = MT.dsize[d] / (0.75 * MT.mby[k]['gc_size'])
                    out[d] = (MT.mby[k]['name'], 'low', 'order-run%d+size(r=%.2f);dp=%.1f' % (len(run), r, s))
        j = k2
    return out


SEED = {}
_seedp = os.path.join(MAP, 'units.seed.json')
if os.path.isfile(_seedp):
    SEED = {x['name']: x for x in json.load(open(_seedp))['units']}
units_json = []
rows = []
stats = collections.Counter()
for u in ALL:
    lo, hi = u['start'], u['end']
    funcs = [f for f in fs if lo <= f['start'] < hi]
    if u['kind'] == 'data' and funcs:
        u['notes'] += ' (%d decoder candidate entries inside this data block were discarded.)' % len(funcs)
        funcs = []
    names = dp_names(u, funcs) if u['kind'] == 'game' else {}
    fl = []
    for f in funcs:
        a = f['start']
        end = min(f['end'], hi)
        if a in names:
            nm, conf, ev = names[a]
            m3 = nm
        else:
            nm, conf, ev, m3 = 'func_%08X' % a, 'low', 'boundary-only', ''
        if a in KNOWN_NAMES:
            nm, conf, ev = KNOWN_NAMES[a], 'high', 'established anchor (handoff)'
            m3 = nm if nm == 'bsearch' else ''
        fevid = '+'.join(F[h(a)]['evidence'])
        ev = ev + ';entry=' + fevid
        fl.append(dict(name=nm, start=h(a), end=h(end), mat3=m3 or None, confidence=conf))
        rows.append([u['name'], nm, h(a), h(end), end - a, m3, conf, ev])
        stats['fn_' + conf] += 1
        if m3:
            stats['fn_named'] += 1
    pools = [dict(start=h(s), end=h(e)) for s, e in pool_runs(lo, hi)] if u['kind'] != 'data' else []
    textend = hi
    ju = dict(name=u['name'], source=u['source'], kind=u['kind'], confidence=u['confidence'],
              text=dict(start=h(lo), end=h(textend)), functions=fl, pools=pools, data=[], bss=[], notes=u['notes'])
    if u['name'] in SEED and SEED[u['name']]['text'] == ju['text']:
        # data/bss layout established by the calibration (map/units.seed.json); copied, not re-derived
        for key in ('data', 'bss'):
            if SEED[u['name']].get(key):
                ju[key] = SEED[u['name']][key]
    if u['source'] and u['kind'] == 'game':
        fname = u['source'].split('/')[-1]
        nm3 = len(MT.mfile_list.get(fname, []))
        ju['notes'] = (ju['notes'] + ' ' if ju['notes'] else '') + 'DC functions %d, MAT3 functions %d, named %d.' % (
            len(fl), nm3, sum(1 for x in fl if x['mat3']))
    units_json.append(ju)
    stats['bytes_' + u['kind']] += hi - lo
    stats['units_' + u['kind']] += 1

# ---------------------------------------------------------------- call-graph cross-check of all names
name_of = {}
for u in units_json:
    for f in u['functions']:
        if f['mat3']:
            name_of[int(f['start'], 16)] = (f['mat3'], u['source'])
mcalls_by = collections.defaultdict(set)
for m in MT.M:
    mcalls_by[(m['file'], m['name'])] |= set(m['calls'])
rowidx = {r[2]: r for r in rows}
upg = collections.Counter()
for u in units_json:
    for f in u['functions']:
        if not f['mat3']:
            continue
        a = int(f['start'], 16)
        fname = (u['source'] or '').split('/')[-1]
        mine = mcalls_by.get((fname, f['mat3']), set())
        conf_n = contra = 0
        for t in MT.dcalls.get(a, []):
            if t in name_of and t != a:
                if name_of[t][0] in mine: conf_n += 1
                else: contra += 1
        for c in MT.dcallers.get(a, []):
            if c in name_of and c != a:
                cf = (name_of[c][1] or '').split('/')[-1]
                if f['mat3'] in mcalls_by.get((cf, name_of[c][0]), set()): conf_n += 1
        r = rowidx[f['start']]
        r[7] += ';callgraph=+%d/-%d' % (conf_n, contra)
        new = f['confidence']
        if f['confidence'] == 'low' and conf_n >= 1:
            new = 'medium'
        elif f['confidence'] == 'medium' and conf_n >= 2 and 'global-match' not in r[7]:
            new = 'high'
        if new != f['confidence']:
            upg[f['confidence'] + '->' + new] += 1
            f['confidence'] = new; r[6] = new
print('call-graph upgrades', dict(upg))
stats = collections.Counter()
for u in units_json:
    stats['bytes_' + u['kind']] += int(u['text']['end'], 16) - int(u['text']['start'], 16)
    stats['units_' + u['kind']] += 1
    for f in u['functions']:
        stats['fn_total'] += 1
        if f['mat3']:
            stats['named_' + f['confidence']] += 1
        else:
            stats['unnamed'] += 1

# anchor verification
for nm, (s, e) in ANCHOR_UNITS.items():
    u = next(x for x in units_json if x['name'] == nm)
    fns = u['functions']
    code_end = int(fns[-1]['end'], 16)
    ok = int(u['text']['start'], 16) == s and e in (code_end, int(u['text']['end'], 16))
    print('anchor %-9s unit %s..%s  last code end %s  expected %s..%s  %s' % (nm, u['text']['start'], u['text']['end'], h(code_end), h(s), h(e), 'OK' if ok else 'MISMATCH'))

# coverage/contiguity check
prev = BASE
for u in units_json:
    s, e = int(u['text']['start'], 16), int(u['text']['end'], 16)
    assert s == prev, (u['name'], h(s), h(prev))
    assert e > s
    prev = e
assert prev == END
out = dict(base=h(BASE), image_size=SIZE, generated=datetime.date.today().isoformat(),
           method='see README.md', units=units_json)
json.dump(out, open(os.path.join(MAP, 'units.json'), 'w'), indent=1)
with open(os.path.join(MAP, 'functions.csv'), 'w', newline='') as fh:
    w = csv.writer(fh)
    w.writerow(['unit', 'name', 'start', 'end', 'size', 'mat3_name', 'confidence', 'evidence'])
    w.writerows(rows)
json.dump(dict(stats), open(os.path.join(RAW, 'build_stats.json'), 'w'), indent=1)
with open(os.path.join(RAW, 'units.tsv'), 'w') as fh:   # input for ApplyUnitMap.java
    for u in units_json:
        fh.write(chr(9).join([u['text']['start'][2:], u['text']['end'][2:], u['name'], u['source'] or '-', u['kind'], u['confidence']]) + chr(10))
for x in warnings:
    print('WARN', x)
print(dict(stats))
print('units', len(units_json), 'functions', len(rows))
