from minimal_decode import B,BASE
import struct,sys
target=int(sys.argv[1],0)
print('direct bsr sites')
for off in range(0,min(len(B),0x98000),2):
    w=struct.unpack_from('<H',B,off)[0]
    if w>>12==0xb:
        d=w&4095;d=d-4096 if d&2048 else d
        if BASE+off+4+2*d==target:print(hex(BASE+off))
print('pointer words and PC-relative load sites')
for off in range(0,len(B)-3,2):
    if struct.unpack_from('<I',B,off)[0]==target:
        print('pool',hex(BASE+off),end=' refs ')
        print([hex(BASE+i) for i in range(max(0,off-1024),off,2) if B[i+1]>>4==0xd and ((BASE+i+4)&~3)+4*B[i]==BASE+off])
