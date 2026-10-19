import sys,json
from pathlib import Path
R=Path(r'C:\rush2049-dreamcast')
sys.path[:0]=[str(R/'tools'),str(R/'pilot/tools')]
import dc_build as D
import sh4
unit,label,fn=sys.argv[1:4]
E=Path(__file__).resolve().parent
e=json.loads((E/(unit+'-'+label+'.json')).read_text())
f=next(x for x in e['functions'] if x['name']==fn)
_,im=D.built_image(str(R/'build/campaign100-lane1'/unit/label/'link'/f'{unit}.abs'))
a=int(f['start'],16); b=int(f.get('linked_at',f['start']),16)
lines=[str(f)]
for o in range(0,max(f['size'],f.get('linked_size',0)),2):
 w=D.C.target_bytes(a+o,a+o+2); g=bytes(im.get(b+o+i,0) for i in range(2))
 if w is None: break
 x=sh4.decode(int.from_bytes(g,'little'),b+o);y=sh4.decode(int.from_bytes(w,'little'),a+o)
 lines.append(f'{b+o:08X} {g.hex()} {x.text:42} | {a+o:08X} {w.hex()} {y.text}')
(E/(unit+'-'+label+'-'+fn+'-aligned.txt')).write_text('\n'.join(lines))
print('\n'.join(lines))
