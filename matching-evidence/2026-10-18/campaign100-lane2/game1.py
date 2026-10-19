from pathlib import Path
p=Path('src/game.c');s=p.read_text();old='''            for (p = 0; p < gNumPlayers; p++) {
                if (!gDeathMode && !AreWeSolid(p)) allSolid = 0;
            }''';new='''            p = 0;
            goto CheckSolid;
        NextSolid:
            if (!gDeathMode && !AreWeSolid(p)) allSolid = 0;
            p++;
        CheckSolid:
            if (p < gNumPlayers) goto NextSolid;''';assert old in s;p.write_text(s.replace(old,new))
