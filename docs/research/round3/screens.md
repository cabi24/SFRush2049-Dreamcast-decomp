# Round 3 screens: 12 target outcomes

Accepted _ControlFileDel yields 374 emitted bytes and one function. Optional _FindFile adds a technically exact 36 bytes only with unsupported goto scaffolding in its nonmatching caller; explicitly withheld and excluded from every accepted total. Other near misses may regress already-accepted downstream functions.

Historical bounded research; accepted results are recorded separately in [the matching note](../../round3-matching.md). Private raw proof artifacts are not distributed. Associated build counts may share whole-TU builds and are not a unique compile count.

## 39. audioscr / updateAudioScreen

Target `8C016F00`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Target frame16 versus candidate12; stick aggregate representation and direction forms did not recover spill allocation. MAT3 lacks DC stick/control-mode path, so target is authority.

## 40. highscorescr / updateHighScoreScreen

Target `8C030ACC`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 1.

Direct local target branch to _SetHighScoreState cannot be reproduced by current external-only declaration/body gap; equivalent else shape neutral. One actual probe then precise structural blocker, not three pointless probes.

## 41. optionscr / updateOptionScreen

Target `8C041C34`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Ternary direction recovers target four-byte live spill and reduces513→9 differing bytes; remaining call scratch rotation at8C041C80 and moved downstream callees. Research rejected for prior credited-span loss.

## 42. videoscr / updateVideoScreen

Target `8C05C338`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Target edges allocation differs at8C05C35A; register edges worsens and local direction/if direction neutral-near-neutral. Three shapes exhausted.

## 43. pakscr / updatePakScreen

Target `8C07E9DE`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Explicit test neutral. Guard/goto shape has15 differences versus4 but changes successor scratch allocation to make _FindFile exact; optional review packet. Branch swapping worse; cap exhausted.

## 44. pakscr / _FindFile

Target `8C07EAF0`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Standalone explicit condition, combined loop condition, while form did not improve fixed-address equality. Incidental exact result from preceding updatePakScreen guard form recorded separately, not a fourth target probe.

## 45. pakscr / _SetState

Target `8C07EB14`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

If chain and default arm worsen; case0 goto shape neutral. Scratch rotation remains at8C07EB3A; cap exhausted.

## 46. pakscr / _ControlPickPak

Target `8C07EBE8`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Separate increment/decrement statements reduce44→7 differences; target reloads short globals after stores. Explicit mask and cleanup guard do not finish. Research only.

## 47. pakscr / _ControlFileDel

Target `8C07ED7C`; outcome: **exact**. Accepted row delta: 374 emitted bytes / 1 functions. Associated candidate builds: 1.

Separate four increment/decrement statements from short-global comparisons gives exact376-byte mapped span,374 emitted bytes. No prior credited address lost.

## 48. pakscr / _ControlFileDelVerify

Target `8C07EF10`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Explicit mask and no-else form neutral; explicit yes expressions worse. Residual shared-pool offset plus call scratch at8C07EF76.

## 49. pakscr / _GetPakStatusStr

Target `8C07EFC6`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Register qualification/inner scope/early return do not recover target prologue scheduling and no-frame shape; earliest divergence8C07EFC6.

## 50. startscr / updateStartScreen

Target `8C08B914`; outcome: **research**. Accepted row delta: 0 emitted bytes / 0 functions. Associated candidate builds: 3.

Separate inc/dec worse; explicit zero guards improve first scratch discrepancy but residual controller-loop scheduling and operand allocation remain, plus4-byte size drift.
