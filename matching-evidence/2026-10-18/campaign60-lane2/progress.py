import json,sys
from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane2');f=p/'progress.json'
d=json.loads(f.read_text()) if f.exists() else {}
unit,status,note=sys.argv[1:4];d[unit]={'status':status,'note':note};f.write_text(json.dumps(d,indent=2))
units='camera checkpoint circuit collision controlsys controlscr drivsym fmath game gameoverscr'.split()
(p/'progress.md').write_text('# Campaign 60 lane 2\n\n'+'\n'.join('- '+u+': '+d.get(u,{}).get('status','pending')+'; '+d.get(u,{}).get('note','') for u in units)+'\n')
