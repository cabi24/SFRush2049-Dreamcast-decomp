from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/visuals');s=(p/'baseline.c').read_text().replace('    tVisual *v; /* r5 */\n    signed short i; /* r6 */\n    for (v = gc->visuals, i = 0;','    signed short i; /* r6 */\n    tVisual *v; /* r5 */\n    for (i = 0, v = gc->visuals;');Path('src/visuals.c').write_text(s)
