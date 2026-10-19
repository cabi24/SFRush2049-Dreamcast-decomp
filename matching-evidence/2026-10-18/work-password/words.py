import sys,pathlib
sys.path.insert(0,r'C:\rush2049-dreamcast\tools')
import dc_build as B
base=pathlib.Path(__file__).parent
_, image=B.built_image(str(base/'baseline/link/password.abs'))
a=0x8c044ca4;b=0x8c044d0c
want=B.C.target_bytes(a,b)
for k in range(0,b-a,2):
    w=int.from_bytes(want[k:k+2],'little')
    g=image[a+k]+256*image[a+k+1]
    print(f'{a+k:08X}: {w:04X} {g:04X} '+('DIFF' if w!=g else ''))
