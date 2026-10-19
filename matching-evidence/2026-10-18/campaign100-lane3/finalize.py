import json,hashlib
from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane3')
tasks=json.loads((p/'tasks.json').read_text())
rows=['# Campaign100 lane3 -17/17 addressed','', 'RESULT: +168 strict text/literal bytes, +1 exact function (HAL_PurgeFonts), +0 data. One success, remaining16 bounded or context-blocked. Addressed counts do not imply matches.','', '| # | Unit | Target | Outcome / attempts / residual |','|---|---|---|---|']
for i,(u,t,s) in enumerate(tasks,1):rows.append('| %d | %s | %s | %s |'%(i,u,t,s))
rows += ['', 'CHANGES: Only src/HAL_text.c retained. HAL_PurgeFonts puts each FindWaveInstance assignment inside its >=0 condition. This compares returned r0 before moving to argument r4, reproducing all168 target bytes at8C02EFBE..8C02F066. Three _HAL_SetFont source-shape failures restored before selecting this independent change.', '', 'VALIDATION: Fresh native baseline for all9 units; isolated build_unit with frozen SHC5.1r11 flags. Independent final native rebuild selected at build/campaign100-lane3/HAL_text/final-rebuild/. All baseline exact function/pool/data spans verified unchanged by check.py. No masks/moved/fill/gap/BSS credit. Objects and absolute files remain private build. Final source/object/abs hashes are in HAL_text/final-rebuild/verification.json. All8 unrelated owned sources exactly restored to current campaign baselines.', '', 'BLOCKERS: Header/map ownership precludes global placement repairs, and artificial padding/section hacks were not used. Missing preceding source and existing capped context residuals are recorded per target. No successful patch replay; no shared report/receipt/objdiff/header/map/extern/tool/HANDOFF changes, downloads,installs,security bypass,MAT3 writes,game/full builds,commits or pushes.', '', 'Reproduce selected result: python -B matching-evidence/2026-10-18/campaign100-lane3/run.py HAL_text LABEL; python -B matching-evidence/2026-10-18/campaign100-lane3/check.py HAL_text final-rebuild.']
(p/'progress.md').write_text('\n'.join(rows)+'\n')
units=list(dict.fromkeys(x[0] for x in tasks))
hashes={}
for u in units:
 f=Path('src')/(u+'.c');data=f.read_bytes();base=(p/u/'baseline/source.c').read_bytes()
 if u!='HAL_text':assert data==base,u
 hashes[u]={'sha256':hashlib.sha256(data).hexdigest(),'restored_baseline':data==base}
(p/'sources-final.json').write_text(json.dumps(hashes,indent=2))
print('Verified8 restored sources; HAL_text only retained delta.17 tasks ledger saved.')
