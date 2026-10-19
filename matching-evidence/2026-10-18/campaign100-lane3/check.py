import sys,json,hashlib,os
from pathlib import Path
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
from sysrof_abs import read_abs
unit,label=sys.argv[1:3]
p=Path('matching-evidence/2026-10-18/campaign100-lane3')/unit/label
root=Path('build/campaign100-lane3')/unit/label
base=json.loads((p.parent/'baseline/result.json').read_text());result=json.loads((p/'result.json').read_text())
_,old=B.built_image(Path('build/campaign100-lane3')/unit/'baseline/link'/f'{unit}.abs')
_,new=B.built_image(root/'link'/f'{unit}.abs')
spans=[(int(f['start'],16),int(f['end'],16)) for f in base['functions'] if f['status']=='exact']+[(int(f['start'],16),int(f['end'],16)) for f in base['segments'] if f.get('exact') and f['kind'] in ('pool','data')]
assert all(new.get(a)==old[a] for lo,hi in spans for a in range(lo,hi))
report={'unit':unit,'label':label,'preserved_old_exact_spans':True,'delta_text':result['exact_text_bytes']-base['exact_text_bytes'],'delta_data':result['exact_data_bytes']-base['exact_data_bytes'],'delta_functions':result['functions_exact']-base['functions_exact'],'hashes':{str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in [Path('src')/f'{unit}.c',root/'obj'/f'{unit}.obj',root/'link'/f'{unit}.abs']}}
(p/'verification.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
