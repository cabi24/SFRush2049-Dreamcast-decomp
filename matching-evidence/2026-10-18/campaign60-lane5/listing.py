import sys,pathlib,struct
sys.path[:0]=['tools','pilot/tools']
import dc_build as B,sh4
unit,label,lo,hi=sys.argv[1:];lo=int(lo,16);hi=int(hi,16)
_,im=B.built_image('build/campaign60-lane5/'+unit+'/'+label+'/link/'+unit+'.abs')
for pc in range(lo,hi,2):
 a=B.C.target_bytes(pc,pc+2);b=bytes(im.get(pc+k,0) for k in range(2))
 if '--all' in sys.argv or a!=b:print('%08X %s %-32s %s %s'%(pc,a.hex(),sh4.decode(int.from_bytes(a,'little'),pc).text,b.hex(),sh4.decode(int.from_bytes(b,'little'),pc).text))
