from pathlib import Path
import struct, sys

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools'))
from dc_target import target_path
B=target_path(ROOT).read_bytes()
BASE=0x8c010000

def sx(x,n): return x-(1<<n) if x&(1<<(n-1)) else x
def dec(w,pc):
    n=(w>>8)&15;m=(w>>4)&15;d=w&15;t=w>>12
    if t==0xe:return f'mov #{sx(w&255,8)},r{n}'
    if t==7:return f'add #{sx(w&255,8)},r{n}'
    if t==9:
        a=pc+4+2*(w&255);v=struct.unpack_from('<h',B,a-BASE)[0]
        return f'mov.w @(0x{a:08x},pc),r{n} ; {v} (0x{v&65535:04x})'
    if t==0xd:
        a=((pc+4)&~3)+4*(w&255);v=struct.unpack_from('<I',B,a-BASE)[0]
        return f'mov.l @(0x{a:08x},pc),r{n} ; 0x{v:08x}'
    if t in (0xa,0xb):return f'{"bra" if t==10 else "bsr"} 0x{pc+4+2*sx(w&4095,12):08x}'
    if w&0xff00 in (0x8900,0x8b00,0x8d00,0x8f00):
        return f'{ {0x89:"bt",0x8b:"bf",0x8d:"bt/s",0x8f:"bf/s"}[w>>8]} 0x{pc+4+2*sx(w&255,8):08x}'
    if t==1:return f'mov.l r{m},@({d*4},r{n})'
    if t==5:return f'mov.l @({d*4},r{m}),r{n}'
    if t==6:
        if d<3:return f'mov.{"bwl"[d]} @r{m},r{n}'
        if d==3:return f'mov r{m},r{n}'
        if d in (4,5,6):return f'mov.{"bwl"[d-4]} @r{m}+,r{n}'
        return f'{ {7:"not",8:"swap.b",9:"swap.w",10:"negc",11:"neg",12:"extu.b",13:"extu.w",14:"exts.b",15:"exts.w"}[d]} r{m},r{n}'
    if t==2:
        if d<3:return f'mov.{"bwl"[d]} r{m},@r{n}'
        if d in (4,5,6):return f'mov.{"bwl"[d-4]} r{m},@-r{n}'
        return f'{ {7:"div0s",8:"tst",9:"and",10:"xor",11:"or",12:"cmp/str",13:"xtrct",14:"mulu.w",15:"muls.w"}.get(d,"???")} r{m},r{n}'
    if t==3:return f'{ {0:"cmp/eq",2:"cmp/hs",3:"cmp/ge",4:"div1",5:"dmulu.l",6:"cmp/hi",7:"cmp/gt",8:"sub",10:"subc",11:"subv",12:"add",13:"dmuls.l",14:"addc",15:"addv"}.get(d,"???")} r{m},r{n}'
    if t==0:
        if d==7:return f'mul.l r{m},r{n}'
        if d in (4,5,6):return f'mov.{"bwl"[d-4]} r{m},@(r0,r{n})'
        if d in (12,13,14):return f'mov.{"bwl"[d-12]} @(r0,r{m}),r{n}'
    one={0x0009:'nop',0x000b:'rts',0x0008:'clrt',0x0018:'sett',0x0019:'div0u'}
    if w in one:return one[w]
    xx={0x402b:'jmp @',0x400b:'jsr @',0x4010:'dt ',0x4011:'cmp/pz ',0x4015:'cmp/pl ',0x4000:'shll ',0x4001:'shlr ',0x4004:'rotl ',0x4005:'rotr ',0x4020:'shal ',0x4021:'shar ',0x4024:'rotcl ',0x4025:'rotcr ',0x4008:'shll2 ',0x4009:'shlr2 ',0x4018:'shll8 ',0x4019:'shlr8 ',0x4028:'shll16 ',0x4029:'shlr16 '}
    if w&0xf0ff in xx:return xx[w&0xf0ff]+f'r{n}'
    sysreg={0x0002:'stc sr,',0x001a:'sts macl,',0x000a:'sts mach,',0x002a:'sts pr,',0x0029:'movt ',0x400a:'lds ',0x401a:'lds ',0x402a:'lds ',0x4006:'lds.l ',0x4016:'lds.l ',0x4026:'lds.l ',0x4002:'sts.l ',0x4012:'sts.l ',0x4022:'sts.l '}
    if w&0xf0ff in sysreg:return sysreg[w&0xf0ff]+f'r{n}'
    if w&0xff00==0x8800:return f'cmp/eq #{sx(w&255,8)},r0'
    return '.word 0x%04x'%w

def dump(start,end):
    for off in range(start,end,2):
        w=struct.unpack_from('<H',B,off)[0]
        print(f'{BASE+off:08x} +{off:06x} {w:04x}: {dec(w,BASE+off)}')

if __name__=='__main__':dump(int(sys.argv[1],0),int(sys.argv[2],0))
