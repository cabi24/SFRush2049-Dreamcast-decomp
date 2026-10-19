import json, hashlib, csv
from pathlib import Path
P=Path(__file__).resolve().parent
events=[json.loads(x) for x in (P/'events.jsonl').read_text().splitlines()]
attempts=[x for x in events if x['label'].startswith('A')]
rows=[]
for i in range(1,501):
    ident='A%04d'%i
    event=next((e for e in attempts if e['label'].startswith(ident+'-')),None)
    rows.append({'id':ident,'status':'completed' if event else 'pending','unit':event['unit'] if event else '',
                 'evidence':event['report'] if event else '', 'result':'no new exact credit' if event else ''})
with (P/'queue.csv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=rows[0].keys());w.writeheader();w.writerows(rows)
manifest={str(p.relative_to(P)):hashlib.sha256(p.read_bytes()).hexdigest() for p in P.rglob('*') if p.is_file() and p.name not in ('checkpoint.json','manifest.json','queue.csv')}
(P/'manifest.json').write_text(json.dumps(manifest,indent=2))
summary={'attempts':len(attempts),'quota':500,'remaining':500-len(attempts),'new_exact_bytes':0,'new_exact_functions':0,
         'completed_ids':[r['id'] for r in rows if r['status']=='completed'],
         'coordination_error':'AGENTS_BUSY: no agent family belongs to this conversation. Other primes and their workers are isolated.',
         'shared_files_modified_by_this_lane':[], 'counting':'Baselines and diagnostics excluded; source changes plus native compile/link/strict compare only.'}
(P/'checkpoint.json').write_text(json.dumps(summary,indent=2))
print(json.dumps(summary))
