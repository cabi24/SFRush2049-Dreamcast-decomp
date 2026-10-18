# Dreamcast matching lessons, 2026-10-18

These observations come from frozen SHC5.1r11 native unit links at original addresses. They are
source-shape evidence, not permission to change compiler options or force unrelated functions.

- In sky, declaration order of color versus XYZ temporaries and a target-backed volatile model
  count determine register allocation. The final three functions and seven initialized bytes match.
- In carload, unsigned car-index typing before adding 40 and argument product/load order in the
  first paint LoadWave call reproduce the target; the full function emits 1,024 nonpadding bytes.
- DC renderer layout differs from MAT3: zoidtexture's object instance stride is 0x58 and textureanim
  reads float UV values at +0x24/+0x28 and the strip word directly at +0x2C. Do not transplant GC offsets.
- Tracks omits GameCube endian conversions and cleanup paths. Target calls and loaded-state stores
  justify these changes; removing code solely to improve size is insufficient.
- Source-derived BSS extent agreement is weaker than independent proof of the original allocation.
  Keep unknown boundaries explicit and never count BSS as file-image progress.
- SHC emits pools inside function ranges, while the linker can leave zero-fill holes inside them.
  Equality of a whole range does not earn those holes. Raw ABS emitted addresses determine credit.
  This removed two inherited drivetra bytes and prevents inflated later progress.

Partial statll/atan/textureanim functions remain parked at their recorded limits. The original
enginetorque and UpdateFrameTimer residuals remain parked. See the receipts for exact source hashes.
