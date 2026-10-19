from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/unk_8C055F38');s=(p/'baseline.c').read_text().replace('func_8C055F38(float x)','func_8C055F38(register float x)');Path('src/unk_8C055F38.c').write_text(s)
