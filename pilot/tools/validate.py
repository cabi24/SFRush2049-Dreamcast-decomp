"""Run local analyzer/verifier controls, not a game build or target execution."""
from pathlib import Path
import hashlib,json,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent
def main():
 reports=[]
 commands=[('synthetic_verifier_controls',[sys.executable,'-B','-m','unittest','discover','-s','tools','-p','test_*.py','-v'],0),('decoder_controls',[sys.executable,'-B','tools/sh4.py','--selftest'],0),('evidence_checks',[sys.executable,'-B','tools/consolidate.py'],0),('compiler_dry_run_gate',[sys.executable,'-B','tools/probe.py','interp','--dry-run'],0)]
 for name,argv,expected in commands:
  result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=60)
  reports.append({'name':name,'argv':argv,'expected_returncode':expected,'actual_returncode':result.returncode,'passed':result.returncode==expected,'stdout':result.stdout,'stderr':result.stderr})
  if result.returncode!=expected:break
 syntax=[]
 for path in sorted(ROOT.rglob('*.py')):
  if '__pycache__' in path.parts:continue
  compile(path.read_text(encoding='utf-8-sig'),str(path),'exec')
  syntax.append({'path':str(path.relative_to(ROOT)),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
 report={'passed':len(reports)==len(commands) and all(x['passed'] for x in reports),'python_files_syntax_checked':len(syntax),'scripts':syntax,'commands':reports,'target_execution':False,'target_compilation':False,'match_credit':0}
 (ROOT/'analysis/validation.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
 print(json.dumps({'passed':report['passed'],'python_files_syntax_checked':len(syntax),'groups':[{'name':x['name'],'passed':x['passed']} for x in reports],'target_compilation':False,'match_credit':0},indent=2))
 return 0 if report['passed'] else 1
if __name__=='__main__':raise SystemExit(main())
