from pathlib import Path
import json
p=Path('matching-evidence/2026-10-18/campaign100-lane2')
for f in p.glob('*-baseline.json'):
 r=json.loads(f.read_text());print(r['name'],[(x['name'],x['start'],x.get('linked_at','-'),x.get('linked_size','-'),x['size'],x['diff_bytes']) for x in r['functions'] if x['status']!='exact'])
