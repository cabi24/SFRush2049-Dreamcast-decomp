from pathlib import Path
import struct,json,hashlib,contextlib,io,sys
import lane_sh4 as sh
OUT=Path(__file__).resolve().parent
ROOT=OUT.parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import sh4
def h(b):return hashlib.sha256(b).hexdigest()
def span(a,b):return dict(file_start=f'0x{a:X}',file_end_exclusive=f'0x{b:X}',va_start=f'0x{sh.BASE+a:08X}',va_end_exclusive=f'0x{sh.BASE+b:08X}',size=b-a,sha256=h(sh.B[a:b]))
def src(n):
 p=Path('C:/rush2049-mat3/decomp/src/GameSource/src')/n
 return dict(path=str(p),sha256=h(p.read_bytes()))
def litrefs(value):
 refs=[]
 for o in range(0,0x96000,2):
  w=sh.u16(o)
  if w&0xf000!=0xd000:continue
  p=((o+4)&~3)+4*(w&255)
  if p+4<=len(sh.B) and sh.u32(p)==value:refs.append(dict(file=f'0x{o:X}',pool_file=f'0x{p:X}',register=f'r{(w>>8)&15}'))
 return refs
def emitdump(name,a,b):
 text='\n'.join(f'{sh.BASE+o:08x} {sh.u16(o):04x} {sh.dec(o)}' for o in range(a,b,2))+'\n'
 (OUT/name).write_text(text,encoding='utf-8')
emitdump('BodyVector.sh4.txt',0x16b00,0x16b5e)
emitdump('WorldVector-control.sh4.txt',0x16b8c,0x16be4)
emitdump('UpdateFrameTimer.sh4.txt',0x10a,0x176)
emitdump('timer-neighborhood.sh4.txt',0xa4,0x1c0)
emitdump('resource-finder-loader.sh4.txt',0x556de,0x55890)
emitdump('wave-mirror-control.sh4.txt',0x55c1e,0x55c78)
decoder_review={}
for name,a,b in [('BodyVector',0x16b00,0x16b5e),('UpdateFrameTimer',0x10a,0x176),('WorldVector_control',0x16b8c,0x16be4)]:
 ins=list(sh4.disassemble(sh.B,a,b))
 unknown=[hex(i.pc) for i in ins if not i.known]
 assert not unknown,(name,unknown)
 decoder_review[name]={'instructions':len(ins),'unknown':unknown,'decoded':[i.to_dict() for i in ins],'cfg':sh4.walk_cfg(sh.B,sh.BASE+a,lower=sh.BASE+a,upper=sh.BASE+b)}
(OUT/'decoder-review.json').write_text(json.dumps(decoder_review,indent=2)+'\n',encoding='utf-8')
pool=[(0x1c4,'frameCounter',0x8c12253c),(0x1d8,'lastFrameCounter',0x8c12255c),(0x1dc,'retraceTime',0x8c0b8c2c),(0x1e0,'frameSeconds',0x8c12254c),(0x1e4,'seconds',0x8c122548),(0x1e8,'timerPaused',0x8c0b8c24),(0x1ec,'lastFrameCount',0x8c122550),(0x1f0,'countdownStart',0x8c122558),(0x1f4,'countdownEnd',0x8c122554)]
body_refs=litrefs(0x8c026b00)
resource_refs=[]
for v in range(0x8c15fa20,0x8c15fa34):
 for r in litrefs(v):resource_refs.append(dict(**r,value=f'0x{v:08X}',field_offset=v-0x8c15fa20))
