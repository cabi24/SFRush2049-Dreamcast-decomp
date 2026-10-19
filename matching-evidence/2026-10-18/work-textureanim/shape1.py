from pathlib import Path
p=Path('src/textureanim.c');s=p.read_text();s=s.replace('    tTexture *ttDef;\n','').replace('                ttDef = MBOX_FindTexture_WORK(plist->name, &index, 0, gNumModels - 1, 1);\n                plist->ptr = ttDef;','                plist->ptr = MBOX_FindTexture_WORK(plist->name, &index, 0, gNumModels - 1, 1);');p.write_text(s)
