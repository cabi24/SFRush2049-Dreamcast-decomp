from pathlib import Path
p=Path('src/model.c');s=p.read_text();a=s.index('    {\n    float catchup_scale');s=s[:a]+s[a:].replace('    {\n    float catchup_scale', '    /* Target loads the shared scale into FR10 before the leader search. */\n    {\n    float catchup_scale',1);lines=s.splitlines();start=next(i for i,x in enumerate(lines) if x=='    float catchup_scale = 0.100000002f;');end=len(lines)-2
for i in range(start,end):lines[i]='    '+lines[i]
s='\n'.join(lines)+'\n';p.write_text(s)
