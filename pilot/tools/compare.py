"""Strict comparisons for the pinned raw SH-4 image. No relocation masking.
Raw linked artifacts or allocated ELF32 little-endian SH executable sections are supported.
A passing comparison is evidence, not automatic source-match promotion.
"""
from pathlib import Path
import argparse, hashlib, json, struct
from dc_target import target_path

ROOT=Path(__file__).resolve().parent.parent
BASE=0x8C010000
TARGET_SHA='336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688'

class Artifact:
 def __init__(self,data,base=BASE,format='auto'):
  self.data=data;self.regions=[];self.format='raw';self.entry=None
  if format=='elf' or (format=='auto' and data[:4]==b'\x7fELF'):
   if len(data)<52 or data[:6]!=b'\x7fELF\x01\x01':raise ValueError('requires ELF32 little-endian')
   typ,machine=struct.unpack_from('<HH',data,16)
   if machine!=42:raise ValueError('ELF machine is not SH (42)')
   if typ!=2:raise ValueError('exact linked comparison requires ET_EXEC, not relocatable objects')
   self.entry=struct.unpack_from('<I',data,24)[0]
   shoff=struct.unpack_from('<I',data,32)[0];entsize,count=struct.unpack_from('<HH',data,46)
   if entsize<40 or shoff+entsize*count>len(data):raise ValueError('invalid ELF section table')
   for i in range(count):
    _,kind,flags,addr,off,size,*rest=struct.unpack_from('<10I',data,shoff+i*entsize)
    if flags&2 and kind==1 and size:
     if off+size>len(data):raise ValueError('truncated allocated section')
     self.regions.append((addr,data[off:off+size]))
   if not self.regions:raise ValueError('no allocated file-backed sections')
   self.format='elf32-sh-le-executable'
  else:self.regions=[(base,data)]
  intervals=sorted((a,a+len(b)) for a,b in self.regions)
  if any(b>c for (a,b),(c,d) in zip(intervals,intervals[1:])):raise ValueError('overlapping artifact regions')
 def read(self,address,size):
  for start,data in self.regions:
   if start<=address and address+size<=start+len(data):return data[address-start:address-start+size]
  raise ValueError(f'artifact does not cover 0x{address:X}+0x{size:X}')

def validate_spans(spans,target_size):
 if not spans:raise ValueError('empty spans cannot establish a match')
 ranges=[]
 for span in spans:
  off=span['target_offset'];size=span['size']
  if not isinstance(off,int) or not isinstance(size,int) or off<0 or size<=0 or off+size>target_size:raise ValueError('invalid target span')
  if span.get('kind') not in ['code','literal','data','padding']:raise ValueError('unknown span kind')
  ranges.append((off,off+size))
 ranges.sort()
 if any(b>c for (a,b),(c,d) in zip(ranges,ranges[1:])):raise ValueError('overlapping spans would double-count bytes')

def compare_bytes(target,artifact,spans):
 validate_spans(spans,len(target));rows=[];failures=[]
 for span in spans:
  off=span['target_offset'];size=span['size'];address=span.get('candidate_address',BASE+off)
  expected=target[off:off+size]
  try:actual=artifact.read(address,size)
  except ValueError as exc:
   rows.append({**span,'covered':False,'error':str(exc),'exact':False});failures.append(str(exc));continue
  mismatches=[i for i,(a,b) in enumerate(zip(expected,actual)) if a!=b]
  rows.append({**span,'candidate_address':address,'covered':True,'exact':not mismatches,'differing_bytes':len(mismatches),'first_differences':[{'target_offset':off+i,'expected':expected[i],'actual':actual[i]} for i in mismatches[:16]],'expected_sha256':hashlib.sha256(expected).hexdigest(),'actual_sha256':hashlib.sha256(actual).hexdigest()})
 return {'exact':all(r['exact'] for r in rows),'compared_bytes':sum(s['size'] for s in spans),'equal_bytes':sum(s['size']-r.get('differing_bytes',s['size']) for s,r in zip(spans,rows)),'spans':rows,'errors':failures,'masks_used':False,'promotion':'not automatic; source, toolchain, semantic and boundary provenance require review'}

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('artifact',type=Path);ap.add_argument('--spans',type=Path,required=True);ap.add_argument('--target',type=Path,default=None);ap.add_argument('--format',choices=['auto','raw','elf'],default='auto');ap.add_argument('--artifact-base',type=lambda s:int(s,0),default=BASE);ap.add_argument('--out',type=Path);ap.add_argument('--self-control',action='store_true');args=ap.parse_args()
 if args.target is None:args.target=target_path(ROOT)
 target=args.target.read_bytes()
 if hashlib.sha256(target).hexdigest()!=TARGET_SHA:raise ValueError('pinned target hash mismatch')
 blob=args.artifact.read_bytes();spec=json.loads(args.spans.read_text());spans=spec['spans'] if isinstance(spec,dict) else spec
 if hashlib.sha256(blob).hexdigest()==TARGET_SHA and not args.self_control:raise ValueError('target-as-candidate requires --self-control and earns no match coverage')
 result=compare_bytes(target,Artifact(blob,args.artifact_base,args.format),spans);result.update({'artifact':str(args.artifact),'artifact_sha256':hashlib.sha256(blob).hexdigest(),'target_sha256':TARGET_SHA,'self_control':args.self_control,'earned_source_bytes':0})
 if args.out:args.out.parent.mkdir(parents=True,exist_ok=True);args.out.write_text(json.dumps(result,indent=2))
 print(json.dumps(result,indent=2));return 0 if result['exact'] else 1
if __name__=='__main__':raise SystemExit(main())
