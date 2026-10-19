import sys,os,json,pathlib,shutil
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
C=B.C
unit,label=sys.argv[1:3]
p=pathlib.Path('matching-evidence/2026-10-18/campaign60-lane5');p.mkdir(parents=True,exist_ok=True)
out=pathlib.Path('build/campaign60-lane5')/unit/label
C.BUILD=str(out.resolve());C.OBJ_DIR=str((out/'obj').resolve());C.LINK_DIR=str((out/'link').resolve())
for d in (C.BUILD,C.OBJ_DIR,C.LINK_DIR):os.makedirs(d,exist_ok=True)
m=C.load_map(None);u=next(x for x in m['units'] if x['name']==unit)
q=p/unit;q.mkdir(exist_ok=True)
if label=='baseline':shutil.copyfile('src/'+unit+'.c',q/'baseline.c')
r=B.build_unit(u,C.read_symbol_file(C.SYMBOLS_TXT),C.read_symbol_file(C.EXTERNS_TXT))
(q/(label+'.json')).write_text(json.dumps(r,indent=2));shutil.copyfile('src/'+unit+'.c',q/(label+'.c'))
print(unit,label,r['status'],r['functions_exact'],r['exact_text_bytes'],r['exact_data_bytes'])
print(json.dumps(sorted([f for f in r['functions'] if f['status']!='exact'],key=lambda f:f.get('diff_bytes',10000))[:5]))
