"""Bounded symbolic interpretation of the training leaf/control, not game execution.

Only integer-exact fixture values are evaluated; no claim about general SH-4 FP
rounding, exceptions, FPSCR, denormals, NaNs, or fused MAC semantics is made.
"""
from pathlib import Path
import json
import lane_sh4 as s
OUT=Path(__file__).resolve().parent

def trace(start,end,alias=False):
    src=0x1000;dst=src if alias else 0x2000;mat=0x3000
    r=[0]*16;r[4]=src;r[5]=dst;r[6]=mat;r[15]=0x8000
    f=[None]*16
    mem={src+4*i:(f'src[{i}]',i+1) for i in range(3)}
    mem.update({mat+4*i:(f'm[{i//3}][{i%3}]',i+1) for i in range(9)})
    loads=[];stores=[];ops=[]
    for pc in range(start,end,2):
        w=s.u16(pc);n=(w>>8)&15;m=(w>>4)&15;d=w&15
        if w==0xb:continue # execute explicit final delay-slot instruction next
        if w&0xf000==0xe000:r[n]=s.s8(w&255)
        elif w&0xf000==0x7000:r[n]+=s.s8(w&255)
        elif w&0xf00f==0x6003:r[n]=r[m]
        elif w&0xf00f==0x2006:
            r[n]-=4;mem[r[n]]=('saved_gpr',r[m])
        elif w&0xf00f==0x6006:
            r[n]=mem[r[m]][1];r[m]+=4
        elif w&0xf000==0xf000:
            if d in (6,8):
                a=r[m]+(r[0] if d==6 else 0);f[n]=mem[a]
                loads.append({'pc':hex(s.BASE+pc),'address':hex(a),'value':f[n][0]})
            elif d in (7,10):
                a=r[n]+(r[0] if d==7 else 0);mem[a]=f[m]
                stores.append({'pc':hex(s.BASE+pc),'destination_index':(a-dst)//4,'expression':f[m][0],'integer_exact_fixture':f[m][1]})
            elif d==12:f[n]=f[m]
            elif d==2:
                a,b=f[n],f[m];f[n]=(f'MUL({a[0]},{b[0]})',a[1]*b[1])
                ops.append({'pc':hex(s.BASE+pc),'op':'FMUL','expression':f[n][0]})
            elif d==14:
                a,b,c=f[0],f[m],f[n];f[n]=(f'MAC({a[0]},{b[0]},{c[0]})',a[1]*b[1]+c[1])
                ops.append({'pc':hex(s.BASE+pc),'op':'FMAC','expression':f[n][0]})
            else:raise ValueError((hex(pc),hex(w)))
        else:raise ValueError((hex(pc),hex(w)))
    return dict(code_start=hex(s.BASE+start),code_end_exclusive=hex(s.BASE+end),src_equals_dst=alias,loads=loads,operations=ops,stores=stores,result=[mem[dst+4*i][1] for i in range(3)])

body=trace(0x16b00,0x16b5e)
alias=trace(0x16b00,0x16b5e,True)
world=trace(0x16b8c,0x16be4)
assert body['result']==[14,32,50]
assert alias['result']==[14,84,797]
assert world['result']==[30,36,42]
assert body['result']!=world['result']
assert alias['result']!=body['result']
result={'scope':'training static trace only; no held-out validation','fixture':{'src':[1,2,3],'matrix':[[1,2,3],[4,5,6],[7,8,9]],'arithmetic_limit':'Small integer-exact products/sums; does not distinguish MAC rounding policies'},'checks':{'row_vs_transposed_negative_control':True,'aliasing_reload_negative_control':True},'BodyVector':body,'BodyVector_src_eq_dst':alias,'WorldVector_wrong_candidate':world}
(OUT/'body-trace.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'checks':result['checks'],'BodyVector':body['result'],'alias':alias['result'],'WorldVector_wrong_candidate':world['result']},indent=2))
