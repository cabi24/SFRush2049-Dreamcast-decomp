from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane2/run.py');s=p.read_text().replace('campaign60-lane2','campaign100-lane2').replace(" controlsys controlscr drivsym fmath game gameoverscr'"," controlsys controlscr drivsym fmath game'");p.write_text(s)
