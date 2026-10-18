"""Final pass after functions.py: every uncovered stretch inside the code regions that starts (after 0000/0009 pads)
with an SHC prologue word and decodes cleanly (no overlap with existing code, valid pools) becomes a function with
evidence 'prologue-sweep'. Rewrites raw/functions.json and raw/cover.bin."""
import json
from common import *
from walk import load_ghidra, walk

CODE = [(0x8C010000, 0x8C0A77B2), (0x8C0D2480, 0x8C1151DC), (0x8C11A4A0, 0x8C11BB0E), (0x8C121F60, END)]
gfun, comp, gcalls = load_ghidra()
F = json.load(open(os.path.join(RAW, 'functions.json')))
cov = bytearray(open(os.path.join(RAW, 'cover.bin'), 'rb').read())
entries = set(f['start'] for f in F.values())


def prologue(p):
    w = w16(p)
    return (w & 0xFF0F) == 0x2F06 or w == 0x4F22 or (w & 0xFF0F) == 0xFF0B or ((w >> 8) == 0x7F and (w & 0x80))


added = []
for it in range(5):
    n0 = len(added)
    for lo, hi in CODE:
        a = lo
        while a < hi:
            if cov[a - BASE] != 0:
                a += 2; continue
            b = a
            while b < hi and cov[b - BASE] == 0:
                b += 2
            p = a
            while p < b and w16(p) in (0, 9):
                p += 2
            if p < b and prologue(p) and p not in entries:
                r = walk(p, entries | {p}, comp)
                ok = r['ins'] and not r['bad'] and min(r['ins']) >= p
                if ok:
                    for x in r['ins']:
                        if cov[x - BASE]:
                            ok = False; break
                if ok:
                    for t, n in r['pools'].items():
                        if not inimg(t) or t in r['ins'] or (n == 4 and (t % 4 or (t + 2) in r['ins'])):
                            ok = False; break
                if ok:
                    entries.add(p)
                    for x in r['ins']:
                        cov[x - BASE] = 1; cov[x - BASE + 1] = 1
                    for t, n in r['pools'].items():
                        for i in range(n):
                            if cov[t - BASE + i] == 0: cov[t - BASE + i] = 2
                    for t, n in r['tables'].items():
                        for i in range(n):
                            if cov[t - BASE + i] == 0: cov[t - BASE + i] = 2
                    F[h(p)] = dict(start=p, end=max(r['ins']) + 2, ninsn=len(r['ins']), evidence=['prologue-sweep'],
                                   pools={h(k): v for k, v in r['pools'].items()}, tables={h(k): v for k, v in r['tables'].items()},
                                   bsr=[t for s, t in r['bsr']], tails=[t for t in r['tails'] if t], jmp_tail=None in r['tails'],
                                   jsr=[gcalls.get(s) for s in r['jsrs']], bad=[])
                    added.append(p)
            a = b
    if len(added) == n0:
        break
json.dump(F, open(os.path.join(RAW, 'functions.json'), 'w'))
open(os.path.join(RAW, 'cover.bin'), 'wb').write(bytes(cov))
print('prologue-sweep added', len(added), [h(x) for x in added[:40]])
