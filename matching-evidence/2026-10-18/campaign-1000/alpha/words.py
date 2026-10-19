exec(open(__file__.replace('words.py','run.py')).read().split('unit,label=sys.argv')[0])
sys.path.insert(0,str(ROOT/'calib'))
sys.path.insert(0,str(ROOT/'pilot/tools'))
import sh4
unit,label=sys.argv[1:3]
out=HERE/'runs'/unit/label
_,image=D.built_image(str(out/'link'/(unit+'.abs')))
entry=json.loads((out/'report.json').read_text())
for f in entry['functions']:
    if len(sys.argv)>3 and f['name']!=sys.argv[3]: continue
    if f['status']=='exact': continue
    print(f)
    lo,hi=int(f['start'],16),int(f['end'],16)
    for a in range(lo,hi,2):
        want=C.target_bytes(a,a+2); got=bytes(image.get(a+i,0) for i in range(2))
        if len(sys.argv)>4 or got!=want: print('%08X %s %-42s | %s %s'%(a,got.hex(),sh4.decode(int.from_bytes(got,'little'),a).text,want.hex(),sh4.decode(int.from_bytes(want,'little'),a).text))
