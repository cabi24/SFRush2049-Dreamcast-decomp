import sys, os, json
from pathlib import Path
ROOT=Path(r'C:\rush2049-dreamcast')
sys.path.insert(0,str(ROOT/'tools'))
import dc_build as D
C=D.C
work=Path(__file__).resolve().parent
label=sys.argv[1] if len(sys.argv)>1 else 'baseline'
out=work/label
out.mkdir(exist_ok=True)
C.BUILD=str(out)
C.OBJ_DIR=str(out/'obj'); C.LINK_DIR=str(out/'link')
for p in (C.OBJ_DIR,C.LINK_DIR): os.makedirs(p,exist_ok=True)
m=C.load_map(); u=next(x for x in m['units'] if x['name']=='mb_wad')
symbols=C.read_symbol_file(str(ROOT/'build/target/symbols.txt'))
externs=C.read_symbol_file(str(ROOT/'src/externs.txt'))
e=D.build_unit(u,symbols,externs)
(out/'result.json').write_text(json.dumps(e,indent=2))
(out/'source.c').write_bytes((ROOT/'src/mb_wad.c').read_bytes())
print(json.dumps(e,indent=2))
