import json,subprocess,sys,hashlib
from pathlib import Path
R=Path(r'C:\rush2049-dreamcast'); E=Path(__file__).resolve().parent
units=['timer','unk_8C0102A4_lz','unzip','asincos','audio','audiomgr','audiosfx','audioscr','gfx']
records=[]
for u in units:
 subprocess.run([sys.executable,str(E/'run.py'),u,'final'],cwd=R,check=True,stdout=subprocess.DEVNULL)
 b=json.loads((E/(u+'-baseline.json')).read_text()); f=json.loads((E/(u+'-final.json')).read_text())
 assert b['source_sha256']==f['source_sha256'],u+' source drift'
 assert b['exact_text_bytes']==f['exact_text_bytes'] and b['exact_data_bytes']==f['exact_data_bytes'],u+' credit drift'
 assert b['functions']==f['functions'] and b['segments']==f['segments'],u+' spans drift'
 hashes={}
 for p in [R/'src'/f'{u}.c',R/'build/campaign100-lane1'/u/'final/link'/f'{u}.abs',R/'build/campaign100-lane1'/u/'final/obj'/f'{u}.obj']:
  hashes[str(p.relative_to(R))]=hashlib.sha256(p.read_bytes()).hexdigest()
 records.append(dict(unit=u,text=f['exact_text_bytes'],data=f['exact_data_bytes'],exact_functions=f['functions_exact'],baseline_spans_preserved=True,sha256=hashes))
 print(u,'validated',flush=True)
(E/'final-validation.json').write_text(json.dumps(dict(new_text_bytes=0,new_data_bytes=0,new_functions=0,units=records),indent=2))
