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

- In dc_audio64_init, a dead `tst r0,r0` after a status call is the retained condition of an
  empty `if (func(...)) { }`, not a missing branch. Supplying it makes func_8C06AEB2 two bytes
  longer, which restores func_8C06ADD4's `mov.w` pool displacement and func_8C06AEC8 outright.
  In the same unit a redundant `unsigned int error` local forced the shared error pointer into
  r6; removing it so both guards read `->error` puts the pointer in r5 and the loaded value in
  r6, matching func_8C06B1C8 and func_8C06B22C. The variadic func_8C06B298 remains parked:
  hand-written alignment gives `tst #3,r0` and args in r5 where the target's compiler-generated
  va_start gives `mov #3,r6`/`tst r0,r6` and args in r6; a const local is folded away.

- `register` is honoured and changes allocation order. SHC allocates register-qualified locals
  before the others: one not live across a call takes the lowest free arg register
  (ZOID_SetObjectParent: `register signed short parent` -> r4 at 8C068BC8, the plain `object`
  then r5; without `register` the order is r5/r4). Register variables live across calls take
  callee-saved registers from r14 downward. Declaration order among several register variables
  did not decide the order.
- `x = x + v` and `x += v` can swap two long-lived registers with identical instructions:
  MBSetupWad's explicit form gives fixup r7 / loop pointer r6 (8C03CF98, 8C03CFBA); `+=` swaps them.
- `do {..} while (a && b)` keeps a just-decremented global in a register for `b`; separate
  `if (...) break;` statements re-read it per statement (extends the per-block reload finding):
  _RemoveObjectTree 8C068B86 `mov.l @r4,r3; cmp/pl r3` versus the cached `cmp/pl r5`.
- Inlined body shape matters independently of the standalone copy. In a `#pragma inline` function
  `if (cond) return; stmt;` and `if (!cond) stmt;` compile identically standalone but allocate the
  inlined temporaries differently (dc_blit BlitSetAttribute: r3 at 8C06B424/8C06B440 versus r1,
  which also shifted BlitXLU's pointer registers). An explicit local block is byte-identical to the
  pragma-inlined call, and an evidence-only `-inline=16` probe without the pragma was byte-identical
  to `-noinline` + pragma, so dc_blit does not distinguish the two policies either.
- Nested `else { if (c) return x; return y; }` versus a flat `else if` chain changes FP temporary
  allocation in the last arm with identical instruction sequences (fsatan 8C01143C: fr2/fr3/fr1
  versus fr0/fr1/fr3); a fully flat `if/if/return` chain instead moves the second compare's
  constant register (fr3 versus fr2).
- A named loop mask local (`unsigned int mask = 0x100 << window`) is allocated into the dead arg
  register r4 after the pointer copy where the inline expression takes a scratch
  (zoid func_8C068CC6 8C068CCE).
- Parked with first differing instruction: statll InitStatLL 8C051396 (ours `mov #0,r0` before
  `mov.l @(12,r4),r6`; target loads `list->list` first, then the shared zero; eleven neutral
  source variants). textureanim InitTextureAnimation 8C05B30A (rotated loop's entry copy of the
  `plist->name` test uses r2 for us, r3 in the target; the loop-back copy already matches).
  zoid _RemoveObjectTree 8C068AA0 (ours `sts macl,r11; add r13,r11` with `mov #-1,r12` in the
  delay slot; target `mov #-1,r12; sts macl,r11` with the add in the delay slot).
