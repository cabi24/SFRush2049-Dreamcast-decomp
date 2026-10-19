from pathlib import Path
import hashlib,json,sys,os
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
p=Path('matching-evidence/2026-10-18/round2-state-model')
rows=[]
for unit,labels in [('state',['baseline','shared-i','fog-pointers','short-loop']),('model',['baseline','reverse-product','local-scale','reverse-blend','selected','final'])]:
 for label in labels:
  b=Path('build/round2-state-model')/unit/label
  r=json.loads((b/'result.json').read_text());(p/(unit+'-'+label+'.json')).write_text(json.dumps(r,indent=2))
  rows.append({'unit':unit,'label':label,'exact_functions':r['functions_exact'],'exact_text_bytes':r['exact_text_bytes'],'diff_bytes':[(f['name'],f['diff_bytes']) for f in r['functions']],'source_sha256':r['source_sha256'],'text_size_delta':r.get('text_size_delta')})
def sha(f):return hashlib.sha256(Path(f).read_bytes()).hexdigest()
r=json.loads((p/'model-final.json').read_text());assert r['status']=='exact' and r['functions_exact']==3
_,image=B.built_image('build/round2-state-model/model/final/link/model.abs')
assert B.compare_range(image,0x8c040c24,0x8c041220)==(True,1532,0)
assert B.compare_range(image,0x8c0bee80,0x8c0bee81)==(True,1,0)
assert sha('src/state.c')==sha(p/'state.original.c')
manifest={'result':'one new exact function, +1402 emitted exact text bytes; state unchanged','trials':rows,'hashes':{str(f):sha(f) for f in [Path('src/state.c'),Path('src/model.c'),Path('build/round2-state-model/model/final/link/model.abs'),Path('build/round2-state-model/model/final/obj/model.obj') ]},'full_text_raw_equality':True,'comparison_masks':False,'canonical_outputs_modified':False}
(p/'manifest.json').write_text(json.dumps(manifest,indent=2));print(json.dumps(manifest,indent=2))
