from pathlib import Path
p=Path('src/textureanim.c');s=Path('matching-evidence/2026-10-18/work-textureanim/baseline/source.c').read_text();s=s.replace('    int i;','    int i;\n    char *name;',1).replace('for (; plist->name; plist++)','for (; (name = plist->name) != 0; plist++)',1).replace('MBOX_FindTexture_WORK(plist->name,','MBOX_FindTexture_WORK(name,',1);p.write_text(s)
