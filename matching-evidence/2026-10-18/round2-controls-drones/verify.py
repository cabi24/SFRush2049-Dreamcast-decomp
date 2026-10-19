import sys,os,json,hashlib
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
from sysrof_abs import read_abs
from pathlib import Path
p=Path('matching-evidence/2026-10-18/round2-controls-drones')
a=json.loads((p/'controls-baseline/result.json').read_text())
b=json.loads((p/'controls-final/result.json').read_text())
old={f['name'] for f in a['functions'] if f['status']=='exact'}
new={f['name'] for f in b['functions'] if f['status']=='exact'}
assert old<=new
assert new-old=={'UpdateRushControls'}
assert a['exact_data_bytes']==b['exact_data_bytes']==104
_,im=B.built_image(str(p/'controls-final/link/controls.abs'))
f=next(f for f in b['functions'] if f['name']=='UpdateRushControls')
lo,hi=int(f['start'],16),int(f['end'],16)
assert bytes(im[x] for x in range(lo,hi))==B.C.target_bytes(lo,hi)
_,runs=read_abs(str(p/'controls-final/link/controls.abs'))
produced={start+k for _,start,chunk in runs for k in range(len(chunk))}
earned=sum(x in produced for x in range(lo,hi))
assert b['exact_text_bytes']-a['exact_text_bytes']==earned
paths=['src/controls.c','src/drones.c',str(p/'controls-final/pp/controls.c'),str(p/'controls-final/obj/controls.obj'),str(p/'controls-final/link/controls.abs')]
report={'new_exact':sorted(new-old),'earned_text_bytes':earned,'new_range':[f['start'],f['end']],'baseline_text':a['exact_text_bytes'],'final_text':b['exact_text_bytes'],'data':104,'baseline_functions':len(old),'final_functions':len(new),'preserved_exact':sorted(old),'hashes':{s:hashlib.sha256(Path(s).read_bytes()).hexdigest() for s in paths}}
(p/'verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
