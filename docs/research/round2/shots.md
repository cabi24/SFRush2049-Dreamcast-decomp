# Round 2: shots outcomes

Historical bounded experiments, not an instruction to apply unmatched source.
Accepted gains are separately documented in ../../round2-matching.md.
Private diagnostics and raw build artifacts are intentionally omitted.

## 11. ProximityDamage — negative

Unit: `shots`; target: `8C096550`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe1: target left-associated damage*y*y rather than damage*(y*y)
- probe3: give delta whole-function scope to test target stack-array lifetime

**Residual:** shots: ProximityDamage [differs, 455 bytes differ]  target 8C096550..8C096728 (0x1d8)  linked 8C096548 +0x1bc;   words: 236 linked / 236 target; 6 displacement-only, 173 differing; first diff at target 8C096568

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 12. ElevateGun — negative

Unit: `shots`; target: `8C096728`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe2: MAT3 algorithm with target-backed pitch/yaw views, positive one/negation and weapon stride
- probe5: explicit position assignments supported by target address temporaries and N64 source clue; rejected for selected source

**Residual:** shots: ElevateGun [differs, 582 bytes differ]  target 8C096728..8C096990 (0x268)  linked 8C096704 +0x28a;   words: 325 linked / 308 target; 7 displacement-only, 211 differing; first diff at target 8C096742

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 13. MissileLock — partial

Unit: `shots`; target: `8C096990`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe2: MAT3 algorithm using native sqrt builtin
- probe3: retain all three best-vector components as target96AAA..ABC proves, instead of scalar yaw/pitch

**Residual:** shots: MissileLock [differs, 177 bytes differ]  target 8C096990..8C096B54 (0x1c4)  linked 8C09698E +0x1c6;   words: 227 linked / 226 target; 11 displacement-only, 40 differing; first diff at target 8C0969A6

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 14. ShotToCarCollision — negative

Unit: `shots`; target: `8C096B54`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe2: MAT3 algorithm with target radius float at+10 and existing DC car/model layouts

**Residual:** shots: ShotToCarCollision [differs, 660 bytes differ]  target 8C096B54..8C096E8A (0x336)  linked 8C096B54 +0x34e;   words: 423 linked / 411 target; 3 displacement-only, 265 differing; first diff at target 8C096B6C

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 15. SonicCarCollision — negative

Unit: `shots`; target: `8C096E8A`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe2: target removes MAT3 wall-collision guard/store and reloads shot->obj per component
- probe3: target multiply association and binary32 0.6 literal

**Residual:** shots: SonicCarCollision [differs, 592 bytes differ]  target 8C096E8A..8C0970F6 (0x26c) + pool ..8C097110  linked 8C096EA2 +0x266;   words: 323 linked / 323 target; 8 displacement-only, 198 differing; first diff at target 8C096EA6; first non-displacement diff at target 8C096EC8

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 16. ShotInit — partial

Unit: `shots`; target: `8C097110`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe3: signed-byte conversion to reflect target no unsigned extension
- probe4: direct signed-byte view reflects native mov.b loads and removes conversion artifacts

**Residual:** shots: ShotInit [differs, 368 bytes differ]  target 8C097110..8C0972A6 (0x196)  linked 8C097108 +0x196;   words: 203 linked / 203 target; 0 displacement-only, 28 differing

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 17. ShotDone — partial

Unit: `shots`; target: `8C0972A6`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe1: target switch case order 3,1,8,0,2,4,6,7
- probe3: separate case-3 object local to test target r13/r14 lifetime

**Residual:** shots: ShotDone [differs, 77 bytes differ]  target 8C0972A6..8C097306 (0x60)  linked 8C09729E +0x60;   words: 48 linked / 48 target; 0 displacement-only, 7 differing

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 18. _CreateShotObject — partial

Unit: `shots`; target: `8C097306`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe1: literal NULL argument rather than local NULL matrix (target97332)

**Residual:** shots: _CreateShotObject [moved, 73 bytes differ]  target 8C097306..8C097352 (0x4c)  linked 8C0972FE +0x4c;   words: 38 linked / 38 target; 0 displacement-only, 0 differing

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 19. _UpdateShotObject — partial

Unit: `shots`; target: `8C097352`. Reported accepted delta: 0 emitted bytes / 0 functions.

- probe3: correct target binary32 0.4 and 0.18 pool constants
- probe4: safely above exact binary32 0.18 decimal to avoid SHC truncation

**Residual:** shots: _UpdateShotObject [moved, 200 bytes differ]  target 8C097352..8C09744A (0xf8)  linked 8C09734A +0xf8;   words: 124 linked / 124 target; 0 displacement-only, 0 differing

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

## 20. _InitNewShot — partial

Unit: `shots`; target: `8C09744A`. Reported accepted delta: 0 emitted bytes / 0 functions.

- research: whole-TU upstream closure and corrected shared pool; no direct body change because existing body already matches shifted target

**Residual:** shots: _InitNewShot [moved, 98 bytes differ]  target 8C09744A..8C0974C2 (0x78) + pool ..8C0974DC  linked 8C097442 +0x92;   words: 73 linked / 73 target; 0 displacement-only, 0 differing

**Resume only with:** Natural upstream source/pool closure plus residual register/stack lifetime explanation; no padding or shifted credit.

