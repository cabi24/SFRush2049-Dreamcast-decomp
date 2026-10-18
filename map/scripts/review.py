import json, collections
from common import *
F=json.load(open(RAW+'/functions.json'))
cov=open(RAW+'/cover.bin','rb').read()
fs=sorted(F.values(), key=lambda f:f['start'])
ov=0
for a,b in zip(fs,fs[1:]):
    if a['end']>b['start']:
        ov+=1
        if ov<15: print('overlap',h(a['start']),h(a['end']),a['evidence'],h(b['start']),b['evidence'])
print('overlaps',ov)
runs=[];i=0
while i<SIZE:
    j=i
    while j<SIZE and (cov[j]!=0)==(cov[i]!=0): j+=1
    runs.append((BASE+i,BASE+j,cov[i]!=0)); i=j
n=0;tot=0
for s,e,c in runs:
    if c or e-s<16: continue
    b=DATA[s-BASE:e-BASE]
    if all(x==0 for x in b) or all(w16(a)==9 for a in range(s,e-1,2)): continue
    if 0x8C0A77B2<=s<0x8C0D2480 or s>=0x8C11BB0E: continue
    n+=1; tot+=e-s
    print(h(s),h(e),hex(e-s), b[:16].hex())
print(n,tot)
c=collections.Counter(tuple(f['evidence']) for f in fs); print(c, len(fs))
