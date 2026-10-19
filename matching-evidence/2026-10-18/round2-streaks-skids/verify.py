from pathlib import Path
import hashlib,json,sys
sys.path.insert(0,str(Path('tools').resolve()))
from sysrof_abs import read_abs
p=Path('matching-evidence/2026-10-18/round2-streaks-skids')
target=Path('pilot/orig/1ST_READ.BIN').read_bytes()
_,runs=read_abs(p/'streaks-int-temp/link/streaks.abs')
lo,hi=0x8c098f32,0x8c098f68
emitted={a+i:b for _,a,d in runs for i,b in enumerate(d) if lo<=a+i<hi}
assert all(v==target[a-0x8c010000] for a,v in emitted.items())
assert all(a in emitted for a in range(lo,hi) if a not in (0x8c098f36,0x8c098f37))
files=['src/streaks.c','src/skids.c',str(p/'streaks-int-temp/link/streaks.abs'),str(p/'streaks-int-temp/obj/streaks.obj')]
meta={f:hashlib.sha256(Path(f).read_bytes()).hexdigest() for f in files}
meta['pool_raw_emitted_bytes']=len(emitted)
meta['pool_all_54_bytes_equal']=True
(p/'verification.json').write_text(json.dumps(meta,indent=2))
print(json.dumps(meta,indent=2))
