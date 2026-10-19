from pathlib import Path
old=Path('matching-evidence/2026-10-18/campaign60-lane3')
new=Path('matching-evidence/2026-10-18/campaign100-lane3')
for name in ('run.py','check.py','listing.py'):
 s=(old/name).read_text().replace('campaign60-lane3','campaign100-lane3')
 (new/name).write_text(s)
