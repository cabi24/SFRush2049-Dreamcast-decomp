import json,hashlib,sys
from pathlib import Path
sys.path.insert(0,str(Path('tools').resolve()))
import dc_build as B
p=Path('matching-evidence/2026-10-18/campaign60-lane4')
names='controls options optionscr pakerr pane pausescr resurrect road save smoke'.split()
rs=[]
for name in names:
 r=json.loads((p/f'{name}.json').read_text());rs.append(r)
 src=Path('src')/f'{name}.c'
 assert hashlib.sha256(src.read_bytes()).hexdigest()==r['hashes'][str(src)],name+' source changed'
 root=Path('build/campaign60-lane4')/name/r['selected']
 assert (root/'pp'/f'{name}.c').read_text(encoding='latin-1')==B.expand_includes(str(src.resolve()))+'\n',name+' headers changed'
 e=json.loads((root/'result.json').read_text())
 assert e['source_sha256']==r['hashes'][str(src)],name+' stale native build'
 print(name,'functions',r['final_functions']-r['baseline_functions'],'text',r['final_text']-r['baseline_text'],'data',r['final_data']-r['baseline_data'])
out={'assignments_completed':len(rs),'delta_functions':sum(r['final_functions']-r['baseline_functions'] for r in rs),'delta_text':sum(r['final_text']-r['baseline_text'] for r in rs),'delta_data':sum(r['final_data']-r['baseline_data'] for r in rs),'units':rs,'canonical_outputs_written':False}
(p/'summary.json').write_text(json.dumps(out,indent=2)+'\n')
print('TOTAL',out['delta_functions'],out['delta_text'],out['delta_data'])
