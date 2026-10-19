from pathlib import Path
import sys,os,json,hashlib
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
P=Path('matching-evidence/2026-10-18/campaign60-lane2')
units='camera checkpoint circuit collision controlsys controlscr drivsym fmath game gameoverscr'.split()
def sha(f):return hashlib.sha256(Path(f).read_bytes()).hexdigest()
records=[]
for u in units:
 baseline=json.loads((P/(u+'-baseline.json')).read_text());label='selected' if u in ('fmath','game') else 'baseline'
 result=json.loads((P/(u+'-'+label+'.json')).read_text());assert sha('src/'+u+'.c')==result['source_sha256']
 if label=='baseline':assert sha('src/'+u+'.c')==sha(P/(u+'.original.c'))
 root=Path('build/campaign60-lane2')/u/label
 _,im=B.built_image(root/'link'/(u+'.abs'))
 for f in result['functions']:
  if f['status']=='exact':assert B.compare_range(im,int(f['start'],16),int(f['end'],16))[0]
 for f in baseline['functions']:
  if f['status']=='exact':assert B.compare_range(im,int(f['start'],16),int(f['end'],16))[0]
 for seg in baseline['segments']:
  if seg['kind']!='bss' and seg.get('exact'):assert B.compare_range(im,int(seg['start'],16),int(seg['end'],16))[0]
 records.append({'unit':u,'label':label,'function_delta':result['functions_exact']-baseline['functions_exact'],'text_delta':result['exact_text_bytes']-baseline['exact_text_bytes'],'data_delta':result['exact_data_bytes']-baseline['exact_data_bytes'],'functions_exact':result['functions_exact'],'source_sha256':sha('src/'+u+'.c'),'object_sha256':sha(root/'obj'/(u+'.obj')),'abs_sha256':sha(root/'link'/(u+'.abs')),'all_old_exact_spans_preserved':True,'all_claimed_exact_functions_raw_rechecked':True})
manifest={'assignments':10,'completed':10,'records':records,'function_delta':sum(x['function_delta'] for x in records),'text_delta':sum(x['text_delta'] for x in records),'data_delta':sum(x['data_delta'] for x in records),'comparison_masks':False,'canonical_outputs_modified':False,'frozen_flags':True,'native_only':True}
(P/'manifest.json').write_text(json.dumps(manifest,indent=2));print(json.dumps(manifest,indent=2))
