from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/unk_8C055F38');s=(p/'baseline.c').read_text().replace('    float xn; /* f0 */','    float tt1; /* stack0 candidate */').replace('    float tt1; /* f0 */','    float xn; /* stack4 candidate */');Path('src/unk_8C055F38.c').write_text(s)
