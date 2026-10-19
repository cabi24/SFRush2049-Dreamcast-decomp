from pathlib import Path
p=Path('src/collision.c');s=p.read_text();a=s.index('void MiniForceApart(');b=s.index('/* 0x8000bf1c',a);part=s[a:b].replace('    float *cf;\n','').replace('    cf = m->centerForce;\n','').replace('cf[','m->centerForce[');p.write_text(s[:a]+part+s[b:])
