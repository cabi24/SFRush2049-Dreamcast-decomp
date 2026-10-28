import json,struct,re
from pathlib import Path
import argparse
p=argparse.ArgumentParser();p.add_argument('--target',required=True);p.add_argument('--map',required=True);p.add_argument('--output',required=True);a=p.parse_args()
b=Path(a.target).read_bytes(); us=json.load(open(a.map))['units']; by={int(f['start'],16):(u,f) for u in us for f in u['functions']}; calls=[];false=[]
for u in us:
 if u['kind']!='game':continue
 pools=[(int(x['start'],16),int(x['end'],16)) for x in u['pools']]
 for f in u['functions']:
  for pc in range(int(f['start'],16),int(f['end'],16),2):
   w=struct.unpack_from('<H',b,pc-0x8c010000)[0]
   if w>>12!=11:continue
   d=w&4095;d=d-4096 if d&2048 else d;t=pc+4+2*d
   if t not in by:continue
   tu,tf=by[t];rec=dict(unit=u['name'],caller=f['name'],caller_address=f['start'],site=hex(pc),callee_unit=tu['name'],callee=tf['name'],callee_address=tf['start'])
   if any(a<=pc<z for a,z in pools):false.append(rec);continue
   calls.append(rec)
out=dict(scope='Mapped game function spans, excludes declared literal pools; static BSR opcode scan, not control-flow proof',calls=calls,cross_unit=[x for x in calls if x['unit']!=x['callee_unit']],pool_false_cross=[x for x in false if x['unit']!=x['callee_unit']])
Path(a.output).write_text(json.dumps(dict(scope=out['scope'],mapped_sites=len(calls),cross_unit_sites=len(out['cross_unit']),pool_false_cross_sites=len(out['pool_false_cross'])),indent=2)+'\n')
print('mapped BSR opcode sites:',len(calls),'cross-unit sites:',len(out['cross_unit']),'false cross-unit pool sites:',len(out['pool_false_cross']))
