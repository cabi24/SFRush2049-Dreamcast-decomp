import sys,json,pathlib
root=pathlib.Path(r'C:\rush2049-dreamcast')
sys.path.insert(0,str(root/'tools'))
import dc_build as B
C=B.C
work=root/'matching-evidence/2026-10-18/work-atan'
label=sys.argv[1] if len(sys.argv)>1 else 'baseline'
out=work/label
out.mkdir(parents=True,exist_ok=True)
C.BUILD=str(out); C.OBJ_DIR=str(out/'obj'); C.LINK_DIR=str(out/'link')
for d in [C.OBJ_DIR,C.LINK_DIR]: pathlib.Path(d).mkdir(exist_ok=True)
original=root/'src/atan.c'
flat=B.expand_includes(str(original))
if label!='baseline': flat=(work/(label+'.c')).read_text()
(out/'atan.c').write_text(flat,encoding='latin1')
C.SRC=str(out)
m=C.load_map(); u=next(u for u in m['units'] if u['name']=='atan')
symbols=C.read_symbol_file(str(root/'build/target/symbols.txt'))
externs=C.read_symbol_file(str(root/'src/externs.txt'))
r=B.build_unit(u,symbols,externs)
(out/'report.json').write_text(json.dumps(r,indent=2))
print(json.dumps(r,indent=2))
if r['status']!='failed':
 sections,im=B.built_image(str(out/'link/atan.abs'))
 lines=[]
 for a in range(u['text'][0],u['text'][1],2):
  want=C.target_bytes(a,a+2); got=bytes([im.get(a,0),im.get(a+1,0)])
  lines.append(f'{a:08X} {want.hex()} {got.hex()}'+(' *' if want!=got else ''))
 (out/'words.txt').write_text('\n'.join(lines))
 print('\n'.join(l for l in lines if '*' in l))
