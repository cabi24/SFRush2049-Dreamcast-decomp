from pathlib import Path
import sys,os,json,hashlib
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
C=B.C
P=Path('matching-evidence/2026-10-18/campaign60-lane2')
units='camera checkpoint circuit collision controlsys controlscr drivsym fmath game gameoverscr'.split()
for unit in (units if sys.argv[1]=='all' else [sys.argv[1]]):
 label=sys.argv[2]
 if label=='baseline':
  assert not (P/(unit+'.original.c')).exists()
  (P/(unit+'.original.c')).write_bytes(Path('src/'+unit+'.c').read_bytes())
 C.BUILD=os.path.abspath('build/campaign60-lane2/'+unit+'/'+label)
 C.OBJ_DIR=os.path.join(C.BUILD,'obj');C.LINK_DIR=os.path.join(C.BUILD,'link')
 for x in (C.BUILD,C.OBJ_DIR,C.LINK_DIR):os.makedirs(x,exist_ok=True)
 u=next(x for x in C.load_map(None)['units'] if x['name']==unit)
 e=B.build_unit(u,C.read_symbol_file(C.SYMBOLS_TXT),C.read_symbol_file(C.EXTERNS_TXT))
 (P/(unit+'-'+label+'.json')).write_text(json.dumps(e,indent=2))
 print(unit,label,'exact',e.get('functions_exact'),'credit',e.get('exact_text_bytes'),'delta',e.get('text_size_delta'))
 print([(f['name'],f['size'],f['diff_bytes']) for f in e.get('functions',[]) if f['status']!='exact'])
