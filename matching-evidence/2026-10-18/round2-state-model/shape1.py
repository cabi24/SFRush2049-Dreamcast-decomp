from pathlib import Path
p=Path('src/state.c');s=p.read_text();s=s.replace('        int p;','').replace('for (p = 0; p < gNumPlayers; p++)','for (i = 0; i < gNumPlayers; i++)').replace('for (; p < 4; p++)','for (; i < 4; i++)').replace('Active(p,','Active(i,').replace('Edges(p,','Edges(i,').replace('if (p > 1)','if (i > 1)');p.write_text(s)
