"""Order-preserving alignment of a run of DC functions against a MAT3 file's function list.
Imports match.py (runs the global propagation first, ~1 s)."""
import math
import match as MT
from common import *

GAP_DC = -1.0      # DC function without MAT3 counterpart
GAP_M = -0.1       # MAT3 function absent on DC (deadstripped, ifdef'd, inlined, moved)
BASE_PAIR = -0.5


def pair_score(d, k, tmp=None):
    m = MT.mby[k]
    s = BASE_PAIR
    ds, ms = MT.dstr[d], MT.mstr[k]
    common = ds & ms
    for x in common:
        s += 4.0 if MT.strfreq[x] <= 2 else 2.0 if MT.strfreq[x] <= 5 else 0.7
    if ds and ms and not common:
        s -= 2
    if ds and not ms and m['has_src']:
        s -= 1
    if ms and not ds:
        s -= 1
    for x in MT.mflt[k]:
        if any(MT.feq(x, y) for y in MT.dflt[d]):
            s += 2.0 if MT.fltfreq[x] <= 3 else 1.0 if MT.fltfreq[x] <= 15 else 0.3
    if m['gc_size'] > 0:
        r = abs(math.log(max(MT.dsize[d], 4) / (0.75 * m['gc_size'])))
        s += 1.0 - 1.5 * r
    else:
        s -= 0.7
    nd, nm = len(MT.dcalls[d]), len(MT.mcalls[k])
    if m['has_src']:
        if nd and nm:
            s += 0.5 - 0.15 * min(abs(nd - nm), 10)
        elif nd != nm:
            s -= 0.15 * min(abs(nd - nm), 10)
    if d in MT.match:
        s += 6.0 if MT.match[d] == k else -3.0
    names = set(m['calls'])
    for t in MT.dcalls[d]:
        nmk = MT.match.get(t) or (tmp.get(t) if tmp else None)
        if nmk:
            s += 1.0 if MT.mby[nmk]['name'] in names else -0.3
    return s


def align(daddrs, mfile, iters=2):
    keys = MT.mfile_list.get(mfile, [])
    tmp = {}
    for it in range(iters):
        sc, pairs = _align(daddrs, keys, tmp)
        tmp = {d: k for d, k, s in pairs if d and k and s > 0}
    return sc, pairs


def _align(daddrs, keys, tmp):
    n, m = len(daddrs), len(keys)
    S = [[0.0] * (m + 1) for _ in range(n + 1)]
    B = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        S[i][0] = S[i - 1][0] + GAP_DC; B[i][0] = 1
    for j in range(1, m + 1):
        S[0][j] = S[0][j - 1] + GAP_M; B[0][j] = 2
    P = {}
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            ps = pair_score(daddrs[i - 1], keys[j - 1], tmp)
            P[i, j] = ps
            a = S[i - 1][j - 1] + ps
            b = S[i - 1][j] + GAP_DC
            c = S[i][j - 1] + GAP_M
            if a >= b and a >= c:
                S[i][j] = a; B[i][j] = 0
            elif b >= c:
                S[i][j] = b; B[i][j] = 1
            else:
                S[i][j] = c; B[i][j] = 2
    i, j = n, m
    pairs = []
    while i > 0 or j > 0:
        if i > 0 and j > 0 and B[i][j] == 0:
            pairs.append((daddrs[i - 1], keys[j - 1], P[i, j])); i -= 1; j -= 1
        elif i > 0 and (j == 0 or B[i][j] == 1):
            pairs.append((daddrs[i - 1], None, GAP_DC)); i -= 1
        else:
            pairs.append((None, keys[j - 1], GAP_M)); j -= 1
    pairs.reverse()
    return S[n][m], pairs


def gain(daddrs, mfile):
    sc, pairs = align(daddrs, mfile)
    base = len(daddrs) * GAP_DC + len(MT.mfile_list.get(mfile, [])) * GAP_M
    return sc - base, pairs
