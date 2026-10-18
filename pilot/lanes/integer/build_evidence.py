"""Read-only integer pilot static mapping; output is confined to this lane."""
from pathlib import Path
import hashlib, json, struct, sys
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import sh4
from dc_target import target_path
BASE=0x8c010000
TARGET=target_path(ROOT)
B=TARGET.read_bytes()

def sha(b): return hashlib.sha256(b).hexdigest()
def span(a,z,kind='code',ownership='exclusive'):
    return dict(file_start=a,file_end=z,start_hex=hex(a),end_hex=hex(z),va_start=hex(BASE+a),va_end=hex(BASE+z),size=z-a,kind=kind,ownership=ownership,sha256=sha(B[a:z]))
def refs(target):
    calls=[];pools=[]
    for off in range(0,min(len(B),0x98000),2):
        w=struct.unpack_from('<H',B,off)[0]
        if w>>12==11:
            d=w&4095;d=d-4096 if d&2048 else d
            if BASE+off+4+d*2==target:calls.append(hex(BASE+off))
    for off in range(0,len(B)-3,2):
        if struct.unpack_from('<I',B,off)[0]==target:
            loads=[hex(BASE+i) for i in range(max(0,off-1024),off,2) if B[i+1]>>4==13 and ((BASE+i+4)&~3)+4*B[i]==BASE+off]
            pools.append({'pool':hex(BASE+off),'loads':loads})
    return {'direct_bsr_candidates':calls,'literal_pointer_candidates':pools,'scope':'BSR scan through file0x98000; pointer scan whole file; data false positives possible until each site is traced.'}

