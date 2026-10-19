import sys, json, hashlib
from pathlib import Path
ROOT=Path(r'C:\rush2049-dreamcast')
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'tools'))
import dc_build as D
C=D.C
unit,label=sys.argv[1:3]
assert 'a' <= unit[0].lower() <= 'l'
out=HERE/'runs'/unit/label
out.mkdir(parents=True,exist_ok=False)
src=HERE/'candidates'/unit
src.mkdir(parents=True,exist_ok=True)
candidate=src/(unit+'.c')
if not candidate.exists():
    candidate.write_text(D.expand_includes(str(ROOT/'src'/(unit+'.c'))),encoding='latin-1')
C.SRC=str(src); C.BUILD=str(out); C.OBJ_DIR=str(out/'obj'); C.LINK_DIR=str(out/'link')
Path(C.OBJ_DIR).mkdir(); Path(C.LINK_DIR).mkdir()
u=next(u for u in C.load_map()['units'] if u['name']==unit)
entry=D.build_unit(u,C.read_symbol_file(str(ROOT/'build/target/symbols.txt')),C.read_symbol_file(str(ROOT/'src/externs.txt')))
(out/'report.json').write_text(json.dumps(entry,indent=2))
(out/'source.c').write_bytes(candidate.read_bytes())
hashes={str(p.relative_to(out)):hashlib.sha256(p.read_bytes()).hexdigest() for p in out.rglob('*') if p.is_file()}
(out/'hashes.json').write_text(json.dumps(hashes,indent=2))
with (HERE/'events.jsonl').open('a') as f:
    f.write(json.dumps({'unit':unit,'label':label,'status':entry['status'],'source_sha256':hashes['source.c'],'report':str(out/'report.json')})+'\n')
print(json.dumps({k:entry.get(k) for k in ('status','exact_text_bytes','functions_exact','notes')}))
print(json.dumps([f for f in entry.get('functions',[]) if f['status'] not in ('missing','exact')]))
