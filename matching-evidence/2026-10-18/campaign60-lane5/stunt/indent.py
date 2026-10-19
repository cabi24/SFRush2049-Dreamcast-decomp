from pathlib import Path
p=Path('src/stunt.c');s=p.read_text();old='''        if (okToStart && absW[2] > absW[1] &&
               absW[2] > absW[0]) {
        plyrInfo->active |= 0x20;
        plyrInfo->stuntW[0][0] = gc->stuntW[0];
        plyrInfo->stuntW[0][1] = gc->stuntW[1];
        plyrInfo->stuntW[0][2] = gc->stuntW[2];
    }
    }
''';new='''        if (okToStart && absW[2] > absW[1] &&
            absW[2] > absW[0]) {
            plyrInfo->active |= 0x20;
            plyrInfo->stuntW[0][0] = gc->stuntW[0];
            plyrInfo->stuntW[0][1] = gc->stuntW[1];
            plyrInfo->stuntW[0][2] = gc->stuntW[2];
        }
    }
''';assert old in s;s=s.replace(old,new,1);p.write_text(s)
