import sys,pathlib
root=pathlib.Path(r'C:\rush2049-dreamcast'); sys.path.insert(0,str(root/'tools'))
import dc_build as B
base=0x8c06926c; end=0x8c0692ac
_,im=B.built_image(str(pathlib.Path(__file__).parent/sys.argv[1]/'link/zoidpoly.abs'))
want=B.C.target_bytes(base,end)
for i in range(0,end-base,2):
 a=base+i; x=im.get(a,0)|(im.get(a+1,0)<<8); y=want[i]|want[i+1]<<8
 print(f'{a:08X} {x:04X} {y:04X}'+(' *' if x!=y else ''))
