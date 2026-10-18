"""Validate lane evidence and regenerate the conservative pilot manifest.
Reads the pinned image/MAT3 references; writes only this pilot's analysis files.
No compiler, target instruction execution, download, or source promotion.
"""
from pathlib import Path
import hashlib,json,sys
from datetime import datetime,timezone
from compare import validate_spans,Artifact,compare_bytes,BASE,TARGET_SHA
from sh4 import disassemble
from dc_target import target_path

ROOT=Path(__file__).resolve().parent.parent
def sha(data):return hashlib.sha256(data).hexdigest()
def read(path):return json.loads(path.read_text(encoding='utf-8-sig'))
def save(name,value):
 p=ROOT/'analysis'/name;p.parent.mkdir(exist_ok=True);p.write_text(json.dumps(value,indent=2)+'\n',encoding='utf-8')
def main():
 target=target_path(ROOT).read_bytes();cfg=read(ROOT/'config/target.json')
 if sha(target)!=TARGET_SHA or len(target)!=cfg['size']:raise ValueError('target pin mismatch')
 integer=read(ROOT/'lanes/integer/mapping.json')
 other=read(ROOT/'lanes/resource_float/mapping.json')
 rows=[];spans=[];api_spans=[];checks=[]
 # API identity alone does not qualify as transferable MAT3 source identity.
 accepted={'InsertLLE','interp','BodyVector','enginetorque','UpdateFrameTimer'}
 for name in cfg['training']+cfg['heldout']:
  if name in integer['mappings']:
   item=integer['mappings'][name];s=item['spans'][0]
   off=s['file_start'];size=s['size'];expected=s['sha256'];score=item['score']
   evidence='lanes/integer/mapping.json';status=item['status']
   source_path=next(p for p in integer['source_hashes'] if Path(p).name==item['source'].split(':')[0])
   source={'path':source_path,'sha256':integer['source_hashes'][source_path]}
  else:
   item=next(x for x in other['mappings'] if x['function']==name);s=item['code']
   off=int(s['file_start'],0) if s else None;size=s['size'] if s else 0
   expected=s['sha256'] if s else None;score=item['identity_score']['total'] if item['identity_score'] else None
   evidence='lanes/resource_float/mapping.json';status=item['status'];source=item['source']
  if sha(Path(source['path']).read_bytes())!=source['sha256']:raise ValueError('reference changed: '+source['path'])
  row={'function':name,'set':'training' if name in cfg['training'] else 'heldout','MAT3_semantic_identity_accepted':name in accepted,'api_counterpart_only':name=='bsearch','equivalent_C_accepted':False,'compiled_SH_objects':0,'exact_local_link':False,'status':status,'score_annotation':score,'evidence':evidence,'source':source,'code':None}
  if s:
   actual=sha(target[off:off+size])
   if actual!=expected:raise ValueError('lane span hash mismatch: '+name)
   decoded=list(disassemble(target,start=off,end=off+size))
   unknown=[hex(i.pc) for i in decoded if not i.known]
   if unknown:raise ValueError('unknown instruction in pinned code: '+name)
   span={'function':name,'target_offset':off,'size':size,'kind':'code','candidate_address':BASE+off,'sha256':actual}
   (spans if name in accepted else api_spans).append(span)
   row['code']={'file_start':hex(off),'file_end_exclusive':hex(off+size),'va_start':hex(BASE+off),'size':size,'sha256':actual}
   checks.append({'function':name,'span_hash_verified':True,'decoded_instruction_count':len(decoded),'unknown_opcodes':unknown,'boundary_status':'static reviewed; not compiler/analyzer corroborated'})
  rows.append(row)
 validate_spans(spans+api_spans,len(target))
 # Dependencies are assigned to named groups, never duplicated per function.
 pool_ranges={}
 for name in ['interp','enginetorque']:
  for d in integer['mappings'][name].get('dependencies',[]):
   for off in range(d['file_start'],d['file_end']):pool_ranges[off]='drivetra_shared'
 timer=next(x for x in other['mappings'] if x['function']=='UpdateFrameTimer')
 for d in timer['shared_pool_dependencies']:
  s=d['span'];start=int(s['file_start'],0)
  if sha(target[start:start+s['size']])!=s['sha256']:raise ValueError('timer pool hash mismatch')
  for off in range(start,start+s['size']):
   if off in pool_ranges:raise ValueError('conflicting dependency groups')
   pool_ranges[off]='timer_shared'
 pools=[]
 for off,group in sorted(pool_ranges.items()):
  if pools and pools[-1]['group']==group and pools[-1]['target_offset']+pools[-1]['size']==off:pools[-1]['size']+=1
  else:pools.append({'group':group,'target_offset':off,'size':1,'kind':'literal','ownership':'central dependency group; not exclusive function ownership'})
 for p in pools:p['sha256']=sha(target[p['target_offset']:p['target_offset']+p['size']])
 validate_spans(spans+api_spans+pools,len(target))
 prior=read(Path(cfg['mat3_reference']).parent/'rush2049-dreamcast/mat3-to-dreamcast-evidence/reference-file-hashes.json')
 refchecks=[]
 for x in prior:
  p=Path(x['path']);actual=sha(p.read_bytes());refchecks.append({'path':str(p),'expected_sha256':x['sha256'],'actual_sha256':actual,'unchanged':actual==x['sha256']})
 if not all(x['unchanged'] for x in refchecks):raise ValueError('a previously pinned MAT3 reference changed')
 trials=read(ROOT/'analysis/trials.json') if (ROOT/'analysis/trials.json').exists() else []
 # This pass intentionally cannot silently promote results from compiler logs.
 if trials:raise ValueError('compiler trials require an explicit integration review')
 now=datetime.now(timezone.utc).isoformat()
 counts={'fixed_pilot_slots':7,'accepted_MAT3_semantic_identities':len(accepted),'additional_API_counterpart_with_source_drift':1,'unlocated_slots':1,'equivalent_C_accepted':0,'SH_compiler_invocations':0,'configuration_trials_used':0,'configuration_trial_cap':16,'emitted_SH_objects':0,'exact_local_links':0,'accepted_identity_code_union_bytes':sum(x['size'] for x in spans),'additional_API_counterpart_code_bytes':sum(x['size'] for x in api_spans),'shared_literal_dependency_union_bytes':sum(x['size'] for x in pools),'earned_source_code_bytes':0,'earned_source_data_bytes':0,'full_image_source_coverage_bytes':0,'full_image_denominator_bytes':len(target),'historical_object_identity':'unmeasurable: no original DC objects','whole_image_exact_rebuild':False}
 manifest={'schema':1,'generated_utc':now,'target':cfg,'functions':rows,'code_spans':spans,'API_counterpart_code_spans':api_spans,'literal_dependency_groups':pools,'counts':counts,'limitations':['Identified functions are static correspondences, not matching C.','Five accepted semantic identities exclude the drifted bsearch API counterpart.','GetWaveByIndex stays in the denominator; absence is not proven.','Shared pools assigned once; surrounding padding/data remains unowned.','Heldout functions were identified only; no heldout compilation or configuration tuning.','No original DC object metadata; no relocation masking or original-byte source credit.','No paired DC-only timing comparison; no quantified reuse speedup claim.']}
 save('manifest.json',manifest)
 save('identity-code-spans.json',{'scope':'identified code only; dependencies must also pass before any local-link promotion','spans':spans})
 save('dependency-spans.json',{'scope':'centrally assigned shared dependencies; do not count per-function copies','spans':pools})
 save('progress.json',{'generated_utc':now,**counts})
 save('span-checks.json',checks)
 save('reference-recheck.json',{'generated_utc':now,'files_checked':len(refchecks),'all_unchanged':True,'files':refchecks})
 control=compare_bytes(target,Artifact(target),spans+pools)
 save('target-self-control.json',{'self_control':True,'earned_source_bytes':0,'note':'Target versus itself validates address/span plumbing only. This is not a generated artifact or decompilation progress.',**control})
 print(json.dumps({'status':'validated','target_sha256':sha(target),'MAT3_references_unchanged':len(refchecks),**counts},indent=2))
if __name__=='__main__':main()
