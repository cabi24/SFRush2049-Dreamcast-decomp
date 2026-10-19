import sys,os,json,pathlib,shutil
root=pathlib.Path(r'C:\rush2049-dreamcast'); sys.path.insert(0,str(root/'tools'))
import dc_common as C, dc_build as B
base=root/'matching-evidence/2026-10-18/work-textureanim'; label=sys.argv[1]; out=base/label
out.mkdir(parents=True,exist_ok=True)
C.BUILD=str(out); C.OBJ_DIR=str(out/'obj'); C.LINK_DIR=str(out/'link')
for d in (C.OBJ_DIR,C.LINK_DIR): os.makedirs(d,exist_ok=True)
shutil.copyfile(root/'src/textureanim.c',out/'source.c')
m=C.load_map(); u=next(u for u in m['units'] if u['name']=='textureanim')
e=B.build_unit(u,C.read_symbol_file(C.SYMBOLS_TXT),C.read_symbol_file(C.EXTERNS_TXT)); (out/'result.json').write_text(json.dumps(e,indent=2))
print(json.dumps(e,indent=2))
