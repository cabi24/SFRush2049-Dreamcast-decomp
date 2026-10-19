import pathlib,sys
r=pathlib.Path(r'C:\rush2049-dreamcast'); sys.path[:0]=[str(r/'tools'),str(r/'pilot/tools')]
import dc_build as B,dc_common as C,sh4
p=r/'matching-evidence/2026-10-18/work-textureanim'; _,im=B.built_image(str(p/'baseline/link/textureanim.abs'))
a=0x8C05B2E4;b=0x8C05B3C6; want=C.target_bytes(a,b)
lines=[]
for x in range(a,b,2):
 w=int.from_bytes(want[x-a:x-a+2],'little');o=int.from_bytes(bytes(im[x+i] for i in range(2)),'little')
 lines.append(f'{x:08X} {o:04X} {sh4.decode(o,x)} | {w:04X} {sh4.decode(w,x)}'+(' DIFF' if w!=o else ''))
(p/'baseline/listing.txt').write_text('\n'.join(lines)); print('\n'.join(s for s in lines if 'DIFF' in s))
