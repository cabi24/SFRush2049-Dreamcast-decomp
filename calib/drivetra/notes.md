# drivetra.c whole-unit compile (2026-10-18)

Target: `0x8C022CA4..0x8C0232F8` (0x654 bytes incl. literal pools), ten functions.
Source: `drivetra.c` (canonical); `drivetra_tu.c` is a byte copy that `run_calib.py` needs (slot
`drivetra_tu` -> `<dir>\drivetra_tu.c`). `run.sh <label> [flags]` copies, compiles and runs `unitcmp.py`.
Types: `C:\rush2049-dreamcast\include\dc_types.h` (included by absolute backslash path, see below).

Configuration (frozen, unchanged): shc-v5.1r11, base flags + `--no-default-flags -- -speed -noinline`,
`#pragma inline(interp)`. Final run label `dt-final3` (text identical to `dt-final2`); trial rows are
in `calib/trials.jsonl` under labels `dt-1`, `dt-2`, `dt-3`, `dt-3-inl16`,
`dt-3-nopragma-speed-inline16`, `dt-3-nopragma-speed-noinline`, `dt-final`, `dt-final2`, `dt-final3`.

## Per-function status (final source)

| Function | VA | Size | unitcmp (object) | Linked at 0x8C022CA4 | Shape changes |
|---|---|---|---|---|---|
| drivetrain | 8C022CA4 | 0xF0 | 1 byte: `bsr engine` displacement (8C022CB6) | 1 byte (same) | 0 (MAT3 verbatim) |
| whatslips | 8C022D94 | 0x168 incl. 2 pools | **EXACT** | **EXACT** | 2 (literal 0.8, product order) |
| autoshift | 8C022EFC | 0x9A | **EXACT** | **EXACT** | 1 (product order) |
| transmission | 8C022F96 | 0x56 incl. pool | relocation word only (8C022FE8) | **EXACT** | 0 (MAT3 verbatim) |
| find_best_gear | 8C022FEC | 0x5C | **EXACT** | **EXACT** | 3 (temp, `ratio = dwangvel*ratio`, `break`) |
| upshift | 8C023048 | 0x2C | **EXACT** | **EXACT** | 0 |
| downshift | 8C023074 | 0x3C + pool | relocation word only (8C0230AC) | **EXACT** | 0 |
| enginetorque | 8C0230B0 | 0x122 | **residual**, 170 masked bytes, 280 vs 290 bytes; first diff 8C0230C2 `mov.l r1,@(4,r15)` vs `mov.l r1,@r15` | differs | 4 scratch shapes, parked |
| interp | 8C0231D2 | 0x14 | **EXACT** | content exact, placed 10 bytes early | 0 |
| engine | 8C0231E6 | 0x112 incl. pool | pool-only: 3 `bsr enginetorque` displacements, relocation words, 2-byte pool alignment pad | shifted | 1 (literal 0.925) |

Linked image (`link/drivetra.json`, unmasked): 0x8C022CA4..0x8C0230BC is byte-equal to the target
except the single byte at 0x8C022CB6 (low byte of `bsr engine`, off by the 10-byte enginetorque
shortfall). That is whatslips, autoshift, transmission, find_best_gear, upshift and downshift with
both inter-function pools (0x8C022FD2 incl. `_carTransGearRatio` resolved to 0x8C0BBA48, and
0x8C0230A0), i.e. 1029 + 18 bytes exact. Everything from 0x8C0230BC on differs because enginetorque
is 10 bytes short. **The unit does not link exactly.**

Engine at matching pool alignment (label `dt-1`, where engine started on the target's mod-4
alignment): the only differences were the three `bsr enginetorque` displacements, the five
unresolved relocation words, and the 0.925 literal (now fixed). In the final unit engine starts
2 bytes off the target alignment (enginetorque is 10 bytes short), so the 4-byte pool alignment pad
moves; that is the whole of the 198 "masked bytes" unitcmp reports.

