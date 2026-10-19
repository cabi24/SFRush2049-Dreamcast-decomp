from pathlib import Path
s=Path('matching-evidence/2026-10-18/campaign60-lane5/stree/baseline.c').read_text().replace('signed short outside;','volatile signed short outside;');Path('src/stree.c').write_text(s)
