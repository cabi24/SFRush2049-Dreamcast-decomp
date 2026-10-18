"""Recursive-descent function extents for the DC image.
Inputs: raw Ghidra export (functions, code ranges, refs). Output: raw/walk.json with functions,
instruction ownership, pool words and gaps."""
import json, collections
from common import *
import sh4dis

def load_ghidra():
    funcs = []
    for l in open(os.path.join(RAW, 'ghidra_functions.tsv')).read().splitlines()[1:]:
        e, n, b = l.split('\t')
        funcs.append(int(e, 16))
    comp = collections.defaultdict(set)   # braf/jmp -> targets
    calls = {}                            # jsr site -> target
    for l in open(os.path.join(RAW, 'ghidra_refs.tsv')):
        f, t, ty, op = l.rstrip('\n').split('\t')
        f, t = int(f, 16), int(t, 16)
        if ty == 'COMPUTED_JUMP':
            comp[f].add(t)
        elif ty == 'COMPUTED_CALL':
            calls[f] = t
    return funcs, comp, calls

VALID_CACHE = {}
def valid(va):
    if va not in VALID_CACHE:
        o = va - BASE
        t = next(sh4dis.disasm_words(DATA[o:o + 2], va))[2]
        VALID_CACHE[va] = t != '??'
    return VALID_CACHE[va]

def _prologue(p):
    if not inimg(p): return False
    w = w16(p)
    return (w & 0xFF0F) == 0x2F06 or w == 0x4F22 or (w & 0xFF0F) == 0xFF0B or ((w >> 8) == 0x7F and (w & 0x80) != 0)


def switch_bound(mova_at):
    """Entry count of an SHC switch table: look back from the mova for cmp/hs|cmp/hi Rm,Rn and the
    constant loaded into Rm (mov #imm or mov.w pool)."""
    for back in range(2, 40, 2):
        a = mova_at - back
        w = w16(a)
        if (w & 0xF00F) in (0x3002, 0x3006):   # cmp/hs Rm,Rn / cmp/hi Rm,Rn
            m = (w >> 4) & 15
            hs = (w & 0xF) == 2
            for b2 in range(2, 30, 2):
                w2 = w16(a - b2)
                if (w2 >> 12) == 0xE and ((w2 >> 8) & 15) == m:
                    v = w2 & 0xFF; v = v - 256 if v & 0x80 else v
                    return v if hs else v + 1
                if (w2 >> 12) == 0x9 and ((w2 >> 8) & 15) == m:
                    v = w16((a - b2) + 4 + 2 * (w2 & 0xFF)); v = v - 65536 if v & 0x8000 else v
                    return v if hs else v + 1
            return None
    return None

def walk(entry, entries, comp):
    """Return dict with insns(set), pools{addr:size}, tables{addr:size}, calls[], tails[], bad[]"""
    ins = set(); pools = {}; tables = {}; bsr = []; tails = []; bad = []; jsrs = []
    work = [entry]
    while work:
        a = work.pop()
        while True:
            if not inimg(a) or a in ins:
                break
            if a != entry and a in entries:
                tails.append(a); break
            if not valid(a):
                bad.append(a); break
            ins.add(a)
            k, t = classify(a)
            if k in ('ldl', 'mova'):
                pools[t] = max(4, pools.get(t, 0))
            elif k == 'ldw':
                pools.setdefault(t, 2)
            if k in DELAYED:
                ds = a + 2
                if valid(ds):
                    ins.add(ds)
                    k2, t2 = classify(ds)
                    if k2 in ('ldl', 'mova'): pools[t2] = max(4, pools.get(t2, 0))
                    elif k2 == 'ldw': pools.setdefault(t2, 2)
                else:
                    bad.append(ds)
            if k in ('rts', 'rte'):
                break
            if k == 'bra':
                ds = w16(a + 2)
                pop = ds == 0x4F26 or (ds & 0xF0FF) == 0x60F6 or (ds & 0xF0FF) == 0xF0F9 or ((ds >> 8) == 0x7F and not ds & 0x80)
                if (t in entries and t != entry) or (pop and t != entry and not (entry <= t < a) and _prologue(t)):
                    tails.append(t)
                else:
                    work.append(t)
                break
            if k == 'jmp':
                tails.append(None)
                break
            if k == 'braf':
                tg = set(comp.get(a, set()))
                tt = None
                for back in range(2, 14, 2):
                    kk, t0 = classify(a - back)
                    if kk == 'mova':
                        tt = t0; break
                if tt is not None:
                    n = switch_bound(a - back)
                    base = a + 4
                    if n is None:
                        n = 0
                        while n < 512:
                            v = w16(tt + 2 * n); v = v - 65536 if v & 0x8000 else v
                            if base + v in tg: n += 1
                            else: break
                    for i in range(n):
                        v = w16(tt + 2 * i); v = v - 65536 if v & 0x8000 else v
                        tg.add(base + v)
                    if n: tables[tt] = 2 * n
                for x in tg: work.append(x)
                break
            if k in ('bt', 'bf'):
                work.append(t); a += 2; continue
            if k in ('bts', 'bfs'):
                work.append(t); a += 4; continue
            if k == 'bsr':
                bsr.append((a, t)); a += 4; continue
            if k in ('jsr', 'bsrf'):
                jsrs.append(a); a += 4; continue
            a += 2
    return dict(ins=ins, pools=pools, tables=tables, bsr=bsr, tails=tails, bad=bad, jsrs=jsrs)

if __name__ == '__main__':
    gfun, comp, calls = load_ghidra()
    extra = [0x8C0100A4, 0x8C0231D2, 0x8C010000]
    entries = set(gfun) | set(extra)
    res = {e: walk(e, entries, comp) for e in sorted(entries)}
    # report anomalies
    nbad = sum(1 for r in res.values() if r['bad'])
    print('functions', len(res), 'with bad decode', nbad)
    json.dump({h(e): dict(start=e, end=max(r['ins']) + 2 if r['ins'] else e, n=len(r['ins']),
                         pools={h(k): v for k, v in r['pools'].items()}, tables={h(k): v for k, v in r['tables'].items()},
                         bsr=[[x, y] for x, y in r['bsr']], tails=[t for t in r['tails']], bad=r['bad'], jsrs=r['jsrs'])
               for e, r in res.items()}, open(os.path.join(RAW, 'walk0.json'), 'w'))
