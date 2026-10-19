from pathlib import Path
import json,sys
P=Path('matching-evidence/2026-10-18/campaign100-lane2')
tasks=[('camera','init_view3'),('camera','SetCameraView'),('checkpoint','CheckCPs'),('checkpoint','PassedCP'),('circuit','UpdateCircuit'),('circuit','GetCircuitCode'),('collision','ForceApart'),('collision','MiniForceApart'),('controlsys','UpdateControls'),('controlsys','ControllerUpdate'),('controlscr','cleanupControlScreen'),('controlscr','validControl'),('drivsym','MovingSurfaceForce'),('drivsym','forces1'),('fmath','CreateYPRMatrix'),('fmath','ExtractYaw'),('game','_UpdateGame')]
f=P/'progress.json';d=json.loads(f.read_text()) if f.exists() else [{'task':i+1,'unit':u,'target':t,'status':'pending','attempts':0,'delta':0,'residual':''} for i,(u,t) in enumerate(tasks)]
if len(sys.argv)>1:
 n,status,attempts,residual=sys.argv[1:5];r=d[int(n)-1];r.update(status=status,attempts=int(attempts),residual=residual)
f.write_text(json.dumps(d,indent=2));(P/'progress.md').write_text('# Campaign100 lane2\n\n|Task|Unit|Target|Status|Attempts|Credit delta|Residual|\n|---|---|---|---|---|---|---|\n'+'\n'.join('|'+ '|'.join(str(r[k]) for k in ('task','unit','target','status','attempts','delta','residual'))+'|' for r in d)+'\n')
