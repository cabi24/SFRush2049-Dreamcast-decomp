import sys,json,hashlib
from pathlib import Path
sys.path.insert(0,str(Path('tools').resolve()))
import dc_build as B
from sysrof_abs import read_abs
name,label,note=sys.argv[1:4]
root=Path('build/campaign60-lane4')/name
p=Path('matching-evidence/2026-10-18/campaign60-lane4')
a=json.loads((root/'baseline/result.json').read_text());b=json.loads((root/label/'result.json').read_text())
u=next(u for u in B.C.load_map(None)['units'] if u['name']==name)
_,ai=B.built_image(str(root/'baseline/link'/f'{name}.abs'));_,bi=B.built_image(str(root/label/'link'/f'{name}.abs'))
ranges=[(s['start'],s['end']) for s in B.text_segments(u)]+[(d['start'],d['end']) for d in u['data']]
for lo,hi in ranges:
 if B.compare_range(ai,lo,hi)[0]:assert B.compare_range(bi,lo,hi)[0],hex(lo)
old={f['name'] for f in a['functions'] if f['status']=='exact'};new={f['name'] for f in b['functions'] if f['status']=='exact'}
assert old<=new
paths=[Path('src')/f'{name}.c',root/label/'obj'/f'{name}.obj',root/label/'link'/f'{name}.abs',root/label/'pp'/f'{name}.c']
r={'unit':name,'selected':label,'baseline_functions':len(old),'final_functions':len(new),'new_functions':sorted(new-old),'baseline_text':a['exact_text_bytes'],'final_text':b['exact_text_bytes'],'baseline_data':a['exact_data_bytes'],'final_data':b['exact_data_bytes'],'note':note,'hashes':{str(q):hashlib.sha256(q.read_bytes()).hexdigest() for q in paths if q.exists()}}
(p/f'{name}.json').write_text(json.dumps(r,indent=2)+'\n')
progress=p/'progress.md';s=progress.read_text();lines=s.splitlines()
for k,line in enumerate(lines):
 if line.startswith('| '+name+' |'):lines[k]=f'| {name} | done | {len(new)-len(old)} | {b["exact_text_bytes"]-a["exact_text_bytes"]} | {note} |'
progress.write_text('\n'.join(lines)+'\n');print(json.dumps(r,indent=2))
