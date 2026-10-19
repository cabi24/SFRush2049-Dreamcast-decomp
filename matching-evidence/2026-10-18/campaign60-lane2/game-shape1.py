from pathlib import Path
p=Path('src/game.c');s=p.read_text();a=s.index('int GameLoop(void)');b=s.index('void OncePerFrame',a);part=s[a:b].replace('gFrames=0','(*(volatile unsigned int *)&gFrames)=0').replace('gFrames++','(*(volatile unsigned int *)&gFrames)++');p.write_text(s[:a]+part+s[b:])
