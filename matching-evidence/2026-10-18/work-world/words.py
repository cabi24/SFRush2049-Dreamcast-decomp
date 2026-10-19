import sys,os
sys.path.insert(0,os.path.abspath('tools'))
import dc_build as B
s,img=B.built_image('matching-evidence/2026-10-18/work-world/baseline/link/world.abs')
a=0x8c065c78;b=0x8c065db0
w=B.C.target_bytes(a,b)
for i in range(0,len(w),2):
 x=int.from_bytes(w[i:i+2],'little'); y=img[a+i]+256*img[a+i+1]
 print('%08X %04X %04X %s'%(a+i,x,y,'*' if x!=y else ''))
