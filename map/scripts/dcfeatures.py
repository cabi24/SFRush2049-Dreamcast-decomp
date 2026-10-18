"""Per-function features for DC: resolved call sequence, tail calls, pool values (strings/data/code).
-> raw/dc_features.json"""
import json
from common import *
from walk import load_ghidra, walk

def resolve_reg(ins_sorted, idx, reg):
    """Look back from ins_sorted[idx] for mov.l @(disp,pc),reg; stop at another write to reg (approx)."""
    for j in range(idx - 1, max(-1, idx - 40), -1):
        a = ins_sorted[j]
        w = w16(a)
        k, t = classify(a)
        if k == 'ldl' and ((w >> 8) & 15) == reg:
            return w32(t) if inimg(t) else None
        n = (w >> 8) & 15
        # crude: any mov/arith writing rN (formats 6nmx, 7nii, Enii, 9n.., Dn..)
        if n == reg and (w >> 12) in (0x6, 0x7, 0xE, 0x9):
            return None
    return None

if __name__ == '__main__':
    gfun, comp, gcalls = load_ghidra()
    F = json.load(open(os.path.join(RAW, 'functions.json')))
    entries = set(v['start'] for v in F.values())
    S = {int(k, 16): v for k, v in json.load(open(os.path.join(RAW, 'dc_strings.json'))).items()}
    out = {}
    for key, f in F.items():
        e = f['start']
        r = walk(e, entries, comp)
        ins = sorted(r['ins'])
        calls = []
        for i, a in enumerate(ins):
            k, t = classify(a)
            if k == 'bsr':
                calls.append(['bsr', a, t])
            elif k in ('jsr', 'jmp'):
                reg = (w16(a) >> 8) & 15
                tv = resolve_reg(ins, i, reg)
                if tv is None: tv = gcalls.get(a)
                if k == 'jsr' or (tv is not None and tv in entries):
                    calls.append([k, a, tv])
            elif k == 'bra' and t in entries and t != e:
                calls.append(['bra', a, t])
        pv = []
        for p, n in sorted(r['pools'].items()):
            if inimg(p) and n >= 4 and p % 4 == 0:
                pv.append([p, w32(p)])
            elif inimg(p) and n == 2:
                pv.append([p, w16(p)])
        strs = [S[v] for p, v in pv if v in S]
        out[key] = dict(start=e, end=f['end'], calls=calls, pools=pv, strings=strs,
                        nins=len(ins))
    json.dump(out, open(os.path.join(RAW, 'dc_features.json'), 'w'))
    print(len(out))
