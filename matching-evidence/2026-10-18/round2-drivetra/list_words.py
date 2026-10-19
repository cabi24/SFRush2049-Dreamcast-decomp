import sys,os,json,pathlib,struct
sys.path.insert(0,os.path.abspath('tools'))
sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_build as B
import sh4
p=pathlib.Path('matching-evidence/2026-10-18/round2-drivetra')
label=sys.argv[1]
e=json.loads((p/label/'result.json').read_text())
_,image=B.built_image(str(p/label/'link/drivetra.abs'))
f=next(f for f in e['functions'] if f['name']=='enginetorque')
start=int(f['start'],16);ours=int(f['linked_at'],16)
want=B.C.target_bytes(start,int(f['end'],16))
mine=bytes(image.get(ours+i,0) for i in range(f['linked_size']))
lines=[]
for i in range(0,max(len(want),len(mine)),2):
 def d(blob,a):
  return sh4.decode(int.from_bytes(blob[i:i+2],'little'),a+i,blob,a).text if i<len(blob) else ''
 lines.append('%08X %-46s | %08X %-46s %s'%(start+i,d(want,start),ours+i,d(mine,ours),'*' if want[i:i+2]!=mine[i:i+2] else ''))
(p/label/'enginetorque-list.txt').write_text('\n'.join(lines))
print(json.dumps({k:e.get(k) for k in ['source_sha256','functions_exact','exact_text_bytes','text_size_delta']}))
print(json.dumps(f))
print('\n'.join(lines[:16]))
