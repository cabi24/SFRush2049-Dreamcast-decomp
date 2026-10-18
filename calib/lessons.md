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

- Physical versus reckoned suspension: canonical `model.suscomp` is +0x5CC, while
  `model.reckon.suscomp` is +0x75C. Skids uses the latter. Shared model size0x808 and car size0x3CC
  are independently proved by indexed arrays, not by padding to the MAT3 sizes. Existing native
  matches survive the consolidated field definitions.
- DC fixed-memory arrays can require typed address macros to preserve separate base/field
  operations. Model's gGameCar at0x8C004000 and circuit's gPlayers at0x8C006000 provide target-backed
  examples; an extern symbol can fold the field into its pool address. These lessons do not
  authorize arbitrary address macros or restarting a capped source search.
- Internal literal islands in a differing mapped function remain uncredited under the frozen
  function-ownership policy (headlights). An explicitly mapped external pool can earn bytes on its
  own only when its full real-address range matches and raw emitted runs prove each credited byte.

- Initialized record padding earns no credit even when the compiler emits explicit zero bytes.
  Pane's sixteen tBlitSet records each contain two pad bytes at+A..B; all512 bytes still compare,
  but only480 earn data credit. Map data_padding records are evidence exclusions from credit,
  not masks: every byte remains part of fixed-address equality. Six focused credit controls pass.
- Source ownership includes a real DoDrones wrapper before its helper and excludes AnimateWeapon's
  generated epilogue from the source-function count. Caller targets, prologues and branches prove
  those corrections; the net total/game function denominators remain unchanged.
- SetInterruptMask is the SH4 SR helper at8C010080, proved by startup code and timer/controller
  pool references. The previous8C018000 mapping was incorrect. Correcting the address does not
  reopen the capped UpdateFrameTimer source-shape search.

- Physics wing rows are a slice: drivsym pools422C/4588 referenceBBE14, while the existing
  full-table view used by smoke isBBE07. The physics view is gPhysicsWingRows; its row0
  is the full table's row1. Keeping a qualified slice preserves the actual row arithmetic
  without redefining a global base from a folded operand.
- WPitchUV starts8C026524 and WRollUV starts8C0265D2, proved by their axis operations and
  option-screen callers. Full source bodies that match at displaced addresses earn zero;
  fmath retains only magnitude's28 bytes and72 data bytes at their actual target addresses.
- The inflater ends8C011290. A distinct farcsine body follows, and fasin/facos are tiny
  wrappers selecting argument0/1. The unbuilt asincos unit owns this region through13C0;
  one previously unmapped body increases source-function denominators by one, with zero
  byte credit from this boundary correction.
