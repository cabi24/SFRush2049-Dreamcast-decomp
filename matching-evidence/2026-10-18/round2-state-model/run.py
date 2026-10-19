import sys,os,json
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
C=B.C
unit,label=sys.argv[1:]
C.BUILD=os.path.abspath('build/round2-state-model/'+unit+'/'+label)
C.OBJ_DIR=os.path.join(C.BUILD,'obj');C.LINK_DIR=os.path.join(C.BUILD,'link')
for p in (C.BUILD,C.OBJ_DIR,C.LINK_DIR):os.makedirs(p,exist_ok=True)
u=next(u for u in C.load_map(None)['units'] if u['name']==unit)
e=B.build_unit(u,C.read_symbol_file(C.SYMBOLS_TXT),C.read_symbol_file(C.EXTERNS_TXT))
open(os.path.join(C.BUILD,'result.json'),'w').write(json.dumps(e,indent=2))
print(json.dumps(e,indent=2))
