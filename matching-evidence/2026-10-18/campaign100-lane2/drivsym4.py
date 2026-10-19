from pathlib import Path
p=Path('src/drivsym.c');s=p.read_text();s=s.replace('    poortract = 0;\n    if ((m->roadcode[2]', '    if ((m->roadcode[2]').replace('        poortract = 1;\n    }\n    dotireforce','        poortract = 1;\n    } else {\n        poortract = 0;\n    }\n    dotireforce');p.write_text(s)
