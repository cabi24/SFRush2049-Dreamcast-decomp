import pathlib,json,hashlib,subprocess,sys
p=pathlib.Path('matching-evidence/2026-10-18/campaign60-lane6')
names=['zip','zoid','dc_unk_8C069E34','dc_sound_8C069F78','dc_audio64_init_8C06ADD4','dc_blit_8C06B2EC','dc_misc_8C06B80C','dc_misc_8C06C08C','paksysdc','dc_kamui_tex_8C06E5B4']
changed={'zoid','dc_unk_8C069E34','dc_audio64_init_8C06ADD4','dc_misc_8C06C08C'}
rows=[]
for name in names:
 base=json.loads((p/name/'baseline/result.json').read_text())
 sha=hashlib.sha256(pathlib.Path('src',name+'.c').read_bytes()).hexdigest()
 if name in changed:
  subprocess.run([sys.executable,'-B',str(p/'check.py'),name,'final'],check=True,stdout=subprocess.DEVNULL)
  r=json.loads((p/name/'final/review.json').read_text())
  assert r['old_exact_spans_preserved'],r
  assert r['source_sha256']==sha
 else:
  assert sha==base['source_sha256'],(name,sha,base['source_sha256'])
  r={'unit':name,'source_sha256':sha,'delta_functions':0,'delta_text':0,'delta_data':0,'restored_exactly':True}
 rows.append(r)
out={'assignments_completed':10,'rows':rows,'delta_functions':sum(r['delta_functions'] for r in rows),'delta_text':sum(r['delta_text'] for r in rows),'delta_data':sum(r['delta_data'] for r in rows),'queue_literal_pool_review_only':88,'no_global_report_written':True}
(p/'final-review.json').write_text(json.dumps(out,indent=2))
print(json.dumps({k:v for k,v in out.items() if k!='rows'},indent=2))
print('\n'.join('%s %s funcs %+d text %+d'%(r['unit'],r['source_sha256'],r['delta_functions'],r['delta_text']) for r in rows))
