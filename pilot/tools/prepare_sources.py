"""Separate the reviewed training scaffolds; never promote them as matching C."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parent.parent
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def function(text,signature):
 start=text.index(signature);opening=text.index('{',start);depth=0
 for i in range(opening,len(text)):
  depth+=(text[i]=='{')-(text[i]=='}')
  if depth==0:return text[start:i+1]+'\n'
 raise ValueError('unclosed function')
def main():
 cfgpath=ROOT/'config/target.json';cfg=json.loads(cfgpath.read_text(encoding='utf-8-sig'))
 cfg['working_copy']=r'C:\Users\cabi2\Documents\Codex\2026-10-17\task-20\dc-pilot\orig\1ST_READ.BIN'
 cfgpath.write_text(json.dumps(cfg,indent=2)+'\n',encoding='utf-8')
 lane=ROOT/'lanes/integer/candidates.c';text=lane.read_text();out=ROOT/'src/training';out.mkdir(parents=True,exist_ok=True)
 prefix='/* Training scaffold only: not equivalent-C, target-compiled, or byte-matched.\n * Derived from the pinned static lane; consult its mapping/ABI limits.\n */\n#include <stdint.h>\n#include <stddef.h>\n\n'
 parts={'bsearch':function(text,'void *dc_bsearch_candidate'),'interp':function(text,'int32_t dc_interp_raw32_candidate'),'InsertLLE':text[text.index('typedef struct DCNode'):text.index('/* MAT3 ll.c')]+function(text,'void dc_InsertLLE_candidate')}
 provenance=[]
 for name,body in parts.items():
  # The raw arithmetic C hypothesis is defined-range only; return width is open.
  extra='/* Defined signed32 arithmetic only. Original int/short return declaration is unresolved.\n * Low32 wrap, division by zero and INT_MIN/-1 need separate target semantics. */\n' if name=='interp' else ''
  if name=='bsearch':extra='/* Distinct DC algorithm; importing the MAT3 body would change zero-size behavior. */\n_Static_assert(sizeof(void *) == 4, "32-bit target pointers required");\n'
  p=out/(name+'.c');p.write_text(prefix+extra+body,encoding='utf-8')
  provenance.append({'path':str(p.relative_to(ROOT)),'sha256':sha(p),'origin':'lanes/integer/candidates.c','origin_sha256':sha(lane),'accepted_E':False})
 src=ROOT/'lanes/resource_float/BodyVector-candidate.c';p=out/'BodyVector.c';p.write_bytes(src.read_bytes())
 provenance.append({'path':str(p.relative_to(ROOT)),'sha256':sha(p),'origin':str(src.relative_to(ROOT)),'origin_sha256':sha(src),'accepted_E':False})
 (ROOT/'analysis/source-provenance.json').write_text(json.dumps(provenance,indent=2)+'\n',encoding='utf-8')
 (out/'README.md').write_text('''# Provisional training source

These four scaffolds make the static hypotheses reviewable. None is accepted equivalent C, emitted SH-4 code, or a byte match. The vintage compiler family, ABI and headers are unresolved. C11 assertions and standard integer headers are analysis conveniences; adapt them only after reviewing the acquired toolchain. Host compilation cannot settle target behavior.

- `bsearch.c` follows the DC library algorithm, which differs from MAT3.
- `InsertLLE.c` preserves the observed list offsets and branches; requires 32-bit pointers.
- `interp.c` covers defined signed32 arithmetic only. Raw32 versus signed16 source return type remains open.
- `BodyVector.c` deliberately leaves DC_FMAC unresolved. FPSCR/rounding, exceptional values and code generation require target evidence; source reloads preserve aliasing.

See `../../analysis/source-provenance.json` and the lane reports. No held-out C bodies were produced or tuned.
''',encoding='utf-8')
 (ROOT/'src/heldout/README.md').write_text('''# Held-out slots

enginetorque, UpdateFrameTimer and GetWaveByIndex retain their fixed held-out roles. Static identification was allowed; no compiler options or C bodies were tuned against them. The first two have identified spans; the getter remains unlocated. Compilation requires an explicitly frozen configuration, its external SHA256 pin, and --validate-heldout. Do not substitute another getter to improve the result.
''',encoding='utf-8')
 (ROOT/'orig/README.md').write_text('''The verified 1ST_READ.BIN extraction remains in task scratch, outside the saved C:\\rush2049-dreamcast\\pilot kit. config/target.json records its local working_copy path and expected SHA256. Tools prefer a local ignored orig/1ST_READ.BIN if one already exists, then that scratch path. No disc image or executable is included in the saved kit. Original media remains read-only.\n''',encoding='utf-8')
 print(json.dumps({'training_scaffolds':len(provenance),'source_promotions':0,'target_compilations':0}))
if __name__=='__main__':main()
