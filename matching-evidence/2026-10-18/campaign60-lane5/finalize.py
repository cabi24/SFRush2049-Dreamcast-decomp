import sys,json,pathlib,hashlib
sys.path.insert(0,'tools');import dc_build as B
p=pathlib.Path('matching-evidence/2026-10-18/campaign60-lane5');rows=[]
units=['stree','stunt','unk_8C055F38','targetpaths','targets','TB_text','videoscr','visuals','waves','unk_8C065F2C']
def spans(r):
 out=set()
 for x in r['functions']:
  if x['status']=='exact':out.update(range(int(x['start'],16),int(x['end'],16)))
 for x in r['segments']:
  if x.get('exact') is True and x['kind'] in ['pool','data']:out.update(range(int(x['start'],16),int(x['end'],16)))
 return out
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
for u in units:
 q=p/u;a=json.loads((q/'baseline.json').read_text());label='final' if (q/'final.json').exists() else 'baseline';b=json.loads((q/(label+'.json')).read_text())
 old=spans(a);new=spans(b);assert old<=new,(u,'lost exact ranges',len(old-new))
 out=pathlib.Path('build/campaign60-lane5')/u/label
 _,im=B.built_image(str(out/'link'/(u+'.abs')));_,runs=B.read_abs(str(out/'link'/(u+'.abs')));produced={addr+i for sec,addr,blob in runs for i in range(len(blob))};assert all(im[ad]==B.C.target_bytes(ad,ad+1)[0] for ad in new);assert old&produced<=new&produced
 source=pathlib.Path('src')/(u+'.c');assert sha(source)==b['source_sha256'],u
 row={'unit':u,'label':label,'delta_functions':b['functions_exact']-a['functions_exact'],'delta_text_bytes':b['exact_text_bytes']-a['exact_text_bytes'],'delta_data_bytes':b['exact_data_bytes']-a['exact_data_bytes'],'prior_exact_ranges_preserved':True,'source_sha256':sha(source),'abs_sha256':sha(out/'link'/(u+'.abs')),'object_sha256':sha(out/'obj'/(u+'.obj')),'expanded_source_sha256':sha(out/'pp'/(u+'.c')),'new_functions':[x['name'] for x in b['functions'] if x['status']=='exact' and not any(z['name']==x['name'] and z['status']=='exact' for z in a['functions'])]}
 rows.append(row);print(u,row['delta_functions'],row['delta_text_bytes'],'preserved')
(p/'receipt.json').write_text(json.dumps({'all_ten_addressed':True,'delta_functions':sum(x['delta_functions'] for x in rows),'delta_text_bytes':sum(x['delta_text_bytes'] for x in rows),'comparison_masks':False,'raw_fixed_address_validation':True,'units':rows},indent=2))
notes={
'stree':'Split outside assignment/test fixes earlier literal pool positions. Three trials; downleaf increment shape discarded. Four new exact functions; earlier FVecInterp/pool/data retained.',
'stunt':'Scoped absW initialization to else branch; fixes _CheckAirTime pool. Direct global form regressed old matches and discarded. _CheckRoll remains65 differing bytes; existing6 exact functions retained.',
'unk_8C055F38':'Three failed shapes: register parameter, inline modff/fabs, swapped stack declaration order. Restored. Baseline155 differing bytes; unchanged56 literal credit. Emission size includes56 pool bytes so claimed58 extra body bytes in initial progress was overly simplistic.',
'targetpaths':'Ordered acceleration multiply statements reduced placement difference but no exact gain. Restored. First unmatched PlacePathTargetPos still495 differing bytes, shifted downstream functions.',
'targets':'Hoisting SetupTarget name assignment unchanged. Restored. initTargetStructs differs only in26 pool displacement bytes; SetupTarget first block schedules pool4 bytes early; broad register/stack differences remain.',
'TB_text':'Three destination-lifetime shapes. Declaring saved source before destination pointer fixes Strcat and upstream/downstream pools. +Strlen58, Strcat228, Sprintf50. Prior10 exact functions and329 data bytes retained.',
'videoscr':'Signed direction local yields unchanged code. Restored. Broad updateVideoScreen function-pointer caching/register and early size drift block moved validOption; no isolated safe win.',
'visuals':'Explicit next=i+1 local preserves separate target index calculations and fixes InitEnvs plus adjacent112 pool bytes. Removing register index had no effect. All earlier44 literal and600 data bytes preserved.',
'waves':'Three busy-wait shapes (direct global, assigned local, pointer local) leave9/7/6 differing bytes in InitWaves; unaccepted/restored. All differences are register selection near entry/calls; old20 functions retained.',
'unk_8C065F2C':'Walking part-mask pointers, matching target pointer increments, eliminates redundant zero no-clip OR test. Fixes following fog function and literal pool (+346). First visibility function remains181 differing bytes; old258 text and12223 data bytes preserved.'}
lines=['# Campaign 60 lane 5 - complete','', '| Unit | Status | Function delta | Text delta | Findings |','|---|---|---:|---:|---|']
for x in rows:lines.append('| '+x['unit']+' | complete | +'+str(x['delta_functions'])+' | +'+str(x['delta_text_bytes'])+' | '+notes[x['unit']]+' |')
lines+=['','Total: +10 exact functions and +1674 text/literal bytes, no initialized-data delta. All previous exact function/pool/data address sets preserved.','', 'Validation: python matching-evidence/2026-10-18/campaign60-lane5/run.py UNIT LABEL; final native links for retained5 units. python matching-evidence/2026-10-18/campaign60-lane5/finalize.py validates hashes, old exact-span subsets and every resulting exact byte against original target. Receipt stores source/object/absolute/preprocessed hashes.','', 'Only src/stree.c, src/stunt.c, src/TB_text.c, src/visuals.c and src/unk_8C065F2C.c retain changes. Other5 restored byte-for-byte. Private build outputs are under build/campaign60-lane5/. No shared reports, headers, maps, tools, receipts or canonical objdiff modified. No masks, original-byte injection, fill/BSS credit, full builds, installs or commits.']
(p/'progress.md').write_text('\n'.join(lines)+'\n')
