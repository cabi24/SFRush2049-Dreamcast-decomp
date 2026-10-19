import sys, pathlib, struct
sys.path[:0]=['tools','pilot/tools']
import dc_build as B, sh4
root=pathlib.Path('matching-evidence/2026-10-18/round2-mbox')
_,image=B.built_image(str(root/sys.argv[1]/'link/mbox.abs'))
lo=int(sys.argv[2],16); hi=int(sys.argv[3],16)
for pc in range(lo,hi,2):
 a=B.C.target_bytes(pc,pc+2); b=bytes(image.get(pc+k,0) for k in range(2))
 ta=sh4.decode(int.from_bytes(a,'little'),pc).text; tb=sh4.decode(int.from_bytes(b,'little'),pc).text
 print('%08X %s %-36s %s %-36s%s'%(pc,a.hex(),ta,b.hex(),tb,' *' if a!=b else ''))
