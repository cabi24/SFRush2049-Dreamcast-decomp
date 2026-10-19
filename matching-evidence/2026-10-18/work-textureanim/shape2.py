from pathlib import Path
p=Path('src/textureanim.c');s=Path('matching-evidence/2026-10-18/work-textureanim/baseline/source.c').read_text();s=s.replace('    tTexture *ttDef;','    void *ttDef;',1);p.write_text(s)
