import sys,os,struct
sys.path.insert(0,os.path.abspath('tools'));sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_build as B,sh4
u,label=sys.argv[1:]
_,im=B.built_image('build/round2-state-model/'+u+'/'+label+'/link/'+u+'.abs')
un=next(x for x in B.C.load_map(None)['units'] if x['name']==u)
for f in un['functions']:
 a,b=f['start'],f['end'];w=B.C.target_bytes(a,b)
 for k in range(0,b-a,2):
  v=bytes(im.get(a+k+j,0) for j in range(2));t=w[k:k+2]
  if v!=t:
   for q in range(max(0,k-4),min(b-a,k+6),2):
    va=a+q;tt=w[q:q+2];vv=bytes(im.get(va+j,0) for j in range(2))
    print('%08X %s %s | %s %s'%(va,tt.hex(),sh4.decode(int.from_bytes(tt,'little'),va).text,vv.hex(),sh4.decode(int.from_bytes(vv,'little'),va).text))
