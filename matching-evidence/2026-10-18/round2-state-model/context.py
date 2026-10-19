import sys,os,struct
sys.path.insert(0,os.path.abspath('tools'));sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_build as B,sh4
_,im=B.built_image('build/round2-state-model/model/baseline/link/model.abs')
a,b=0x8c040c9a,0x8c040d20
w=B.C.target_bytes(a,b)
for k in range(0,b-a,2):
 va=a+k;t=int.from_bytes(w[k:k+2],'little');v=sum(im.get(va+j,0)<<(8*j) for j in range(2));print('%08X %-45s | %s'%(va,sh4.decode(t,va).text,sh4.decode(v,va).text))
