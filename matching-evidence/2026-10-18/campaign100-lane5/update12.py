import json,pathlib
p=pathlib.Path('matching-evidence/2026-10-18/campaign100-lane5');r=json.loads((p/'tasks.json').read_text());updates={8:('bounded blocker; restored',1,'SetupTarget name declaration reorder unchanged; first block stack/register/pool allocation remains.'),9:('evidence blocker',0,'LocalPointInBody all68 bytes identical at target+240; prior functions accumulate240 byte drift, local body changes would harm proven code.'),10:('bounded blocker; restored',2,'Explicit copy byte at block and function scopes unchanged6 register-selection bytes.'),11:('strict win retained',2,'Target inlines center/right/font-height helpers inside DrawText; source pragma before declarations fixes Printf, DrawText, DrawEllipseText, DrawTextC and pool. First pragma position failed compile2805 then corrected.')}
for i,(s,n,b) in updates.items():r[i].update(status=s,attempts=n,residual=b)
r[11].update(delta_functions=4,delta_text=410)
(p/'tasks.json').write_text(json.dumps(r,indent=2))
