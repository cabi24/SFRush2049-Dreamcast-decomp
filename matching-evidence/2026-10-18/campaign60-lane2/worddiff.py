import sys,os,json
sys.path.insert(0,os.path.abspath('tools'));sys.path.insert(0,os.path.abspath('pilot/tools'))
import dc_build as B,sh4
unit,label,func=sys.argv[1:4]
_,im=B.built_image('build/campaign60-lane2/'+unit+'/'+label+'/link/'+unit+'.abs')
u=next(x for x in B.C.load_map(None)['units'] if x['name']==unit)
f=next(x for x in u['functions'] if x['name']==func)
a,b=f['start'],f['end'];w=B.C.target_bytes(a,b)
idx=[i for i in range(0,b-a,2) if bytes(im.get(a+i+j,0) for j in range(2))!=w[i:i+2]]
print(func,hex(a),hex(b),'bad words',len(idx))
for k in sorted(set(q for i in (range(0,b-a,2) if len(sys.argv)>4 else idx[:15]) for q in range(max(0,i-2),min(b-a,i+4),2))):
 va=a+k;t=int.from_bytes(w[k:k+2],'little');v=sum(im.get(va+j,0)<<(8*j) for j in range(2));print('%08X %04X %-42s | %04X %s'%(va,t,sh4.decode(t,va,B.C.load_target()).text,v,sh4.decode(v,va).text))
