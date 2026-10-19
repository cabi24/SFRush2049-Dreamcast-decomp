import sys,os,json,pathlib
sys.path.insert(0,os.path.abspath('tools'));sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_build as B
import sh4
unit,label,name=sys.argv[1:4];p=pathlib.Path('matching-evidence/2026-10-18/campaign100-lane6')/unit/label
e=json.loads((p/'result.json').read_text());f=next(f for f in e['functions'] if f['name']==name)
_,image=B.built_image('build/campaign100-lane6/'+unit+'/'+label+'/link/'+unit+'.abs')
start=int(f['start'],16);ours=int(sys.argv[5],16) if len(sys.argv)>5 else int(f.get('linked_at',f['start']),16)
size=f['size'] if len(sys.argv)>5 else f.get('linked_size',f['size']);want=B.C.target_bytes(start,int(f['end'],16));mine=bytes(image.get(ours+i,0) for i in range(size))
lines=[]
for i in range(0,max(len(want),len(mine)),2):
 def d(blob,a):
  if i>=len(blob):return ''
  v=sh4.decode(int.from_bytes(blob[i:i+2],'little'),a+i,blob,a)
  return v.text+(' =%08X'%v.literal_value if v.literal_value is not None else '')
 lines.append('%08X %-54s | %08X %-54s %s'%(start+i,d(want,start),ours+i,d(mine,ours),'*' if want[i:i+2]!=mine[i:i+2] else ''))
(p/(name+'-words.txt')).write_text('\n'.join(lines));print(json.dumps(f));print('\n'.join(lines[:int(sys.argv[4]) if len(sys.argv)>4 else 80]))
