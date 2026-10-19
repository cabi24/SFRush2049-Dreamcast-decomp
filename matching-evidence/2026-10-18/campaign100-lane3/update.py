import json
from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane3/tasks.json');t=json.loads(p.read_text())
t[12][2]='bounded/prior-cap: fresh native baseline unchanged6-byte difference. campaign60 already exhausted direct-static,frame-temp,direct-store shapes. No new target/type evidence justifying replay; retained baseline. Residual call-register assignment r1/r2/r3 atA378..380.'
t[13][2]='bounded/prior-cap: fresh native baseline unchanged2-byte difference, GameLoop pointer r3 versus targetr1. Prior loop-result shape regressed. No new evidence to retry; baseline retained. Exact pool46 bytes preserved.'
t[14][2]='blocked/context: IntervalPos actual entry8C03B0A4 versus target8C03B120 (124 early); preceding capped IntervalPosChange112-byte short with pool/alignment context. Native target prologue inspected; no source-only placement adjustment justified.'
t[15][2]='blocked/context: TargetSpeed actual8C03B95C versus target8C03B9B4 (88 early); preceding IntervalPos/FindMaxpathIntervals unresolved. Native target first16 instructions inspected; existing exact first4 functions preserved. No masked or moved credit.'
t[16][2]='bounded/restored: init_cars baseline1226diff. New target-backed pointer caching shapes car1219,both1090,model1134. Target computes car/model bases before clearing model, candidate delays car;24-byte versus20-byte frame and pools remain different. No strict gain.'
p.write_text(json.dumps(t,indent=2))
