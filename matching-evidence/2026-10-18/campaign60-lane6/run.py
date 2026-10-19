import sys,os,json,pathlib,hashlib
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
C=B.C
units=['zip','zoid','dc_unk_8C069E34','dc_sound_8C069F78','dc_audio64_init_8C06ADD4','dc_blit_8C06B2EC','dc_misc_8C06B80C','dc_misc_8C06C08C','paksysdc','dc_kamui_tex_8C06E5B4']
unit,label=sys.argv[1:3]
todo=units if unit=='all' else [unit]
packet=pathlib.Path('matching-evidence/2026-10-18/campaign60-lane6')
m=C.load_map(None)
symbols=C.read_symbol_file(C.SYMBOLS_TXT);externs=C.read_symbol_file(C.EXTERNS_TXT)
for name in todo:
 C.BUILD=os.path.abspath('build/campaign60-lane6/'+name+'/'+label)
 C.OBJ_DIR=os.path.join(C.BUILD,'obj');C.LINK_DIR=os.path.join(C.BUILD,'link')
 for p in (C.BUILD,C.OBJ_DIR,C.LINK_DIR):os.makedirs(p,exist_ok=True)
 u=next(u for u in m['units'] if u['name']==name)
 e=B.build_unit(u,symbols,externs)
 out=packet/name/label;out.mkdir(parents=True,exist_ok=True)
 (out/'source.c').write_bytes(pathlib.Path('src',name+'.c').read_bytes())
 (out/'result.json').write_text(json.dumps(e,indent=2))
 print(name,e['status'],e.get('functions_exact'),e.get('exact_text_bytes'),e.get('exact_data_bytes'),e.get('text_size_delta'),flush=True)
 print(json.dumps([f for f in e.get('functions',[]) if f['status']!='exact']),flush=True)
