from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/stree');s=(p/'outside-split.c').read_text().replace('    q = posh >= midh;','    q = 0;\n    if (posh >= midh) q++;');Path('src/stree.c').write_text(s)
