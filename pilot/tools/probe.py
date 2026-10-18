"""One bounded, provenance-pinned compiler probe; no shell command construction.
No tools are acquired. Held-out compilation requires frozen configuration and explicit validation mode.
"""
from pathlib import Path
import argparse,hashlib,json,subprocess,time,sys
ROOT=Path(__file__).resolve().parent.parent

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('function');ap.add_argument('--config',type=Path,default=ROOT/'config/toolchain.json');ap.add_argument('--validate-heldout',action='store_true');ap.add_argument('--dry-run',action='store_true');args=ap.parse_args()
 target=json.loads((ROOT/'config/target.json').read_text());config=json.loads(args.config.read_text(encoding='utf-8-sig'))
 group='training' if args.function in target['training'] else 'heldout' if args.function in target['heldout'] else None
 if not group:raise ValueError('function is outside fixed seven-slot pilot')
 if group=='heldout' and (not args.validate_heldout or not config.get('frozen')):raise ValueError('held-out functions cannot tune an unfrozen compiler configuration')
 if group=='training' and args.validate_heldout:raise ValueError('validation switch is for held-out functions only')
 if not config.get('compiler') or not config.get('compile_argv'):
  print(json.dumps({'status':'blocked','function':args.function,'reason':'No reviewed SH-4 compiler/assembler/linker configuration is available. No compilation executed.'},indent=2));return 3
 program=Path(config['compiler']).resolve()
 if not program.is_file() or digest(program)!=config.get('compiler_sha256') or not config.get('provenance'):raise ValueError('compiler path/hash/provenance not pinned')
 source=ROOT/'src'/group/(args.function+'.c')
 if not source.exists():raise ValueError('candidate source absent')
 logpath=ROOT/'analysis/trials.json';logs=json.loads(logpath.read_text()) if logpath.exists() else []
 config_hash=digest(args.config)
 training_configs={x['config_sha256'] for x in logs if x['group']=='training'}
 if group=='training' and config_hash not in training_configs and len(training_configs)>=target['configuration_trial_cap']:raise ValueError('16-configuration cap reached')
 if group=='heldout':
  # The pin lives outside the configuration to avoid a self-referential hash.
  freeze=ROOT/'config/frozen-toolchain.sha256'
  if not freeze.is_file() or freeze.read_text().strip()!=config_hash:raise ValueError('external frozen configuration hash missing or mismatched')
 out=ROOT/'build'/group/args.function/(config_hash[:12]+'-'+str(len(logs)+1));obj=out/'candidate.o'
 if obj.exists():raise ValueError('output already exists; refusing stale object reuse')
 substitutions={'source':str(source),'object':str(obj),'root':str(ROOT),'out':str(out)}
 argv=[str(program)]+[str(x).format(**substitutions) for x in config['compile_argv']]
 if args.dry_run:
  print(json.dumps({'status':'configuration ready; dry run only','argv':argv,'compiler_executed':False},indent=2));return 0
 out.mkdir(parents=True,exist_ok=True)
 record={'function':args.function,'group':group,'config_sha256':config_hash,'compiler_sha256':digest(program),'source_sha256':digest(source),'argv':argv,'status':'pending compiler invocation; outcome requires inspection'}
 logs.append(record);logpath.write_text(json.dumps(logs,indent=2))
 started=time.time()
 try:result=subprocess.run(argv,cwd=out,capture_output=True,text=True,timeout=60)
 except (OSError,subprocess.TimeoutExpired) as exc:
  record.update({'status':'compiler launch/timeout failure','error':str(exc),'seconds':time.time()-started});logpath.write_text(json.dumps(logs,indent=2));raise
 (out/'compiler.stdout.txt').write_text(result.stdout);(out/'compiler.stderr.txt').write_text(result.stderr)
 record.update({'returncode':result.returncode,'seconds':time.time()-started,'object_sha256':digest(obj) if obj.exists() and result.returncode==0 else None,'status':'compiled-only; no byte-match promotion' if result.returncode==0 else 'compilation failed; no byte-match promotion'})
 logpath.write_text(json.dumps(logs,indent=2));print(json.dumps(record,indent=2));return result.returncode
if __name__=='__main__':raise SystemExit(main())
