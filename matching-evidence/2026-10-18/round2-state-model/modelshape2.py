from pathlib import Path
p=Path('src/model.c');s=Path('matching-evidence/2026-10-18/round2-state-model/model.original.c').read_text();s=s.replace('    high_index = -1;','    {\n    float catchup_scale = 0.100000002f;\n    high_index = -1;').replace('scalef(lagdist, 0.100000002f)','scalef(lagdist, catchup_scale)');s=s.rstrip()+'\n}\n';p.write_text(s)
