"""For each function: strings referenced through its pool words -> MAT3 files containing them."""
import json, collections
from common import *
F = json.load(open(os.path.join(RAW, 'functions.json')))
S = {int(k, 16): v for k, v in json.load(open(os.path.join(RAW, 'dc_strings.json'))).items()}
M = json.load(open(os.path.join(RAW, 'mat3_strings.json')))
def fstrings(f):
    out = []
    for p, n in f['pools'].items():
        p = int(p, 16)
        if n >= 4 and inimg(p) and p % 4 == 0:
            v = w32(p)
            if v in S: out.append(S[v])
    return out
if __name__ == '__main__':
    fs = sorted(F.values(), key=lambda f: f['start'])
    rows = []
    for f in fs:
        ss = fstrings(f)
        files = collections.Counter()
        for s in ss:
            for fl in M.get(s, []): files[fl] += 1
        if ss:
            rows.append((f['start'], ss, files))
    for a, ss, files in rows:
        if a < 0x8C0A8000:
            print(h(a), dict(files.most_common(3)), [s[:30] for s in ss[:4]])
