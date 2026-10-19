import sys,os,json
sys.path.insert(0,os.path.abspath('tools'));import dc_build as B
unit,label,name=sys.argv[1:4]
u=next(u for u in B.C.load_map(None)['units'] if u['name']==unit);f=next(f for f in u['functions'] if f['name']==name)
_,image=B.built_image('build/campaign100-lane6/'+unit+'/'+label+'/link/'+unit+'.abs')
start=f['start'];want=B.C.target_bytes(start,f['end']);hits={}
for n in [4,6,8]:
 hits[n]=['%08X'%a for a in range(start-128,start+128,2) if bytes(image.get(a+i,0) for i in range(n))==want[:n]]
print(name,json.dumps(hits))
