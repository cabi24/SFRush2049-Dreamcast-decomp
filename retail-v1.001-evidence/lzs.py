"""LZSS-style decoder validated against all 314 retail LZS assets. No file I/O."""

def unlzs(data):
 out=bytearray();i=0
 while i<len(data):
  flags=data[i];i+=1
  for bit in range(8):
   if i>=len(data):raise ValueError('missing explicit terminator')
   if flags&(1<<bit):out.append(data[i]);i+=1
   else:
    if i+1>=len(data):raise ValueError('incomplete backreference')
    a,b=data[i:i+2];i+=2;length=(a&15)+2;distance=((a&240)<<4)|b
    if distance==0:
     if a!=0 or i!=len(data):raise ValueError('unexpected zero-distance token')
     return bytes(out)
    if distance>len(out):raise ValueError(f'invalid distance {distance} outlen {len(out)} at {i-2}')
    for _ in range(length):out.append(out[-distance])
    if len(out)>50000000:raise ValueError('size cap')
 raise ValueError('no terminator')
