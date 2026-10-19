from pathlib import Path
p=Path('src/collision.c');s=Path('matching-evidence/2026-10-18/campaign100-lane2/collision.original.c').read_text();a=s.index('void ForceApart(');b=s.index('/* 0x8000bd54',a);part=s[a:b].replace('    float *cf, *of;\n','').replace('    cf = m->centerForce;\n','').replace('    of = other->centerForce;\n','')
for i in range(3):part=part.replace('cf[%d] += cent[%d];'%(i,i),'collision_add(&m->centerForce[%d], cent[%d]);'%(i,i)).replace('of[%d] -= cent[%d];'%(i,i),'collision_sub(&other->centerForce[%d], cent[%d]);'%(i,i))
helper='#pragma inline(collision_add)\n#pragma inline(collision_sub)\nstatic void collision_add(float *p, float v) { *p += v; }\nstatic void collision_sub(float *p, float v) { *p -= v; }\n'
s=s[:a]+part+s[b:];s=s.replace('#include "collision_types.h"','#include "collision_types.h"\n'+helper);p.write_text(s)
