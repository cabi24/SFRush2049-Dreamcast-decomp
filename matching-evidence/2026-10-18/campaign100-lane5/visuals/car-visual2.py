from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/visuals');s=(p/'baseline.c').read_text().replace('    tVisual *v; /* r5 */','    register tVisual *v; /* r5 */',1);Path('src/visuals.c').write_text(s)
