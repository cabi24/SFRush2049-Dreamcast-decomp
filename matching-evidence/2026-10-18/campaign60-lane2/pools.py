import sys,os
sys.path.insert(0,os.path.abspath('tools'))
import dc_common as C
u=next(x for x in C.load_map(None)['units'] if x['name']=='circuit');a,b=u['text'];t=C.target_bytes(a,b)
for h in (0x8c09b82c,0x8c09b8f4,0x8c047be4):print(hex(h),[hex(a+i) for i in range(len(t)-3) if t[i:i+4]==h.to_bytes(4,'little')])
