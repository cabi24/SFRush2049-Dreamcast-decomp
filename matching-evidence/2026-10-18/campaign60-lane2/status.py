import json
from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane2')
for f in p.glob('*-baseline.json'):
 r=json.loads(f.read_text());print(r['name'],r['functions_exact'],r.get('text_size_delta'),[(x['name'],x['size'],x['diff_bytes']) for x in r['functions'] if x['status']!='exact'])
