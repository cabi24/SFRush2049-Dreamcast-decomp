import sys
from pathlib import Path
root=Path(r'C:\rush2049-dreamcast')
sys.path[:0]=[str(root/'tools'),str(root/'pilot/tools')]
import dc_build as D,sh4
base=0x8c03cf94
label=sys.argv[1] if len(sys.argv)>1 else 'baseline'
_,im=D.built_image(str(Path(__file__).resolve().parent/label/'link/mb_wad.abs'))
want=D.C.target_bytes(base,base+70)
lines=[]
for i in range(0,70,2):
 a=base+i; w=int.from_bytes(want[i:i+2],'little'); b=im.get(a,0)|(im.get(a+1,0)<<8)
 lines.append(f'{a:08X} {b:04X} {sh4.decode(b,a).text:28} {"=" if b==w else "!"} {w:04X} {sh4.decode(w,a).text}')
print('\n'.join(lines))
(Path(__file__).resolve().parent/label/'listing.txt').write_text('\n'.join(lines))
