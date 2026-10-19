from pathlib import Path
s=Path('matching-evidence/2026-10-18/campaign60-lane5/stree/baseline.c').read_text();s=s.replace('    if ((outside = (posh >= tp->maxh || posh < tp->minh || posv >= tp->maxv || posv < tp->minv)) != 0) {','    outside = (posh >= tp->maxh || posh < tp->minh || posv >= tp->maxv || posv < tp->minv);\n    if (outside) {');Path('src/stree.c').write_text(s)
