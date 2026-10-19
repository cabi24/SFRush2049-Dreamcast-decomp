import sys,json,pathlib,hashlib
sys.path.insert(0,'tools');import dc_build as B
p=pathlib.Path('matching-evidence/2026-10-18/campaign100-lane5');units=['stree','stunt','unk_8C055F38','targetpaths','targets','TB_text','videoscr','visuals'];rows=[]
def spans(r):
 out=set()
 for x in r['functions']:
  if x['status']=='exact':out.update(range(int(x['start'],16),int(x['end'],16)))
 for x in r['segments']:
  if x.get('exact') is True and x['kind'] in ['pool','data']:out.update(range(int(x['start'],16),int(x['end'],16)))
 return out
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
for u in units:
 q=p/u;a=json.loads((q/'baseline.json').read_text());label='final' if (q/'final.json').exists() else 'baseline';b=json.loads((q/(label+'.json')).read_text());old=spans(a);new=spans(b);assert old<=new,(u,'lost exact ranges',len(old-new));out=pathlib.Path('build/campaign100-lane5')/u/label
 _,im=B.built_image(str(out/'link'/(u+'.abs')));assert all(im[ad]==B.C.target_bytes(ad,ad+1)[0] for ad in new);source=pathlib.Path('src')/(u+'.c');assert sha(source)==b['source_sha256'],u
 row={'unit':u,'label':label,'delta_functions':b['functions_exact']-a['functions_exact'],'delta_text_bytes':b['exact_text_bytes']-a['exact_text_bytes'],'delta_data_bytes':b['exact_data_bytes']-a['exact_data_bytes'],'prior_exact_ranges_preserved':True,'source_sha256':sha(source),'expanded_source_sha256':sha(out/'pp'/(u+'.c')),'object_sha256':sha(out/'obj'/(u+'.obj')),'abs_sha256':sha(out/'link'/(u+'.abs')),'new_functions':[x['name'] for x in b['functions'] if x['status']=='exact' and not any(z['name']==x['name'] and z['status']=='exact' for z in a['functions'])]};rows.append(row);print(u,row['delta_functions'],row['delta_text_bytes'],'preserved')
(p/'receipt.json').write_text(json.dumps({'ledger_rows':16,'distinct_targets':15,'inventory_blocker':'unk_8C055F38 contains one function, its only56-byte external pool already exact. Awaiting parent replacement.','delta_functions':sum(x['delta_functions'] for x in rows),'delta_text_bytes':sum(x['delta_text_bytes'] for x in rows),'comparison_masks':False,'units':rows},indent=2))
