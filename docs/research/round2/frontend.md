# Round 2: frontend outcomes

Historical bounded experiments, not an instruction to apply unmatched source.
Accepted gains are separately documented in ../../round2-matching.md.
Private diagnostics and raw build artifacts are intentionally omitted.

## 31. updateGameOverScreen — partial

Unit: `gameoverscr`; target: `8C0291CC`. Reported accepted delta: 0 emitted bytes / 0 functions.

- Explicit reset !=0 guard changed scratch allocation, no strict gain.
- Approved legacy UnloadTrack() private declaration corrects four scratch differences; only two upstream displaced calls remain.

**Residual:** Needs capped setGameOverState layout fix; header-only proposal retained as research, source restored.

## 32. updateModeScreen — negative

Unit: `modescr`; target: `8C07DC08`. Reported accepted delta: 0 emitted bytes / 0 functions.

- MAT3 block-local fileh without extra player temporary: no gain, lost6 pool bytes; restored.
- Approved PS_PakChanged(int) argument ABI: target extu.b reproduced, but moves later exact functions; restored.

**Residual:** First mismatch frame8 vs12 at8C07DC18; target explicitly homes player and fileh separately. Need independent original local-lifetime evidence.

## 33. setGameOverState — negative

Unit: `gameoverscr`; target: `8C029314`. Reported accepted delta: 0 emitted bytes / 0 functions.

- MAT3 width=max/2 lifetime reuse with direct state access; neutral relative prior direct shape.
- MAT3 enum parameter/global typing instead of int; neutral.
- State pointer plus donor menuSize*lineHeight order and width reuse; no strict gain.

**Residual:** Baseline first mismatch 8C029324 frame12 versus target8. Direct donor uses frame8 but retains newState in r14 instead of homing it; target keeps state address in r14. Need genuine lifetime/alias evidence; no more probes this round.

## 34. animText — partial

Unit: `modescr`; target: `8C07DF0C`. Reported accepted delta: 72 emitted bytes / 0 functions.

- Single shared conditional Sprintf call: rejected layout regression.
- Target float constants0.34906585f/2.7925268f account for SHC truncation.
- Format local hoisted before singular/plural branch, as target; gains external pools.

**Residual:** Function still25 bytes differ, first8C07DF38 fr14 vsfr15. Three probes consumed. Strict72-byte external-pool improvement retained, not a function claim.

## 35. controlMain — partial

Unit: `gameoverscr`; target: `8C0295B2`. Reported accepted delta: 0 emitted bytes / 0 functions.

- Shared approved UnloadTrack() contract probe fixes both RemoveEnvirons callee register differences in this body.

**Residual:** Remaining six instruction differences are calls to displaced earlier helpers; no strict gain. Header change research-only pending setter layout.

## 36. drawMain — negative

Unit: `gameoverscr`; target: `8C029794`. Reported accepted delta: 0 emitted bytes / 0 functions.

- Donor literal menu-count bound removes reconstructed stack bound.
- Target rotated-loop shape via continue after invalid option.
- Target title position loads before text pointer.

**Residual:** No strict gain. Target retains4-byte frame absent donor literal/continue version; target entry also shifted by upstream setter. All rejected source restored.

## 37. updateButtons — exact

Unit: `modescr`; target: `8C07E556`. Reported accepted delta: 1104 emitted bytes / 1 functions.

- Donor for instead of reconstructed while: neutral.
- Continue after hidden button prevents unrolling and reproduces target initial branch/pool placement.
- Cursor XYZ stores rather than donor ZXY reproduce target instruction order.

**Residual:** None within updateButtons; fullTU remains partial at updateModeScreen/animText.

## 38. initButtons — exact

Unit: `modescr`; target: `8C07E248`. Reported accepted delta: 710 emitted bytes / 1 functions.

- Donor index<13 loop replaces invented pointer-end bound and fixes frame16->12.
- Approved private gNumModels canonical volatile unsigned-char contract fixes body; downstream updateButtons loop restores pools.

**Residual:** None within initButtons.

## 39. uninitScreen — exact

Unit: `modescr`; target: `8C07DEAC`. Reported accepted delta: 42 emitted bytes / 1 functions.

- WholeTU initButtons size correction resolves sole call displacement. No pointless local source mutation.

**Residual:** No local probe required: exact result established by supported upstream modifications.

## 40. uninitButtons — exact

Unit: `modescr`; target: `8C07E50E`. Reported accepted delta: 72 emitted bytes / 1 functions.

- WholeTU initButtons corrected entry; updateButtons continue shape restores its shared pool. No local mutation needed.

**Residual:** No local probe required: exact result established by supported upstream modifications.

