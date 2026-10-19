from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/videoscr');s=(p/'baseline.c').read_text().replace('        if (++gVideoMenuSelect >= 4) {','        gVideoMenuSelect++;\n        if (gVideoMenuSelect >= 4) {',1);Path('src/videoscr.c').write_text(s)
