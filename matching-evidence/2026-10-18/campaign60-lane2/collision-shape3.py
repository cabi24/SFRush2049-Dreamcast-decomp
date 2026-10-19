from pathlib import Path
p=Path('src/collision.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/collision.original.c').read_text().replace('        dsq = 0.0f;\n        for (i = 0; i < 3; i++) {\n            dsq += vec[i] * vec[i];\n        }','        dsq = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];',1);p.write_text(s)
