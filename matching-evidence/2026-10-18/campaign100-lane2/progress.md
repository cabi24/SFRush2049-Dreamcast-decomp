# Campaign100 lane2

|Task|Unit|Target|Status|Attempts|Credit delta|Residual|
|---|---|---|---|---|---|---|
|1|camera|init_view3|blocked|2|0|init_view3 direct state reduces TU prefix drift8 to4 bytes but remains shifted2; signed state unchanged. No strict new spans; restored.|
|2|camera|SetCameraView|blocked|0|0|SetCameraView28-byte body moved-exact in direct-state trial, linked+4; baseline+8. Unresolved earlier prefix sizes. No new evidence permits replaying capped init_views or synthetic padding.|
|3|checkpoint|CheckCPs|blocked|1|0|CheckCPs frame76/2 saved floats versus68/1. Removed cached model/car/prev/link aliases worsens638 to657 differing bytes; restored all3 exact prefix functions.|
|4|checkpoint|PassedCP|blocked|0|0|PassedCP actual link8C01C018 versus target8C01C008. CheckCPs shared-pool size drift16 persists; fresh target inspected, no independent relocation/padding allowed.|
|5|circuit|UpdateCircuit|blocked|0|0|UpdateCircuit links8C01CDCC versus target8C01CDB8 (+20). Preceding capped circuit generator owns drift and shared pools; no new prefix evidence, no artificial repositioning.|
|6|circuit|GetCircuitCode|blocked|0|0|GetCircuitCode links8C01D0D8 versus target8C01D0C8 (+16). Inspected entry: existing code is displaced behind UpdateCircuit shared pool. Prefix correction prerequisite; no isolated-address credit.|
|7|collision|ForceApart|blocked|3|0|ForceApart target280 bytes, baseline264/136diff. Force initialization order243diff, direct centers254bytes/176diff, inline scalar add/sub266bytes/180diff. No strict gains; all reverted.|
|8|collision|MiniForceApart|blocked|1|0|MiniForceApart baseline links-16 from ForceApart. Fresh direct center-field shape fails; prefix and FP load scheduling remain unresolved. Restored baseline2 exact functions.|
|9|controlsys|UpdateControls|blocked|1|0|Direct PotRange assignment worsened UpdateControls344 to380 differing bytes and regressed previous exact spans; restored.|
|10|controlsys|ControllerUpdate|blocked|1|0|Field-base indexing motivated by target base+8 addressing worsened ControllerUpdate56 to664 differing bytes; restored.|
|11|controlscr|cleanupControlScreen|blocked|0|0|cleanupControlScreen body90 bytes but starts10 bytes early behind previously capped updateControlScreen. No fresh prefix evidence.|
|12|controlscr|validControl|blocked|0|0|validControl starts72 bytes late behind earlier controlscr functions; target starts with stack allocation while fixed address contains prior epilogue/literals. Prefix unresolved, no padding trial.|
|13|drivsym|MovingSurfaceForce|blocked|2|0|Target stack52 versus generated56 prompted reversed array order and single volatile index.624 unchanged then772 differing bytes; restored.|
|14|drivsym|forces1|blocked|3|0|Local traction1 hoisted target fr13 and reduced1063 to391 differences; explicit poortract else gave464; wind operand order reduced to62. No whole function/segment gain; restored at3-shape cap. Residual zero-store addressing and drag-expression scheduling.|
|15|fmath|CreateYPRMatrix|blocked|0|0|CreateYPRMatrix starts16 bytes early behind capped NormalVector; matching prologue appears16 bytes earlier. No fresh prefix evidence, no relocation credit.|
|16|fmath|ExtractYaw|blocked|0|0|ExtractYaw starts16 bytes early behind capped NormalVector, own body moved; no fresh prefix evidence and no padding attempted.|
|17|game|_UpdateGame|blocked|1|0|Target allSolid loop uses branch-to-bottom test; explicit labels reduced1265 to758 differing bytes but no strict credit gain. Restored; remaining literal/register scheduling and later loops unresolved.|
