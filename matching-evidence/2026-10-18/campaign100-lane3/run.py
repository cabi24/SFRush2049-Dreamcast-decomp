import sys,os,json,shutil
from pathlib import Path
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
C=B.C
packet=Path('matching-evidence/2026-10-18/campaign100-lane3')
unit,label=sys.argv[1:3]
C.BUILD=os.path.abspath('build/campaign100-lane3/'+unit+'/'+label)
C.OBJ_DIR=os.path.join(C.BUILD,'obj');C.LINK_DIR=os.path.join(C.BUILD,'link')
for p in (C.BUILD,C.OBJ_DIR,C.LINK_DIR):os.makedirs(p,exist_ok=True)
out=packet/unit/label;out.mkdir(parents=True,exist_ok=True)
m=C.load_map(None);u=next(u for u in m['units'] if u['name']==unit)
e=B.build_unit(u,C.read_symbol_file(C.SYMBOLS_TXT),C.read_symbol_file(C.EXTERNS_TXT))
(out/'result.json').write_text(json.dumps(e,indent=2))
shutil.copyfile('src/'+unit+'.c',out/'source.c')
print(unit,label,e['status'],e.get('exact_text_bytes'),e.get('functions_exact'))
for f in e.get('functions',[]):print(f['name'],f['start'],f['size'],f['status'],f.get('diff_bytes'))
