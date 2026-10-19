from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/videoscr');s=(p/'baseline.c').read_text().replace('    unsigned int autoEdges;','    int autoEdges;');Path('src/videoscr.c').write_text(s)
