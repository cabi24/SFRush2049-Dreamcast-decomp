"""Score a DC address range against MAT3 files: python tryfile.py LO HI [file ...] [-v]
Without files: tries every MAT3 file and prints the top 8."""
import sys
import segdp
import match as MT
from common import *

args = [a for a in sys.argv[1:] if not a.startswith('-')]
lo, hi = int(args[0], 16), int(args[1], 16)
files = args[2:] or sorted(MT.mfile_list)
ds = [a for a in MT.daddr if lo <= a < hi]
res = []
for fl in files:
    g, pairs = segdp.gain(ds, fl)
    nm = sum(1 for d, k, s in pairs if d and k)
    res.append((g, g, fl, nm, pairs))
res.sort(key=lambda r: -r[0])
for gain, sc, fl, nm, pairs in res[:8 if len(files) > 8 else len(files)]:
    print('%-18s gain %.1f score %.1f matched %d/%d dc, %d mat3' % (fl, gain, sc, nm, len(ds), len(MT.mfile_list[fl])))
if '-v' in sys.argv:
    gain, sc, fl, nm, pairs = res[0]
    for d, k, s in pairs:
        print('  ', h(d) if d else '-' * 10, (MT.dsize[d] if d else ''), k.split(':')[1] if k else '-', (MT.mby[k]['gc_size'] if k else ''), round(s, 1))
