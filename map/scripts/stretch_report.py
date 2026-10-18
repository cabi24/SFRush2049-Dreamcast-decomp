"""For every low-confidence stretch between confident segments in the game region, rank MAT3 GameSource
files by DP alignment gain (per atom and for the whole stretch)."""
import json, sys
import segdp
import match as MT
from common import *

S = json.load(open(os.path.join(RAW, 'assign.json')))
A = json.load(open(os.path.join(RAW, 'atoms.json')))['atoms']
LIMIT = 0x8C09B1E4
conf = [s for s in S if s['file'] and s['weight'] >= 15 and s['start'] < LIMIT]
placed = set(s['file'] for s in conf)
gs = sorted(set(m['file'] for m in MT.M if not m['file'].startswith('ngc') and not m['file'].startswith('NGC')))
gs = [f for f in MT.mfile_list if not f.startswith('ngc')]
cache = {}
def g(ds, fl):
    key = (tuple(ds), fl)
    if key not in cache:
        cache[key] = segdp.gain(ds, fl)[0]
    return cache[key]
prev = None
out = []
segs = [s for s in S if s['start'] < LIMIT]
i = 0
while i < len(segs):
    s = segs[i]
    if s['file'] and s['weight'] >= 15:
        prev = s; i += 1; continue
    j = i
    while j < len(segs) and not (segs[j]['file'] and segs[j]['weight'] >= 15):
        j += 1
    nxt = segs[j] if j < len(segs) else None
    lo, hi = segs[i]['start'], segs[j - 1]['end']
    ds = [a for a in MT.daddr if lo <= a < hi]
    print('=== stretch %s..%s (%d funcs) between %s and %s' % (h(lo), h(hi), len(ds), prev['file'] if prev else None, nxt['file'] if nxt else None))
    ranks = sorted(((g(ds, fl), fl) for fl in gs), reverse=True)[:6]
    print('   whole:', ', '.join('%s%s %.1f' % (fl, '*' if fl in placed else '', v) for v, fl in ranks))
    for a in A:
        if lo <= a['start'] < hi:
            da = [x for x in MT.daddr if a['start'] <= x < a['end'] + 1 and x < hi]
            if not da: continue
            ranks = sorted(((g(da, fl), fl) for fl in gs), reverse=True)[:4]
            print('   atom %s..%s n=%d %s:' % (h(a['start']), h(a['end']), len(da), 'PAD' if a['pad_after_code'] else ''),
                  ', '.join('%s%s %.1f' % (fl, '*' if fl in placed else '', v) for v, fl in ranks))
    prev = nxt
    i = j
