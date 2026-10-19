from pathlib import Path
p=Path('src/controlscr.c');s=p.read_text().replace('gPlayerRects[gNumWindows-1][p].x + gPlayerRects[gNumWindows-1][p].w','gPlayerRects[gNumWindows-1][p].w + gPlayerRects[gNumWindows-1][p].x').replace('gPlayerRects[gNumWindows-1][p].y + gPlayerRects[gNumWindows-1][p].h','gPlayerRects[gNumWindows-1][p].h + gPlayerRects[gNumWindows-1][p].y');p.write_text(s)
