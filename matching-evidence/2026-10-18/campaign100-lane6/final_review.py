import os,sys,pathlib,json,hashlib,subprocess
sys.path.insert(0,os.path.abspath('tools'));import dc_build as B
p=pathlib.Path('matching-evidence/2026-10-18/campaign100-lane6')
names=['zip','zoid','dc_unk_8C069E34','dc_sound_8C069F78','dc_audio64_init_8C06ADD4','dc_blit_8C06B2EC','dc_misc_8C06B80C','dc_misc_8C06C08C'];changed={'zoid','dc_audio64_init_8C06ADD4','dc_misc_8C06C08C'}
rows=[]
for name in names:
 label='final' if name in changed else 'baseline'
 source=pathlib.Path('src',name+'.c');sha=hashlib.sha256(source.read_bytes()).hexdigest()
 e=json.loads((p/name/label/'result.json').read_text());assert e['source_sha256']==sha,(name,'source stale')
 root=pathlib.Path('build/campaign100-lane6',name,label);pp=root/'pp'/(name+'.c')
 assert pp.read_text(encoding='latin-1')==B.expand_includes(str(source.resolve()))+'\n',(name,'headers stale')
 if name in changed:
  subprocess.run([sys.executable,'-B',str(p/'check.py'),name,'final'],check=True,stdout=subprocess.DEVNULL)
  r=json.loads((p/name/'final/review.json').read_text());assert r['old_exact_spans_preserved'],r
 else:r={'unit':name,'source_sha256':sha,'restored_exactly':True,'delta_functions':0,'delta_text':0,'delta_data':0}
 r['compiler_input_matches_current_headers']=True
 r['artifact_hashes']={str(f.relative_to(root)):hashlib.sha256(f.read_bytes()).hexdigest() for f in [pp,root/'obj'/(name+'.obj'),root/'obj'/(name+'.elf'),root/'link'/(name+'.abs')]}
 rows.append(r)
m=B.C.load_map(None)
out={'assignments_addressed':16,'distinct_target_inventory_blockers':1,'compiler':B.FROZEN_VERSION,'flags':B.FROZEN_ARGS,'target_sha256':B.C.TARGET_SHA256,'map_sha256':m['sha256'],'rows':rows,'delta_functions':sum(r['delta_functions'] for r in rows),'delta_text':sum(r['delta_text'] for r in rows),'delta_data':sum(r['delta_data'] for r in rows)}
(p/'final-review.json').write_text(json.dumps(out,indent=2))
print(json.dumps({k:v for k,v in out.items() if k!='rows'},indent=2))
print('\n'.join('%s %s %+d funcs %+d text'%(r['unit'],r['source_sha256'],r['delta_functions'],r['delta_text']) for r in rows))
