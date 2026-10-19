from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/targetpaths');s=(p/'baseline.c').read_text().replace('        dV = (dV * dT * dT) / (2.0f * pt->timeToNext);','        dV *= dT;\n        dV *= dT;\n        dV /= 2.0f * pt->timeToNext;');Path('src/targetpaths.c').write_text(s)
