# include/ — Dreamcast type layouts

`dc_types.h` holds the Dreamcast (SHC 5.x, SH-4 little-endian) layouts of the game structures that
matched or calibrating source uses. It is shared by every compiled unit; one definition per struct.

## Conventions

- **C89 for SHC**: block comments only, no `stdint.h`, no anonymous unions or `//`. The header must
  compile with the frozen configuration (`tools\shc_cc.py -v shc-v5.1r11 ... --no-default-flags --
  -speed -noinline`).
- **Including it**: units are compiled from a temporary work directory, so use the absolute path with
  backslashes: `#include "C:\rush2049-dreamcast\include\dc_types.h"`. SHC rejects the forward-slash
  form (it reports illegal character 0x2D and cannot open the file).
- **Names** follow MAT3 (`C:\rush2049-mat3\decomp\include\rush\rush_types.h`) where the DC evidence
  agrees: field names, signedness (`signed char`, `signed short`) and element types. Anonymous MAT3
  types get readable names: `tCar` (MAT3 `_tCar`), `tTune` (`anon_diffbl_weight_scale_2c`), `tTire`
  (`anon_tradius_springK_5c`), `tFastIn` (`anon_modelIteration_modelTime_10`).
- **Partial overlays**: each struct lists only fields proven on DC, at their proven offsets, with
  explicit `char padN[...]` gaps written as `END - START` of the hole. A struct ends at its last proven
  field unless its size is proven (an array stride, a `sizeof`-dependent computation).
- **Size self-checks** at the end of the header (`dc_check_*` typedefs, no code or data) fail the
  compile if padding arithmetic is wrong. Offset checks are not possible: SHC rejects `&((T *)0)->f`
  in array bounds.

## Evidence rules

A field may be added or moved only with evidence from the DC binary (`pilot\orig\1ST_READ.BIN`, base
0x8C010000), cited in the field's comment by target address:

1. a displacement in a load/store (`mov.b @(0xa,r14)`, `mov.l @(0x4,r4)`, `mov #0x14,r0` +
   `fmov.s @(r0,rN)`), or a `mov.w` literal-pool word holding the offset (cite the pool word's
   address);
2. an array base plus stride (e.g. `tires` at 0x430: 0x15C = 3*0x5C + 0x48 proves both the element
   size 0x5C and `angvel` at +0x48);
3. a compiled unit that reproduces the target bytes with the field at that offset (record the unit
   and the run label, e.g. `calib\drivetra\notes.md`).

MAT3 offsets are a hint, never evidence. So far every proven DC offset in tModelData, tCar, tTune and
tTire equals the MAT3 GameCube offset; when one differs, the DC value wins and the difference is noted
in the field comment.

The access width fixes the type (`mov.b` -> char, `mov.w` -> short, `fmov.s` -> float); signedness
comes from how the value is used (sign-extending loads are the default on SH-4, so a `tst`/`cmp`
pattern or `extu` decides).

## Filling the overlays in

When a new unit touches a struct:

1. derive offsets from that unit's listing (pool words and displacements);
2. split the existing `padN` gap around the new field, keeping the gap expressions as
   `0xNEXT - 0xPREV_END`; never renumber or re-type proven fields without new evidence;
3. if the new field lies past the current end of the struct, extend it and update the size check;
4. compile one existing unit (e.g. `sh calib\drivetra\run.sh <label>`) and confirm its `.text` is
   unchanged, then compile the new unit;
5. when a struct becomes complete (every byte proven or shown unused), replace the size check with
   the full size and say so in the struct comment.

Globals (`carEngineTorque`, `carTransGearRatio`, ...) are declared in the units that use them for now,
with their DC addresses from the pool words; a shared declarations header can follow once more than
one unit needs the same global.