sources={n:Path(r'C:\rush2049-mat3\decomp\src\GameSource\src')/n for n in ('bsearch.c','ll.c','drivetra.c')}
mapping={
 'bsearch':dict(set='training',entry=0x8c09bc04,status='identified API counterpart; different source/library implementation',source='bsearch.c:5',spans=[span(0x8bc04,0x8bc7a)],score=85,score_parts={'behavior':30,'call_neighborhood':25,'layout_abi':20,'rare_constants':0,'second_contract':10},runner_up={'name':'unlocated MAT3 custom bsearch body','score':None,'decision':'No alternative with MAT3 limit/fallback or zero-size guard located. Do not assert exhaustive absence.'},abi={'arguments':['r4 key','r5 base','r6 nmemb','r7 element_size','entry SP+0 comparator'],'result':'r0 pointer or0','callee_preserved':'r8-r14 plus saved PR/MACL','frame':'40 bytes: 7 callee-save GPRs (28), PR(4), MACL(4), local(4)'},evidence=[
 'Comparator entry arg5 loaded from post-prologue SP+40 at0x8C09BC2C and called indirectly at0x8C09BC3A; R4 key andR5 selected element.',
 'Unsigned lower index1 and upper nmemb; midpoint lower+((upper-lower)>>1); element base+(mid-1)*size; comparator sign lowers upper or raises lower.',
 'MBOX object lookup caller0x8C03D424 uses element_size72, model-active loop,16-char name buffer, comparator0x8C03D286; matches MAT3 mbox.c MBOX_FindObject_WORK call contract.',
 'FindWorld caller0x8C065DEC uses element_size20, comparator0x8C065DB0 comparing names at+4, WorldTable/count globals and stack name key; matches MAT3 world.c:FindWorld contract.'
 ],drift=[
 'MAT3 bsearch.c rejects size==0, computes an end limit, uses nmemb halves and a final fallback comparator. DC routine has none of these. Equal-key comparator traversal can differ.',
 'Name/API correspondence is strong; direct reuse of MAT3 body is not an equivalent source claim for all inputs. A distinct DC candidate is required.'
 ]),
 'InsertLLE':dict(set='training',entry=0x8c03a1c8,status='identified; full branch/side-effect sequence traced',source='ll.c:22',spans=[span(0x2a1c8,0x2a28e)],score=85,score_parts={'behavior':30,'call_neighborhood':25,'layout_abi':20,'rare_constants':0,'second_contract':10},runner_up={'name':'neighbor RemoveLLE','entry':'0x8C03A28E','score':45,'decision':'Rejected: removes/rewires nodes and decrements count; opposite side effects.'},abi={'arguments':['r4 list','r5 insert','r6 before'],'result':'void; no reliable output register contract','callee_preserved':'r14 saved at0x8C03A1CA and restored in RTS delay slot0x8C03A28C'},field_map={'list.handle':0,'list.dbl':1,'list.numElements':4,'list.head':8,'list.tail':12,'node.next':0,'node.prev':4},evidence=[
 'Null insert branches to common epilogue after preservingR14.',
 'Handle mode dereferences insert and optional before once; direct mode uses pointers unchanged.',
 'Before+double path rewires previous next or listhead, copies before.prev to insert.prev then updates before.prev.',
 'Before+single path traverses direct or handle next links; matches the original lack of earlybreak after a replacement.',
 'Append path conditionally sets prev, rewires tail next or head, assigns tail; common path writes insert.next=before and increments count.',
 'Neighbor0x8C03A1B8 stores handle/dbl then falls through to ClearLL0x8C03A1BE zeroing head/tail/count; RemoveLLE0x8C03A28E has matching inverse layout.'
 ],drift=[]),
 'interp':dict(set='training',entry=0x8c0231d2,status='identified arithmetic helper; source return width unresolved',source='drivetra.c:182',spans=[span(0x131d2,0x131e6)],dependencies=[span(0x13228,0x1322c,'literal','shared torque/interp pool; central integrator assigns owner once')],score=90,score_parts={'behavior':30,'call_neighborhood':25,'layout_abi':20,'rare_constants':5,'second_contract':10},runner_up={'name':'unlocated same-expression helper','score':None,'decision':'Exact SUB R4,R5 plus nearby MUL pattern found only at0x131D2; equivalent differently allocated helpers not exhaustively excluded.'},abi={'arguments':['r4 a','r5 b','r6 rem','r7 total'],'result':'r0 full32-bit a+signed_quotient; no explicit16-bit sign extension','preservation':'PR pushed/popped; helper preservesR2/R3 and observedR4-R7; standalone mutatesR5,R1,R3 and flags','raw_expression':'uint32 wrapping product from (b-a)*rem; signed32 division helper; wrapping add to a. Restrict candidate C fixtures to defined arithmetic until overflow/INT_MIN/-1 behavior is specified.'},evidence=[
 '0x131D2 SUB R4,R5;0x131D6 MUL.L R6,R5;0x131DA STS MACL,R1;0x131DC JSR helper; delay-slot MOV R7,R0; return delay-slot ADD R4,R0.',
 'Helper pointer shared at0x13228 ->0x8C09B680; signedDIV0S plus32 DIV1 steps implements signed32 division, R1 numerator/R0 denominator, returnR0.',
 'Three instances of the same arithmetic are inlined in enginetorque; intermediates are explicitly stored/loaded as signed16-bit.',
 'No directBSR or static literal-pointer reference to standalone interp found in bounded code/whole-file pointer scan; it appears retained out-of-line despite inline callers.'
 ],drift=[
 'MAT3 declaration returns signedshort; DC standalone has no EXTS.W and has no located out-of-line callers to establish whether source declaration was int or short. Expose raw32 and low16 separately in validation; do not silently impose the GC return type.'
 ]),
 'enginetorque':dict(set='held_out',entry=0x8c0230b0,status='identified only; held-out compilation/configuration tuning not performed',source='drivetra.c:143',spans=[span(0x130b0,0x131d2)],dependencies=[span(0x1321c,0x13220,'literal','torque constants'),span(0x13228,0x1323c,'literal','shared pool with interp; includes helper pointers and scale-table pointer')],score=98,score_parts={'behavior':30,'call_neighborhood':23,'layout_abi':20,'rare_constants':15,'second_contract':10},runner_up={'name':'neighbor engine','entry':'0x8C0231E6','score':45,'decision':'Calls this routine with rpm/throttle/curve; lacks torque table lookup/clamps, so excluded as the torque routine.'},abi={'arguments':['r4 model','r5 rpm narrowed atentry viaEXTS.W','r6 throttle narrowed by16-bit division helper','r7 signedshort torque curve'],'result':'r0 low16 meaningful; engine callerEXTS.W at0x8C023210 verifies torque return usage','callee_preserved':'r11-r14,PR;20bytes locals'},field_map={'model.tune_pointer':4,'model.handling_byte':11,'model.engine_type_byte':12,'tune.torque_scale_float':8,'carEngineTorque_row_stride':12,'torque_curve_row_stride':24},evidence=[
 'Adjacent signed16 literals1150 and1149 at0x1321C/0x1321E anchor rpm division/remainder and upper-clamp rem1149; negative-rpm index/rem both zero; clampindex11to10.',
 'Throttle division/remainder14; clampindex9to8, rem13.',
 'Signedshort curve addressing tindex*24+rindex*2, two adjacent12-short rows, and three inline interpolation steps with denominators1149,1149,14.',
 'Final scale reads model+4 tune pointer/tune+8 and table0x8C0BBC4C indexed by signedbytes model+12 times12 andmodel+11 times4; FMUL/FLOAT/FTRC sequence.',
 'Directengine calls0x8C02320C,0x8C023264,0x8C023284; caller converts rpm/throttle FPtointeger and narrows torque result to signed16.'
 ],drift=['No compilation, flag search, held-out semantic fixture tuning, or exact-byte claim was performed. FP execution mode remains unverified.'])
}

