"""Recheck saved native linked outputs and unmasked, nonpadding source credit.
No compiler invocation. Writes the local report summary and a reproducible receipt.
"""
from pathlib import Path
import hashlib,json,time
import dc_common as C
import dc_build as B
from sysrof_abs import read_abs
ROOT=Path(C.DC)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def intervals(addresses):
 out=[]
 for a in sorted(addresses):
  if out and out[-1][1]==a:out[-1][1]=a+1
  else:out.append([a,a+1])
 return [[f'{a:08X}',f'{b:08X}'] for a,b in out]
def main():
 m=C.load_map();report_path=ROOT/'build/report.json';r=json.loads(report_path.read_text())
 assert r['map_sha256']==m['sha256'] and r['target_sha256']==C.TARGET_SHA256
 C.load_target();owners={};rows=[]
 for e in r['units']:
  name=e['name'];u=next(x for x in m['units'] if x['name']==name)
  assert sha(ROOT/e['source'])==e['source_sha256'],name
  ap=ROOT/'build/link'/(name+'.abs');sections,runs=read_abs(str(ap));_s,image=B.built_image(str(ap))
  emitted={a+i for _sec,a,data in runs for i in range(len(data))}
  code=set();data=set()
  for seg in B.text_segments(u):
   ex,_built,_diff=B.compare_range(image,seg['start'],seg['end'])
   if ex and seg['kind'] in ['func','pool']:code.update(a for a in range(seg['start'],seg['end']) if a in emitted)
  for d in u['data']:
   ex,_built,_diff=B.compare_range(image,d['start'],d['end'])
   if ex:data.update(a for a in range(d['start'],d['end']) if a in emitted)
  assert len(code)==e['exact_text_bytes'] and len(data)==e['exact_data_bytes'],name
  for a in code|data:
   if a in owners:raise ValueError(f'credit overlap {a:08X}: {owners[a]} / {name}')
   owners[a]=name
  for f in e['functions']:
   ex,_b,_d=B.compare_range(image,int(f['start'],16),int(f['end'],16))
   assert (f['status']=='exact')==ex,(name,f['name'])
  rows.append({'unit':name,'text_nonpadding':len(code),'data_nonpadding':len(data),'exact_functions':e['functions_exact'],'text_spans':intervals(code),'data_spans':intervals(data),'source_sha256':e['source_sha256'],'expanded_source_sha256':hashlib.sha256(B.expand_includes(str(ROOT/e['source'])).encode('latin-1')).hexdigest(),'linked_abs_sha256':sha(ap),'object_sha256':sha(ROOT/'build/obj'/(name+'.obj'))})
 r['summary']=B.summarize(m,{e['name']:e for e in r['units']});r['generated']=time.strftime('%Y-%m-%dT%H:%M:%S')
 r['credit_policy']='Only explicitly emitted bytes in entirely exact fixed-address function/pool/data ranges. All linker-fill, gaps and alignment excluded. No masks.'
 report_path.write_text(json.dumps(r,indent=1)+'\n')
 receipt={'target_sha256':C.TARGET_SHA256,'map_sha256':m['sha256'],'summary':r['summary'],'credited_union_bytes':len(owners),'overlap':False,'comparison_masks':False,'compiler_executed_by_audit':False,'tool_hashes':{p:sha(ROOT/'tools'/p) for p in ['dc_build.py','shc_cc.py','shc_link.py','sysrof_abs.py','dc_common.py','audit_report.py']},'header_sha256':sha(ROOT/'include/dc_types.h'),'units':rows}
 (ROOT/'evidence').mkdir(exist_ok=True);(ROOT/'evidence/strict-integration-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n')
 print(json.dumps({'strict_report_validated':True,'summary':r['summary'],'credited_union_bytes':len(owners),'credit_overlap':False},indent=2))
if __name__=='__main__':main()
