from pathlib import Path
p=Path('src/model.c');s=p.read_text();s=s[:-4]+'    }\n}\n' if s.endswith('}\n}\n') else s;p.write_text(s)
