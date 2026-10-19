import sys,os,struct
sys.path[:0]=[os.path.abspath('tools'),os.path.abspath('pilot/tools')]
from sysrof_abs import read_abs
from sh4 import decode
unit,label,low,high=sys.argv[1:]
lo,hi=int(low,16),int(high,16)
target=open('pilot/orig/1ST_READ.BIN','rb').read()
sections,runs=read_abs('build/campaign100-lane3/'+unit+'/'+label+'/link/'+unit+'.abs')
built={a+i:b for _,a,data in runs for i,b in enumerate(data)}
for a in range(lo,hi,2):
 t=int.from_bytes(target[a-0x8c010000:a-0x8c010000+2],'little');b=built.get(a,0)|(built.get(a+1,0)<<8)
 print('%08X %s %04X %-42s | %04X %s'%(a,' ' if t==b else '*',t,decode(t,a,target).text,b,decode(b,a).text))
