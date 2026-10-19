import sys, os, json
from pathlib import Path
ROOT=Path(r'C:\rush2049-dreamcast')
sys.path.insert(0,str(ROOT/'tools'))
sys.path.insert(0,str(ROOT/'pilot/tools'))
import dc_build as D
import sh4
C=D.C
unit,label=sys.argv[1:3]
evidence=Path(__file__).resolve().parent
out=ROOT/'build/campaign60-lane1'/unit/label
out.mkdir(parents=True,exist_ok=True)
C.BUILD=str(out); C.OBJ_DIR=str(out/'obj'); C.LINK_DIR=str(out/'link')
for p in (C.OBJ_DIR,C.LINK_DIR): os.makedirs(p,exist_ok=True)
m=C.load_map(); u=next(x for x in m['units'] if x['name']==unit)
e=D.build_unit(u,C.read_symbol_file(str(ROOT/'build/target/symbols.txt')),C.read_symbol_file(str(ROOT/'src/externs.txt')))
(evidence/(unit+'-'+label+'.json')).write_text(json.dumps(e,indent=2))
(out/'source.c').write_bytes((ROOT/'src'/f'{unit}.c').read_bytes())
print(json.dumps({k:e[k] for k in ('name','status','exact_text_bytes','exact_data_bytes','functions_exact','functions')},indent=2))
if e['status']!='failed':
 _,image=D.built_image(str(out/'link'/f'{unit}.abs'))
 lines=[]
 for f in e['functions']:
  if f['status']=='exact': continue
  lo,hi=int(f['start'],16),int(f['end'],16)
  lines.append(str(f))
  for a in range(lo,hi,2):
   b=C.target_bytes(a,a+2); g=bytes(image.get(a+i,0) for i in range(2))
   lines.append(f'{a:08X} {g.hex()} {sh4.decode(int.from_bytes(g,"little"),a).text:40} | {b.hex()} {sh4.decode(int.from_bytes(b,"little"),a).text}'+(' *' if b!=g else ''))
 (evidence/(unit+'-'+label+'.txt')).write_text('\n'.join(lines))
