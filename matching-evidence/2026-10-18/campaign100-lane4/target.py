import sys,os
sys.path.insert(0,os.path.abspath('tools'));sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_common as C
import sh4
u=next(u for u in C.load_map(None)['units'] if u['name']==sys.argv[1]);f=next(f for f in u['functions'] if f['name']==sys.argv[2])
for a in range(f['start'],min(f['end'],f['start']+int(sys.argv[3])*2),2):
 w=int.from_bytes(C.target_bytes(a,a+2),'little');i=sh4.decode(w,a);print('%08X %s'%(a,i.text))
