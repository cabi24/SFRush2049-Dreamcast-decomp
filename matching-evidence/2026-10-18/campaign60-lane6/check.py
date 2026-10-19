import sys,os,json,pathlib,hashlib
sys.path.insert(0,os.path.abspath('tools'));import dc_build as B
p=pathlib.Path('matching-evidence/2026-10-18/campaign60-lane6')
unit,label=sys.argv[1:3]
base=json.loads((p/unit/'baseline/result.json').read_text());now=json.loads((p/unit/label/'result.json').read_text())
_,image=B.built_image('build/campaign60-lane6/'+unit+'/'+label+'/link/'+unit+'.abs')
prior=json.loads(pathlib.Path('matching-evidence/2026-10-18',unit+'.json').read_text())
failed=[]
for a,b in prior['text_spans']+prior['data_spans']:
 a,b=int(a,16),int(b,16);w=B.C.target_bytes(a,b)
 if any(image.get(a+i)!=v for i,v in enumerate(w)):failed.append([hex(a),hex(b)])
rec={'unit':unit,'label':label,'source_sha256':now['source_sha256'],'old_exact_spans_preserved':not failed,'failed_spans':failed,'delta_functions':now['functions_exact']-base['functions_exact'],'delta_text':now['exact_text_bytes']-base['exact_text_bytes'],'delta_data':now['exact_data_bytes']-base['exact_data_bytes'],'new_exact_functions':[f for f in now['functions'] if f['status']=='exact' and next(x for x in base['functions'] if x['name']==f['name'])['status']!='exact'],'hashes':{}}
for kind,path in [('object','obj/'+unit+'.obj'),('abs','link/'+unit+'.abs')]:
 rec['hashes'][kind]=hashlib.sha256(pathlib.Path('build/campaign60-lane6',unit,label,path).read_bytes()).hexdigest()
(p/unit/label/'review.json').write_text(json.dumps(rec,indent=2));print(json.dumps(rec,indent=2))
