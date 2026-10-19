import sys,os,json
sys.path.insert(0,os.path.abspath('tools'))
sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_build as B
import sh4
unit,label,name=sys.argv[1:4]
p='matching-evidence/2026-10-18/round2-controls-drones/'+label
e=json.load(open(p+'/result.json'))
f=next(f for f in e['functions'] if f['name']==name)
_,im=B.built_image(p+'/link/'+unit+'.abs')
st,en=int(f['start'],16),int(f['end'],16)
for a in range(st,en,2):
 w=B.C.target_bytes(a,a+2); g=bytes(im.get(a+k,0) for k in range(2))
 if w!=g or '--all' in sys.argv:
  print('%08X %s %-38s | %s %s'%(a,g.hex(),sh4.decode(int.from_bytes(g,'little'),a).text,w.hex(),sh4.decode(int.from_bytes(w,'little'),a).text))
