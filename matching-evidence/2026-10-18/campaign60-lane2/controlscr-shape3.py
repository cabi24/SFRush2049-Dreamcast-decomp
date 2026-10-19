from pathlib import Path
p=Path('src/controlscr.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/controlscr.original.c').read_text();old='''                int farX, farY;
                farY = gPlayerRects[gNumWindows-1][p].y + gPlayerRects[gNumWindows-1][p].h;
                farX = gPlayerRects[gNumWindows-1][p].x + gPlayerRects[gNumWindows-1][p].w;
                InitPane(gPlayerRects[gNumWindows-1][p].x,
                    gPlayerRects[gNumWindows-1][p].y, farX, farY,176,0,0,0);''';new='''                anon_x_y_18 *rect = &gPlayerRects[gNumWindows-1][p];
                InitPane(rect->x, rect->y, rect->x + rect->w,
                    rect->y + rect->h,176,0,0,0);''';assert old in s;s=s.replace(old,new);p.write_text(s)
