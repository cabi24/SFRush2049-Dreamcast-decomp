from pathlib import Path
import struct,sys,json
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools'))
from dc_target import target_path
TARGET=target_path(ROOT)
B=TARGET.read_bytes()
BASE=0x8c010000
def u16(o):return struct.unpack_from('<H',B,o)[0]
def u32(o):return struct.unpack_from('<I',B,o)[0]
def s8(v):return v-256 if v&128 else v
def sext(v,b):return v-(1<<b) if v&(1<<(b-1)) else v
def dec(o):
 w=u16(o); n=(w>>8)&15;m=(w>>4)&15;d=w&15;p=BASE+o
 if w==9:return 'nop'
 if w==0xb:return 'rts'
 if w==0x2a:return 'sts pr,r0'
 if w==0x3a:return 'stc sgr,r0'
 if w&0xf0ff==0x002a:return f'sts pr,r{n}'
 if w&0xf0ff==0x4022:return f'sts.l pr,@-r{n}'
 if w&0xf0ff==0x4026:return f'lds.l @r{n}+,pr'
 if w&0xf0ff==0x400b:return f'jsr @r{n}'
 if w&0xf0ff==0x402b:return f'jmp @r{n}'
 if w&0xf0ff==0x4000:return f'shll r{n}'
 if w&0xf0ff==0x4001:return f'shlr r{n}'
 if w&0xf0ff==0x4008:return f'shll2 r{n}'
 if w&0xf0ff==0x4009:return f'shlr2 r{n}'
 if w&0xf0ff==0x4018:return f'shll8 r{n}'
 if w&0xf0ff==0x4019:return f'shlr8 r{n}'
 if w&0xf0ff==0x4028:return f'shll16 r{n}'
 if w&0xf0ff==0x4029:return f'shlr16 r{n}'
 if w&0xf0ff==0x4010:return f'dt r{n}'
 if w&0xf000==0xd000:
  a=((p+4)&~3)+4*(w&255);off=a-BASE
  return f'mov.l @(0x{a:08x},pc),r{n} ; 0x{u32(off):08x}' if 0<=off<len(B)-3 else 'badpc'
 if w&0xf000==0x9000:
  a=p+4+2*(w&255);return f'mov.w @(0x{a:08x},pc),r{n} ; 0x{u16(a-BASE):04x}'
 if w&0xf000==0xe000:return f'mov #{s8(w&255)},r{n}'
 if w&0xf000==0x7000:return f'add #{s8(w&255)},r{n}'
 if w&0xf000==0xa000:return f'bra 0x{p+4+2*sext(w&4095,12):08x}'
 if w&0xf000==0xb000:return f'bsr 0x{p+4+2*sext(w&4095,12):08x}'
 if w&0xff00 in (0x8900,0x8b00,0x8d00,0x8f00):
  op={0x89:'bt',0x8b:'bf',0x8d:'bt/s',0x8f:'bf/s'}[w>>8];return f'{op} 0x{p+4+2*s8(w&255):08x}'
 if w&0xf000==0x1000:return f'mov.l r{m},@({4*d},r{n})'
 if w&0xf000==0x5000:return f'mov.l @({4*d},r{m}),r{n}'
 if w&0xf000==0x6000:
  if d in (0,1,2):return f'mov.{"bwl"[d]} @r{m},r{n}'
  if d in (4,5,6):return f'mov.{"bwl"[d-4]} @r{m}+,r{n}'
  if d==3:return f'mov r{m},r{n}'
  if d in (0xc,0xd,0xe,0xf):return f'{ {12:"extu.b",13:"extu.w",14:"exts.b",15:"exts.w"}[d]} r{m},r{n}'
  if d==7:return f'not r{m},r{n}'
  if d==0xb:return f'neg r{m},r{n}'
 if w&0xf000==0x2000:
  if d in (0,1,2):return f'mov.{"bwl"[d]} r{m},@r{n}'
  if d in (4,5,6):return f'mov.{"bwl"[d-4]} r{m},@-r{n}'
  if d in (8,9,10,11):return f'{ {8:"tst",9:"and",10:"xor",11:"or"}[d]} r{m},r{n}'
  if d in (0xe,0xf):return f'mulu.w r{m},r{n}'
 if w&0xf000==0x3000:
  if d in (0,2,3,6,7,8,0xc):return f'{ {0:"cmp/eq",2:"cmp/hs",3:"cmp/ge",6:"cmp/hi",7:"cmp/gt",8:"sub",12:"add"}[d]} r{m},r{n}'
 if w&0xf000==0xf000:
  if d in range(6):return f'{["fadd","fsub","fmul","fdiv","fcmp/eq","fcmp/gt"][d]} fr{m},fr{n}'
  if d==6:return f'fmov.s @(r0,r{m}),fr{n}'
  if d==7:return f'fmov.s fr{m},@(r0,r{n})'
  if d==8:return f'fmov.s @r{m},fr{n}'
  if d==9:return f'fmov.s @r{m}+,fr{n}'
  if d==0xa:return f'fmov.s fr{m},@r{n}'
  if d==0xb:return f'fmov.s fr{m},@-r{n}'
  if d==0xc:return f'fmov fr{m},fr{n}'
  if d==0xe:return f'fmac fr0,fr{m},fr{n}'
  if w&0xf0ff==0xf02d:return f'float fpul,fr{n}'
  if w&0xf0ff==0xf03d:return f'ftrc fr{n},fpul'
  if w&0xf0ff==0xf04d:return f'fneg fr{n}'
  if w&0xf0ff==0xf05d:return f'fabs fr{n}'
  if w&0xf0ff==0xf06d:return f'fsqrt fr{n}'
 if w&0xf0ff==0x405a:return f'lds r{n},fpul'
 if w&0xf0ff==0x005a:return f'sts fpul,r{n}'
 if w&0xf00f==0x0007:return f'mul.l r{m},r{n}'
 if w&0xf0ff==0x001a:return f'sts macl,r{n}'
 if w&0xf00f==0x000c:return f'mov.b @(r0,r{m}),r{n}'
 if w&0xf00f==0x000d:return f'mov.w @(r0,r{m}),r{n}'
 if w&0xf00f==0x000e:return f'mov.l @(r0,r{m}),r{n}'
 if w&0xf00f==0x0004:return f'mov.b r{m},@(r0,r{n})'
 if w&0xf00f==0x0005:return f'mov.w r{m},@(r0,r{n})'
 if w&0xf00f==0x0006:return f'mov.l r{m},@(r0,r{n})'
 return f'.word 0x{w:04x}'
def dump(a,b):
 for o in range(a,b,2):print(f'{BASE+o:08x} {u16(o):04x} {dec(o)}')
def scan():
 hits=[]
 for o in range(2,0x95000,2):
  if u16(o)!=0xb:continue
  for ln in [80,100,120,140,160,180,200]:
   start=o-ln
   ws=[u16(x) for x in range(max(0,start),o,2)]
   ct={i:sum((w&0xf00f)==0xf000+i for w in ws) for i in [0,1,2,0xe]}
   if ct[2]>=9 and ct[0]>=6 and ct[1]==0:
    hits.append({'rts':hex(o),'window':ln,'counts':ct});break
 print(json.dumps(hits,indent=2))
if __name__=='__main__':
 if len(sys.argv)>1 and sys.argv[1]=='scan':scan()
 elif len(sys.argv)>2:dump(int(sys.argv[1],0),int(sys.argv[2],0))
