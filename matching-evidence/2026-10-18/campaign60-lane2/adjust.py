from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane2/worddiff.py');s=p.read_text().replace("unit,label,func=sys.argv[1:4]","unit,label,func=sys.argv[1:4]").replace('idx[:15]','(range(0,b-a,2) if len(sys.argv)>4 else idx[:15])');s=s.replace('sh4.decode(t,va).text','sh4.decode(t,va,B.C.load_target()).text');p.write_text(s)
