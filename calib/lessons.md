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

- SHC recognizes `_builtin_strcpy` and `_builtin_strcmp`; mbox verifies their target runtime ABI.
  Use the builtins only when target evidence agrees. No guessed ABI pragmas are needed.
- Zoidwindow nested conditions with literal2 let SHC hoist the constant into fr12 automatically;
  an explicit local picked other registers. A two-float `sxy` array reproduces projection stack
  accesses. All15 source functions now match at their real addresses.
- Bstree's `#pragma inline(ClearBST)` retains standalone ClearBST and reproduces InitBST.
  An early return in the insertion helper produces the count-store branch delay slot.
- DC zoidblit has eight-bit flags at +0x15 and hide at +0x16, differing from MAT3's field order
  and16-bit flags. The target-backed layout made all seven source functions exact on build1.
- The four-byte unreachable epilogue after objcmp's tail jump is generated code belonging to
  that comparator, not a separate source function. Its byte credit is retained after merging.
