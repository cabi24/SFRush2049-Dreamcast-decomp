from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/visuals');s=(p/'baseline.c').read_text().replace('    register int i; /* r4 */','    int i; /* r4 */');Path('src/visuals.c').write_text(s)
