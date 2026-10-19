import sys,os,json,hashlib,shutil
from pathlib import Path
sys.path.insert(0,os.path.abspath('tools'))
sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_build as B
import sh4
C=B.C
name,label=sys.argv[1:3]
root=Path('build/campaign100-lane4')
p=root/name/label
C.BUILD=str(p.resolve());C.OBJ_DIR=str((p/'obj').resolve());C.LINK_DIR=str((p/'link').resolve())
for d in (p,p/'obj',p/'link'):d.mkdir(parents=True,exist_ok=True)
src=Path('src')/(name+'.c')
if label=='baseline' and not (root/name/'original.c').exists():shutil.copyfile(src,root/name/'original.c')
u=next(u for u in C.load_map(None)['units'] if u['name']==name)
if len(sys.argv)>3 and sys.argv[3]=='diff':
 e=json.loads((p/'result.json').read_text());_,im=B.built_image(str(p/'link'/(name+'.abs')))
 f=next(f for f in e['functions'] if f['name']==sys.argv[4])
 st,en=int(f['start'],16),int(f['end'],16)
 for a in range(st,en,2):
  ours=int(f.get('linked_at',f['start']),16) if 'aligned' in sys.argv else st
  w=C.target_bytes(a,a+2);g=bytes(im.get(a-st+ours+k,0) for k in range(2))
  if w!=g or 'all' in sys.argv:
   print('%08X %s %-35s | %s %s'%(a,g.hex(),sh4.decode(int.from_bytes(g,'little'),a).text,w.hex(),sh4.decode(int.from_bytes(w,'little'),a).text))
 sys.exit()
e=B.build_unit(u,C.read_symbol_file(C.SYMBOLS_TXT),C.read_symbol_file(C.EXTERNS_TXT))
(p/'result.json').write_text(json.dumps(e,indent=2))
print(name,label,e['status'],'text',e['exact_text_bytes'],'data',e['exact_data_bytes'])
for f in e['functions']:print(f['name'],f['status'],f['diff_bytes'],f.get('linked_at','static'),f.get('linked_size',''),f['size'])
if e['notes']:print(e['notes'])

