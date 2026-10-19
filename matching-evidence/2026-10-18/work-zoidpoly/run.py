import sys, os, json, pathlib
root = pathlib.Path(r'C:\rush2049-dreamcast')
sys.path.insert(0,str(root/'tools'))
import dc_build as B
C=B.C
work=pathlib.Path(__file__).parent
label=sys.argv[1] if len(sys.argv)>1 else 'baseline'
out=work/label
out.mkdir(parents=True,exist_ok=True)
C.BUILD=str(out)
C.OBJ_DIR=str(out/'obj')
C.LINK_DIR=str(out/'link')
for p in (C.OBJ_DIR,C.LINK_DIR): os.makedirs(p,exist_ok=True)
unit=next(u for u in C.load_map()['units'] if u['name']=='zoidpoly')
entry=B.build_unit(unit,C.read_symbol_file(C.SYMBOLS_TXT),C.read_symbol_file(C.EXTERNS_TXT))
(out/'result.json').write_text(json.dumps(entry,indent=2))
print(json.dumps(entry,indent=2))
if entry['status']!='failed':
    try: B.listing(unit,entry,180)
    except ModuleNotFoundError as e: print('Listing unavailable:',e)