for name,row in mapping.items():
    row['entry_hex']=hex(row['entry']);row['references']=refs(row['entry'])
    s=row['spans'][0];lo=s['file_start'];hi=s['file_end']
    cfg=sh4.walk_cfg(B,row['entry'],BASE,lower=BASE+lo,upper=BASE+hi)
    row['cfg_instruction_count']=len(cfg['instructions']);row['cfg_warnings']=cfg['warnings'];row['cfg_truncated']=cfg['truncated']
    row['code_sha256']=s['sha256']
    (HERE/(name+'.cfg.json')).write_text(json.dumps(cfg,indent=2)+'\n')
    lines=[]
    for i in sh4.disassemble(B,lo,hi,BASE):
        comment=f' ; literal=0x{i.literal_value&0xffffffff:08X}' if i.literal_value is not None else ''
        lines.append(f'{i.pc:08X} {i.word:04X}  {i.text}{comment}')
    (HERE/(name+'.disasm.txt')).write_text('\n'.join(lines)+'\n')

report=dict(target=str(TARGET),target_sha256=sha(B),base=hex(BASE),scope='Static mapping only; no compiler execution or byte-match/C recovery claims.',source_hashes={str(p):sha(p.read_bytes()) for p in sources.values()},mappings=mapping,summary={'identified_api_or_semantic_counterparts':4,'equivalent_c_proven':0,'object_emission_matches':0,'exact_local_link_matches':0,'whole_image_verified':False,'code_bytes_union':sum(r['spans'][0]['size'] for r in mapping.values()),'original_object_identity':'Not measurable from flat target.'},decoder_note='Custom dependency-free decoder; known encodings/CFG reviewed against byte-level instructions. Not an established analyzer; execution and compiler corroboration still required.')
(HERE/'mapping.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'target_sha256':report['target_sha256'],'mappings':{n:dict(entry=r['entry_hex'],bytes=r['spans'][0]['size'],sha256=r['code_sha256'],cfg_instructions=r['cfg_instruction_count'],warnings=r['cfg_warnings']) for n,r in mapping.items()},'source_hashes':report['source_hashes']},indent=2))