data={
 'schema':1,'scope':'bounded static correspondence; no target compilation, held-out tuning or exact-match claim',
 'target':{'path':str(sh.TARGET),'sha256':h(sh.B),'base':'0x8C010000','byte_order':'little','isa':'SH-4'},
 'mappings':[
  {'function':'BodyVector','set':'training','status':'I_identified','source':src('fmath.c'),'source_line':239,'code':span(0x16b00,0x16b5e),'owned_data_spans':[],
   'identity_score':{'behavior':30,'neighborhood':20,'fields':20,'uncommon_constants':0,'independent_call_contract':10,'total':80},
   'runner_up':{'function':'WorldVector','score':45,'veto':'row/column matrix addressing is transposed'},
   'abi':{'src':'r4','dst':'r5','matrix':'r6','return':'void','gpr_clobbers':['r0','r1','r2','r6','r7'],'fpr_clobbers':['fr0','fr2','fr3'],'stack_bytes':0},
   'fp_strategy':'Per row: FMUL src[2]*matrix[row][2], then FMAC src[1]*matrix[row][1]+acc, then FMAC src[0]*matrix[row][0]+acc. Opcode trace establishes order; exact FMAC rounding/FPSCR remain unresolved.',
   'alias_contract':'Source is reloaded for every row after previous dst store. Do not hoist src[0..2] or assume restrict. Final dst[2] store occupies RTS delay slot at file0x16B5C.',
   'calls':[],'literal_load_xrefs':body_refs,
   'confirmed_call_examples':[{'jsr_file':'0x14436','target_load_file':'0x14432','arg_setup':'r4=model+0x2D4, r5=model+0x208, r6=model+0x2EC'}],
   'negative_control':{'name':'WorldVector','code':span(0x16b8c,0x16be4),'difference':'Reads matrix columns across offsets0,12,24; BodyVector reads consecutive row fields0,4,8 then rows+12,+24.'},
   'static_training_trace':{'file':'body-trace.json','normal_result':[14,32,50],'src_eq_dst_result':[14,84,797],'WorldVector_wrong_candidate_result':[30,36,42],'limit':'integer-exact static fixture does not resolve general FP semantics'},
   'decoder_review':'decoder-review.json',
   'acceptance_limits':['No equivalent-C completion claim until FP policy is established.','No emitted object or local linked bytes exist.','Compiler family/version not identified.']},
  {'function':'UpdateFrameTimer','set':'held_out','status':'I_identified_no_compilation_or_tuning','source':src('timer.c'),'source_line':45,'code':span(0x10a,0x176),
   'identity_score':{'behavior':30,'neighborhood':25,'fields':20,'uncommon_constants':15,'independent_call_contract':0,'total':90},
   'shared_pool_dependencies':[dict(role=role,span=span(o,o+4),value=f'0x{v:08X}',ownership='shared_timer_pool_not_exclusive') for o,role,v in pool],
   'calls':[],'abi':{'arguments':[],'return':'void','gpr_clobbers':['r0','r1','r2','r3','r4','r5'],'fpr_clobbers':['fr2','fr3'],'stack_bytes':0},
   'behavior':['frameCounter-lastFrameCounter stored before lastFrameCounter reload/update','paused: add lastFrameCount to both countdownStart and countdownEnd','running: signed conversion of frameCounter-countdownStart times retraceTime stored seconds','signed >5 clamp on lastFrameCount','signed FLOAT lastFrameCount times retraceTime stored frameSeconds in RTS delay slot'],
   'drift':[{'kind':'platform_clock','evidence':'DC counter0x8C12253C incremented by callback0x8C0100A4, registered by InitializeTimer0x8C0100AE at call0x8C0100D0; MAT3 instead uses sc.frameCount.','consequence':'Use a DC counter declaration; do not import GC sc structure or scheduler wrapper.'}],
   'neighbor_functions':{'InitializeTimer':'0x8C0100AE','PauseTimer':'0x8C0100FA','ResumeTimer':'0x8C010102','SetCountdownTimer':'0x8C010176','AddCountdownTime':'0x8C010194','GetCountdownTimeRemaining':'0x8C0101A8','CountdownTimeOut':'0x8C0101FC'},
   'acceptance_limits':['Shared timer literal pool cannot be counted as exclusively owned by this function.','No held-out compilation, numerical validation or configuration tuning performed.']},
  {'function':'GetWaveByIndex','set':'held_out','status':'not_found_possible_GC_addition','source':src('waves.c'),'source_line':325,'code':None,'identity_score':None,
   'verified_DC_layout':{'base':'0x8C15FA20','stride':20,'count':64,'wave_offset':6,'wave_type':'unsigned16','ptr_offset':12,'loaded_offset':0,'ready_offset':1,'model_offset':3,'handle_offset':5},
   'anchors':{'finder':'0x8C0656E2','loader':'0x8C06575C','filename_table':'0x8C0C44C0','setup_dispatch':'0x8C065862'},
   'search':'All aligned MOV.L PC-relative loads within file0..0x96000 whose literal equals wave-instance base through +19 were examined. This is bounded evidence, not proof that no getter/inlined equivalent exists anywhere.',
   'base_field_literal_xrefs':resource_refs,
   'only_base_plus_6_literal':{'pool':'0x55908','load':'0x55868','consumer':'0x8C065862 reads wave index, chooses texture mode, tail-branches setup0x8C06588C; does not return wave value'},
   'MAT3_caller_evidence':{'only_source_caller':'C:/rush2049-mat3/decomp/src/NGC/ngcZoid.c:922','context':'GC renderer uses wave>=0xE9 to discriminate track drawing'},
   'interpretation':'Likely helper introduced for GC backend; not established. Keep all seven pilot slots and mark this slot unscored/absent-pending rather than substituting a wave-field access or counting a match.'}
 ],
 'not_performed':['held-out compilation or option tuning','compiler acquisition/install','game execution','MAT3 writes','canonical source/configuration edits','emitted/local exact-match claims']}
(OUT/'mapping.json').write_text(json.dumps(data,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'written':'mapping.json','mappings':len(data['mappings']),'BodyVector_literal_load_refs':len(body_refs),'resource_field_literal_refs':len(resource_refs)},indent=2))
