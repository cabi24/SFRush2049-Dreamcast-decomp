"""Inspect a DC address range: functions, sizes, matched names, resolved calls (with names), callers, strings."""
import json, sys, collections
from common import *
X = json.load(open(os.path.join(RAW, 'matches.json')))
D = json.load(open(os.path.join(RAW, 'dc_features.json')))
lo, hi = int(sys.argv[1], 16), int(sys.argv[2], 16)
fs = sorted(D.values(), key=lambda f: f['start'])
callers = collections.defaultdict(list)
for f in fs:
    for c in f['calls']:
        if c[2] is not None:
            callers[c[2]].append(f['start'])
def nm(a):
    if a is None: return '?'
    x = X.get(h(a))
    return x['name'] if x else ('f%X' % a if a else '?')
for f in fs:
    if lo <= f['start'] < hi:
        x = X.get(h(f['start']))
        tag = '%s:%s(%.1f)' % (x['file'], x['name'], x['score']) if x else ''
        cl = [nm(c[2]) for c in f['calls']]
        cr = sorted(set(nm(c) for c in callers.get(f['start'], [])))
        print(h(f['start']), f['end'] - f['start'], tag)
        print('     calls:', ' '.join(cl[:14]))
        if cr: print('     callers:', ' '.join(cr[:10]))
        if f['strings']: print('     strings:', [s[:30] for s in f['strings'][:6]])
