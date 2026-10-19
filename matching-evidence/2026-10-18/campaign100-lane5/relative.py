import sys,pathlib
sys.path[:0]=['tools','pilot/tools']
import dc_build as B,sh4
u,label,lo,hi,shift=sys.argv[1:];lo=int(lo,16);hi=int(hi,16);shift=int(shift)
_,im=B.built_image('build/campaign100-lane5/'+u+'/'+label+'/link/'+u+'.abs')
for pc in range(lo,hi,2):
 a=B.C.target_bytes(pc,pc+2);q=pc+shift;b=bytes(im.get(q+k,0) for k in range(2));ta=sh4.decode(int.from_bytes(a,'little'),pc).text;tb=sh4.decode(int.from_bytes(b,'little'),q).text
 if a!=b:print('%08X %-32s | %08X %s'%(pc,ta,q,tb))
