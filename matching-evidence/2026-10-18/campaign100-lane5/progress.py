import pathlib,json
p=pathlib.Path('matching-evidence/2026-10-18/campaign100-lane5');rows=json.loads((p/'tasks.json').read_text());s=['# Campaign100 lane5','', '| ID | Unit | Target | Status | Attempts | Delta functions/text | Residual |','|---|---|---|---|---:|---|---|']
for x in rows:s.append('| %s | %s | %s | %s | %s | %s/%s | %s |'%(x['id'],x['unit'],x['target'],x['status'],x['attempts'],x['delta_functions'],x['delta_text'],x['residual']))
(p/'progress.md').write_text('\n'.join(s)+'\n')
