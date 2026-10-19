# Survey: does the saved build/report.json reflect the dirty tree or HEAD, per unit?
# Also dumps per-unit baseline stats from the report for the dirty units.
import json, hashlib, subprocess
from pathlib import Path

ROOT = Path(r'C:\rush2049-dreamcast')
rep = json.load(open(ROOT / 'build/report.json'))
units = {u['name']: u for u in rep['units']}

DIRTY = ['HAL_text', 'Init', 'TB_text', 'blit', 'controls', 'dc_misc_8C06C08C',
         'dc_unk_8C069E34', 'fmath', 'game', 'model', 'options', 'optionscr',
         'pakerr', 'pane', 'streaks', 'stree', 'stunt', 'unk_8C065F2C',
         'visuals', 'zoid', 'zoidpoly']

def sha(b):
    return hashlib.sha256(b).hexdigest()

print('report generated:', rep['generated'])
print()
print(f"{'unit':<22}{'src_match':<11}{'kind':<8}{'status':<10}{'funcs':<7}{'funcs_ex':<9}{'moved':<7}{'text_bytes':<11}{'text_ex':<9}{'data_ex':<8}")
print('-' * 100)

for n in DIRTY:
    u = units.get(n)
    if not u:
        print(f'{n:<22}  NOT IN REPORT')
        continue
    wt = sha((ROOT / 'src' / f'{n}.c').read_bytes())
    head = subprocess.run(['git', 'show', f'HEAD:src/{n}.c'], capture_output=True, cwd=ROOT).stdout
    if wt == u.get('source_sha256'):
        m = 'WT(dirty)'
    elif sha(head) == u.get('source_sha256'):
        m = 'HEAD'
    else:
        m = 'NEITHER'
    print(f"{n:<22}{m:<11}{u.get('kind','?'):<8}{u.get('status','?'):<10}"
          f"{u.get('functions_total',0):<7}{u.get('functions_exact',0):<9}"
          f"{u.get('functions_moved',0):<7}{u.get('text_bytes',0):<11}"
          f"{u.get('exact_text_bytes',0):<9}{u.get('exact_data_bytes',0):<8}")

s = rep['summary']
print()
print('summary:', json.dumps(s, indent=1))