Pool placement is the compiler's and matches the target everywhere it can be checked: the mid-function
pool inside whatslips after `bra` (8C022DBC, shared by drivetrain and whatslips), whatslips' tail
pool (8C022EE2), the autoshift/transmission pool after transmission (8C022FD2), the
find_best_gear/upshift/downshift pool after downshift (8C0230A0), and the enginetorque/interp/engine
pool inside engine (8C02321C).

## Shape changes and evidence

SHC fact found here: **SHC flattens `a * (b * c)` into `(a * b) * c`** (float), keeping operand order
and common-subexpressioning the new left product. A value routed through a local is not flattened.
Probe: scratch `assoc.c`, `fact * (shiftk * up)` -> `fmul shiftk,fact; fmul up,...`, while
`shiftk * up * fact` -> `fmul up,shiftk; fmul fact,...`. The target never shows the flattened form,
so its products are left-linear in the DC source (MAT3's right-nested text is CodeWarrior-shaped).
For `x * y`, SHC loads `y` first and multiplies into `x`'s register (`fmul y,x`).

whatslips
1. `0.8f` -> `0.80000002f` (both uses). Target word 8C022DE4 = 0x3F4CCCCD; `0.8f` truncates to
   0x3F4CCCCC. Any literal in [0.80000001192, 0.80000007153) works.
2. `m->engangvel += m->dt * (m->engtorque * m->car->enginvmi)` ->
   `m->engangvel += m->engtorque * m->car->enginvmi * m->dt`; target 8C022E0C..34 loads enginvmi
   (pool 0x00A0 before 0x0404), `fmul enginvmi,engtorque`, then `fmac dt`. And
   `dv = m->dt * (m->car->enginvmi * (m->engtorque - m->clutchtorque))` ->
   `dv = (m->engtorque - m->clutchtorque) * m->car->enginvmi * m->dt`; target 8C022E70..88:
   `fsub`, `fmul enginvmi,diff`, `fmul dt,product`. MAT3 text gave `(dt*enginvmi)*diff`.
   Result: EXACT (both pools included).

autoshift
1. `fact * (m->tune->shiftk * m->car->upshiftangvel)` (and down) ->
   `m->tune->shiftk * m->car->upshiftangvel * fact`. Target 8C022F2A..40 computes `shiftk*up` and
   `shiftk*down` separately then `* fact` into fr15/fr14. This also reproduced the target's
   otherwise odd 4-byte frame and dead spill `mov.l r3,@r15` of `m->tune` (8C022F04, 8C022F1A),
   and the tail call `bra downshift` with the frame pop. Result: EXACT.

find_best_gear (three changes, at the cap)
1. Product moved to a local `ratio = ts * dwratio * gear_ratio` (target computes the three-factor
   product before touching dwangvel; MAT3 text flattened to `((dwangvel*ts)*dw)*gr`).
2. `ratio = m->dwangvel * ratio; if (ratio < usang)` - reproduces the target's
   `fmov fr5,fr3; fmov.s dwangvel,fr5; fmul fr3,fr5` (8C02302C..30). `if (m->dwangvel * ratio < usang)`
   keeps dwangvel in fr3 instead.
3. `return` -> `break`: with `return` the loop tail allocates the `gear++` temporary to r2 and the
   re-read to r3; the target (8C023036..40) has r3 then r2. Alternatives that did not change the
   swap: `++m->gear`, `m->gear += 1`, `m->gear = m->gear + 1`, a `while` loop.
   Result: EXACT.

engine
1. `0.925f` -> `0.92500002f`: target word 8C0232F4 = 0x3F6CCCCD; `0.925f` truncates to 0x3F6CCCCC.
   This was hidden in the partial-unit run (pool words compared only through the unit's pool shift).

enginetorque (parked; stop rule reached)
- Full-unit context does not change it: the emitted enginetorque bytes are identical to the
  partial-unit build (`out/drivetra-part-2`), 280 bytes vs 290, 170 masked bytes differ, first
  difference at 8C0230C2 (`mov.l r1,@(4,r15)` vs `mov.l r1,@r15`, the stack slot for the
  extended rpm across the first divide helper call).
- Scratch shapes (`et.c`, counts are differing instruction lines vs 88 for the current shape):
  A array-index arguments `torquecurve[tindex*12 + rindex (+1)]` 107; C explicit `high_ptr = low_ptr + 1`
  94 (gets the first stack slot and the `+2` pointer register right, but stores the wrong pointer);
  D `low_ptr = &torquecurve[...]` 98; E `low_ptr += 12` for the second row 142. That is one shape
  over the cap of three; parked.
- What the target shows (8C023122..8C023196): low_ptr is formed in r3, then `mov r3,r5; add #2,r5`
  (a separate pointer to element 1, never `@(2,rN)` displacement), and low_ptr itself is stored to a
  stack slot (slot 4 in row 1, slot 0 in row 2 where it is reloaded). Slot map: slot0 {rpm,
  b(row1), low_ptr(row2), right}, slot4 {low_ptr(row1), left}, slot8 {rrem, 14}, slot12 {rindex*2,
  1149(row2)}, slot16 {1149(row1)}. Ours: slot0 {1149, right}, slot4 {rpm, b}, slot8 {rrem},
  slot12 {left}, slot16 {rindex*2}. Lead for a later attempt: the pointer being stored like an inline
  parameter temporary suggests the DC source passes a pointer into an inlined helper (a second
  `#pragma inline` function taking `const short *`), which would also explain the `+2` pointer.

## Inline policy observation

`-speed -noinline` + `#pragma inline(interp)`, `-speed -inline=16` + pragma, and `-speed -inline=16`
with the pragma removed all produce identical `.text` for the whole unit (1608 bytes). `-inline=16`
inlines only interp (into enginetorque); it does not inline transmission (0x3C of code),
find_best_gear, upshift or downshift into their callers, which matches the target's `bsr` calls.
Without the pragma under `-noinline`, interp is called (text 1580), which is wrong. The unit still
cannot tell the two policies apart. Every result above uses `-noinline` + pragma.

## Recovered offsets (all agree with MAT3's GameCube layout)

| Struct.field | Offset | Evidence |
|---|---|---|
| tModelData.car | 0x000 | `mov.l @r4,r3` before car fields (8C022DEE, 8C022F1E, 8C022FC2) |
| tModelData.tune | 0x004 | `mov.l @(0x4,r4)` (8C022E42, 8C022FA0, 8C023002) |
| tModelData.trans | 0x009 (s8) | `mov.b @(0x9,r4)` (8C022F96, 8C02300A) |
| tModelData.autotrans | 0x00A (s8) | `mov.b @(0xa,r14)` (8C022CAC) |
| tModelData.handling / engine_type | 0x00B / 0x00C | enginetorque (partial unit, unchanged) |
| tModelData.tireForce[4][3] | 0x064 | `add #0x64` then +0x28 = [3][1], +0x1C = [2][1] (8C022CDE..E4); pool 0x0080, 0x008C |
| tModelData.torque[4] | 0x3B4 | pool 0x03BC = torque[2] (8C022DC8), `add #4` -> torque[3] |
| tModelData.clutch | 0x3CC | pool 8C022DD6 |
| tModelData.throttle | 0x3D0 | pool 8C022FD2 |
| tModelData.gear (s16) | 0x3F4 | pools 8C022EE4, 8C022FD8, 8C0230A0, `mov.w @(r0,r4)` |
| tModelData.commandgear (s16) | 0x3F6 | pool 8C022FD6 |
| tModelData.engtorque | 0x404 | pool 8C022EEA |
| tModelData.engangvel | 0x408 | pools 8C022DD4, 8C022EF0, 8C022FDA |
| tModelData.torquecurve | 0x40C | engine (partial unit) |
| tModelData.dwtorque | 0x410 | pool 8C022DC4 |
| tModelData.dwinvmi | 0x414 | pool 8C022EF4 |
| tModelData.efdwinvmi | 0x418 | pools 8C022DCC, 8C022EF6 |
| tModelData.dwangvel | 0x41C | pools 8C022DC0, 8C0230A4 |
| tModelData.clutchtorque | 0x420 | pool 8C022EE6 |
| tModelData.clutchangvel | 0x424 | pool 8C022EF2 |
| tModelData.transratio | 0x428 | pool 8C022FDC |
| tModelData.totalratio | 0x42C | pools 8C022DD2, 8C022EEE |
| tModelData.tires[4] | 0x430 | pool 8C022DBC used as array base |
| tTire size | 0x5C | 0x58C-0x530 (angvel), 0x558-0x4FC (invmi) |
| tTire.invmi | 0x14 | pools 8C022DCE (0x4FC), 8C022DD0 (0x558) |
| tTire.angvel | 0x48 | 8C022CC4..CCA: 0x15C = 3*0x5C+0x48, 0x100 = 2*0x5C+0x48 |
| tModelData.magicdiff (s8) | 0x5A8 | pool 8C022DC2, `mov.b @(r0,r14)` |
| tModelData.roadcode[4] | 0x60C | engine (partial unit) |
| tModelData.dt | 0x634 | pool 8C022EEC |
| tModelData.crashflag (s8) | 0x640 | engine (partial unit) |
| tModelData.shifttime | 0x658 | pool 8C0230A6 |
| tModelData.fastin.modelTime | 0x714 (fastin 0x710 + 4) | pool 8C0230A8 |
| tCar.enginvmi | 0xA0 | pool 8C022EE8 |
| tCar.dwratio | 0xA4 | pools 8C022FDE, 8C0230A2 |
| tCar.clutchmaxt | 0xA8 | pool 8C022EE2 |
| tCar.upshiftangvel | 0xAC | pool 8C022FD4 |
| tCar.downshiftangvel | 0xB0 | 8C022F28 `add #4` from 0xAC |
| tTune.torque_scale | 0x08 | enginetorque `mov #8` |
| tTune.trans_scale | 0x0C | `mov #0xc` (8C022FB6, 8C023006) |
| tTune.clutchmaxtk | 0x10 | `mov #0x10` (8C022E44) |
| tTune.shiftk | 0x14 | `mov #0x14` (8C022F16) |
| carTransGearRatio | 0x8C0BBA48, float[][6] | pool words 8C022FE8, 8C0230AC; index `trans*24 + (gear+1)*4` |
| carEngineTorque | 0x8C0BBC4C, float[][3] | pool word 8C023238 |
| float literals | 0.5, 500, 104.719666, 0.8(=0x3F4CCCCD), 1.25, 3.0, 0.25, 128, 1.5, 0.925(=0x3F6CCCCD), 9.5493045 | pools 8C022DD8..E4, 8C022EF8, 8C022FE0..E4, 8C0232EC..F4, 8C02323C |

## Practical notes

- SHC cannot open `#include "C:/rush2049-dreamcast/..."` (forward slashes: it reports an illegal
  character 0x2D and mangles the path); `#include "C:\rush2049-dreamcast\include\dc_types.h"` works.
  Sources are compiled from a temp work dir, so relative includes do not resolve.
- `&((T *)0)->f` is rejected in array bounds ("Illegal array size"), so dc_types.h checks sizes only.
- Link command (run from `C:\rush2049-dreamcast`):
  `python tools\shc_link.py --obj calib\out\dt-final3\shc-v5.1r11\drivetra_tu.obj --start P=8C022CA4
  --define _carEngineTorque=8C0BBC4C --define _carTransGearRatio=8C0BBA48 --define __divls=8C09B680
  --define __modls=8C09B82C --define __divws=8C09B7DC --define __modws=8C09B9AC --name drivetra
  --out-dir calib\drivetra\link`. LNK refuses a start address that is not 4-aligned (error 312), so a
  shifted link cannot be used to test interp/engine at their own addresses.
