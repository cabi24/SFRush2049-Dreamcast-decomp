from pathlib import Path
p=Path('src/drivsym.c');s=p.read_text();s=s.replace('(gWind * (m->uvs[0][0] * gWind))','((m->uvs[0][0] * gWind) * gWind)').replace('(gWind * (m->uvs[1][0] * gWind))','((m->uvs[1][0] * gWind) * gWind)');p.write_text(s)
