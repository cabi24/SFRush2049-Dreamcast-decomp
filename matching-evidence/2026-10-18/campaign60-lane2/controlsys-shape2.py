from pathlib import Path
p=Path('src/controlsys.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/controlsys.original.c').read_text().replace('    int mask;\n','',1).replace('        mask=SetInterruptMask(15);','        int mask=SetInterruptMask(15);',1);p.write_text(s)
