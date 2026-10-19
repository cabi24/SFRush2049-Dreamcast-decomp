import sys,os,json,pathlib
sys.path.insert(0,os.path.abspath('tools'));sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_build as B
import sh4
unit,label,name=sys.argv[1:4]
p=pathlib.Path('matching-evidence/2026-10-18/campaign60-lane6')/unit/label
e=json.loads((p/'result.json').read_text());u=next(u for u in B.C.load_map(None)['units'] if u['name']==unit)
f=next(f for f in e['functions'] if f['name']==name)
_,image=B.built_image('build/campaign60-lane6/'+unit+'/'+label+'/link/'+unit+'.abs')
start=int(f['start'],16);ours=int(f.get('linked_at',f['start']),16)
size=f.get('linked_size',f['size']);want=B.C.target_bytes(start,int(f['end'],16));mine=bytes(image.get(ours+i,0) for i in range(size))
lines=[]
for i in range(0,max(len(want),len(mine)),2):
 def d(blob,a):
  return sh4.decode(int.from_bytes(blob[i:i+2],'little'),a+i,blob,a).text if i<len(blob) else ''
 lines.append('%08X %-46s | %08X %-46s %s'%(start+i,d(want,start),ours+i,d(mine,ours),'*' if want[i:i+2]!=mine[i:i+2] else ''))
(p/(name+'-words.txt')).write_text('\n'.join(lines))
print(json.dumps(f));print('\n'.join(lines[:int(sys.argv[4]) if len(sys.argv)>4 else 80]))
