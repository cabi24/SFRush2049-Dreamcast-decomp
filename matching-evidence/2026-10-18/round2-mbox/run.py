import sys,os,json
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
C=B.C
packet=os.path.abspath('matching-evidence/2026-10-18/round2-mbox')
label=sys.argv[1]
C.BUILD=os.path.join(packet,label)
C.OBJ_DIR=os.path.join(C.BUILD,'obj');C.LINK_DIR=os.path.join(C.BUILD,'link')
for p in (C.BUILD,C.OBJ_DIR,C.LINK_DIR):os.makedirs(p,exist_ok=True)
m=C.load_map(None)
u=next(u for u in m['units'] if u['name']=='mbox')
e=B.build_unit(u,C.read_symbol_file(C.SYMBOLS_TXT),C.read_symbol_file(C.EXTERNS_TXT))
open(os.path.join(C.BUILD,'result.json'),'w').write(json.dumps(e,indent=2))
print(json.dumps(e,indent=2))

