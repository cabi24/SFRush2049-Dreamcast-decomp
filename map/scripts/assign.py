"""Label atoms with MAT3 source files from function matches; merge into unit hypotheses.
-> raw/assign.json: list of segments [{start,end,first,last,file,weight,counts}] covering the game region atoms."""
import json, collections, sys
from common import *

X = json.load(open(os.path.join(RAW, 'matches.json')))
A = json.load(open(os.path.join(RAW, 'atoms.json')))
F = json.load(open(os.path.join(RAW, 'functions.json')))
fs = sorted(F.values(), key=lambda f: f['start'])
HI = 0x8C0A77B2
atoms = [a for a in A['atoms'] if a['start'] < HI]
lab = []
for a in atoms:
    c = collections.Counter()
    for f in fs[a['first']:a['last'] + 1]:
        x = X.get(h(f['start']))
        if x:
            c[x['file']] += min(x['score'], 20.0)
    if c:
        (best, w), = c.most_common(1)
        tot = sum(c.values())
        lab.append((best if w >= 0.6 * tot and w >= 4.0 else None, w, dict(c)))
    else:
        lab.append((None, 0, {}))
# contiguity fill: None atoms between two atoms of the same file get that file
labels = [l[0] for l in lab]
i = 0
while i < len(labels):
    if labels[i] is None:
        j = i
        while j < len(labels) and labels[j] is None:
            j += 1
        left = labels[i - 1] if i > 0 else None
        right = labels[j] if j < len(labels) else None
        if left and left == right:
            for k in range(i, j):
                labels[k] = left + '?'
        i = j
    else:
        i += 1
segs = []
for a, l, (b, w, c) in zip(atoms, labels, lab):
    key = l.rstrip('?') if l else None
    if segs and key is not None and segs[-1]['file'] == key:
        s = segs[-1]
        s['end'] = a['end']; s['last'] = a['last']; s['weight'] += w; s['atoms'] += 1
        for k, v in c.items():
            s['counts'][k] = s['counts'].get(k, 0) + v
        if a['pad_after_code']:
            s['inner_pad'].append(a['start'])
    else:
        segs.append(dict(start=a['start'], end=a['end'], first=a['first'], last=a['last'], file=key, weight=w,
                         counts=dict(c), atoms=1, inner_pad=[], pad_before=a['pad_after_code']))
json.dump(segs, open(os.path.join(RAW, 'assign.json'), 'w'), indent=0)
seen = collections.defaultdict(list)
for s in segs:
    if s['file']:
        seen[s['file']].append(h(s['start']))
if '-q' not in sys.argv:
    for s in segs:
        other = {k: round(v, 1) for k, v in s['counts'].items() if k != s['file']}
        print(h(s['start']), h(s['end']), s['last'] - s['first'] + 1, s['file'], round(s['weight'], 1), other,
              'PADBEFORE' if s['pad_before'] else '', 'INNERPAD' + str([x for x in s['inner_pad']]) if s['inner_pad'] else '')
    print('files in >1 segment:', {k: v for k, v in seen.items() if len(v) > 1})
