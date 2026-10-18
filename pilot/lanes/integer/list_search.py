from minimal_decode import B,BASE
import struct
W=struct.unpack('<'+'H'*(len(B)//2),B[:len(B)//2*2])
for i,w in enumerate(W[:0x90000//2]):
    if w&0xf0ff==0xe000:
        m=(w>>8)&15
        seq=W[i+1:i+10]
        if all(0x1400|m<<4|d in seq for d in (1,2,3)):
            print(hex(i*2),[hex(q) for q in W[i:i+10]])
