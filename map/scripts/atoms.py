"""Minimal unit blocks ('atoms'): consecutive functions that must share a translation unit.
Constraints: shared/crossing literal pools, bsr / bra tail calls / fall-through, 4-alignment of unit starts.
-> raw/atoms.json"""
import json, bisect, collections
from common import *
D = json.load(open(os.path.join(RAW, 'dc_features.json')))
F = json.load(open(os.path.join(RAW, 'functions.json')))
cov = open(os.path.join(RAW, 'cover.bin'), 'rb').read()
fs = sorted(F.values(), key=lambda f: f['start'])
starts = [f['start'] for f in fs]
idx = {s: i for i, s in enumerate(starts)}
N = len(fs)
def fidx(a):
    return bisect.bisect_right(starts, a) - 1
link = [0] * (N + 1)   # link[k] > 0 : no cut allowed between function k-1 and k
reason = collections.defaultdict(set)
def nocut(i, j, why):
    if i > j: i, j = j, i
    for k in range(i + 1, j + 1):
        link[k] += 1; reason[k].add(why)
for i, f in enumerate(fs):
    for p, n in f['pools'].items():
        p = int(p, 16)
        if inimg(p) and p >= starts[0]:
            j = fidx(p)
            if j != i: nocut(i, j, 'pool')
    for t in f['bsr']:
        if t in idx: nocut(i, idx[t], 'bsr')
    for t in f['tails']:
        if t in idx: nocut(i, idx[t], 'tail')
    if i + 1 < N and f['end'] == starts[i + 1]:
        # fall-through possible only if the last instruction is not a terminator
        last = f['end'] - 4
        k, _ = classify(last)
        if k not in ('rts', 'rte', 'bra', 'jmp', 'braf'):
            nocut(i, i + 1, 'fallthrough')
# alignment: a unit must start 4-aligned
for k in range(1, N):
    if starts[k] % 4:
        link[k] += 1; reason[k].add('align')
# atoms
atoms = []
cur = [0]
for k in range(1, N):
    if link[k]:
        cur.append(k)
    else:
        atoms.append(cur); cur = [k]
atoms.append(cur)
def padbefore(k):
    """pad words between previous covered byte and function k start"""
    s = starts[k]; a = s - 2; pads = []
    while a >= BASE and cov[a - BASE] == 0 and w16(a) in (0, 9):
        pads.append(w16(a)); a -= 2
    return pads, (a + 2)
out = []
for at in atoms:
    s = starts[at[0]]
    pads, ps = padbefore(at[0])
    prevcode = ps > BASE and cov[ps - 1 - BASE] == 1
    out.append(dict(first=at[0], last=at[-1], start=s, end=fs[at[-1]]['end'], n=len(at),
                    pad=pads, pad_after_code=bool(pads) and prevcode))
json.dump(dict(atoms=out, link=link, reasons={k: sorted(v) for k, v in reason.items()}), open(os.path.join(RAW, 'atoms.json'), 'w'))
print('functions', N, 'atoms', len(out))
