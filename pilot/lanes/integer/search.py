from minimal_decode import B,BASE,dec
import struct
W=struct.unpack('<'+'H'*(len(B)//2),B[:len(B)//2*2])
for i,w in enumerate(W[:0x90000//2]):
    if w&0xf0ff==0x4011:
        lo=max(0,i-100);hi=i+100
        if (w&0xff00|0x15) in W[lo:hi] and any(q&0xf0ff==0x4001 for q in W[lo:hi]) and any(q&0xf0ff==0x400b for q in W[lo:hi]):
            print(hex(i*2), 'cmp/pl', [hex(j*2) for j in range(lo,hi) if W[j]==(w&0xff00|0x15)])
