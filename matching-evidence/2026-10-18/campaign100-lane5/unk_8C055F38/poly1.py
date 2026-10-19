from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/unk_8C055F38');s=(p/'baseline.c').read_text().replace('    float tt1; /* f0 */\n','').replace('modff(x, &tt1)','modff(x, &g)').replace('((tt1 - xn * C1)','((g - xn * C1)');Path('src/unk_8C055F38.c').write_text(s)
