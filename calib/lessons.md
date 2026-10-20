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

- Static functions are invisible to the comparison: SHC emits no symbol for a `static` function,
  so a map entry naming one reports "symbol not in the compiled object" and is silently
  uncredited. Declare map-named DC helpers non-static (drones `_AssignDrones`/`_LinearInterp`;
  `_LinearInterp` is content-exact but moved until `_AssignDrones` matches).
- Parameter spill slots are assigned by first use (ShowPakError: `*choice1 = 0` before
  `*choice2 = 0` gives choice1 @4 / choice2 @12 at 8C04416A; the reverse order swaps the slots
  while `done`, used first, stays @8).
- A retained empty `if (i < 3) { }` after a `for (i = 0; i < 3; i++) ... break;` search loop emits
  a dead `cmp/ge` (SetPakError 8C043F42); its two bytes shift the whole later register numbering
  (same family as the dc_audio64 empty-if lesson).
- Comparison operand order is visible in `cmp/eq Rm,Rn`: SetPakError `err == field` gives
  `cmp/eq r3,r9` at 8C043F78, `field == err` gives `cmp/eq r9,r3`.
- Statement forms that only renumber scratch registers: TB_Strcpy `do {} while ((*s1++ = *s2++) != 0)`
  puts the byte temp in r2 (8C05AB2A), the `while` form r3; `x == 0 && y() == 0` loads the callee
  address into r1 where `!x && !y()` uses r3 (func_8C03A3AE 8C03A3CE); an early-return guard versus
  an if-block swaps r2/r3 (_ControlPakErrNQA 8C04367E).
- Parked with first differing instruction: TB_GetTextNumLines 8C05A89C (frame -20 vs -16; SHC keeps
  a sign-extended copy of `ch` in r12 and spills `dbl`, the target re-extends `ch` per statement and
  keeps `dbl` in r8; the same ext caching shows in TB_DrawTextScrollBox, so it is systematic, not
  found in 20 source variants). TB_EllipseText 8C05A998 (frame -24 vs -28; target keeps `width` in
  r8 and `w`/`ew` as stack words). ShowPakError 8C0441AE after the spill-order fix (ours tail-merges
  the two _DrawChoice calls, the target has separate call sites at 8C0443DC/8C04440C and a 4-byte
  larger frame with a word spill of gText.tab[25]). func_8C03A360 8C03A378 (three temps numbered
  r2/r3/r1 versus r1/r2/r3; a non-void prototype for func_8C06B976 is an untested candidate).
  _AssignDrones 8C0255AC (`mov #0,r4` vs `mov r12,r5`, 18 bytes too long, not probed).

- Pool-flush rule and translation-unit boundary detection: at the end of function G, SHC emits the
  pending literal pool before the next function F iff (F_end - first_pending_reference) >= 0x146
  bytes (0x144 no flush, 0x146 flush; pool size, entry count and dead code do not matter), plus an
  unconditional flush at TU end. A target pool that violates the rule marks a TU boundary. The
  former dc_misc_8C06C08C unit is three TUs (boundaries 8C06C664 and 8C06CA98): pools at 8C06C642
  (measure 0x13E) and 8C06CA80 (0x142); splitting it took the range from 11/31 to 25/31 exact.
- Dead-result assignment suppresses a tail call: `int r; r = f(0);` gives `sts.l pr; bsr; lds.l pr;
  rts` where a plain call, `(void)f()` or `return f()` tail-calls with `bra`/`jmp` (func_8C06CA6A).
- Conversion assignments are not folded: `unsigned int unit = <int expr>` keeps a dead `mov rIV,r9`
  copy in an unrolled body (func_8C06C4A6) and a dead `mov.l r4,@r15` home write (func_8C06C58C);
  `signed short cl = camlink` is homed on the stack with a mov.w store and reload instead of
  `exts.w` (MBOX_NewObjectCam). Plain int copies fold away.
- Index loops over a one-element array strength-reduce to a pointer IV whose bound `base+44` is
  recomputed into a scratch register every iteration; pointer loops `p < base+1` hoist the bound
  into a callee-saved register (func_8C06C692/C986/CA44).
- A struct-typed local is addressed from r15 directly in calls and stores; an `int[8]` local used
  as a value across a call gets its address CSE'd into a callee-saved register (func_8C06C692).
- Parameter declaration order changes prologue scheduling of FP copies and constants without
  changing the ABI registers (func_8C06C2BE: `(int, float, float, int, int)`).
- `if (!cond) continue;` in a for loop yields `bra test; body; test; bf body` with a signed-char
  bound re-extended per iteration; a nested `if` gives the -speed top-tested shape with the
  extension hoisted (mbox search loops). `if ((ptr = f()) != 0)` tests r0 (`tst r0,r0; bf/s;
  mov r0,r13`); `ptr = f(); if (ptr)` tests the variable register (`mov r0,r13; tst r13,r13`).
- Parked with first differing instruction: func_8C06C524 8C06C544 (`mov.l 6DEC,r11` vs r10, port
  and address registers swapped), func_8C06C58C 8C06C596 (target loads `mov.w 1536,r6` before the
  index home store), func_8C06C712 8C06C785 (`jsr @r3` vs `jsr @r1`), func_8C06CAB4 8C06CAB6
  (pool temps r3/r2 vs r2/r1; its size also displaces func_8C06CA98/CAA8's pool), MBOX_FindObject_WORK
  8C03D3B5 (scratch rotation only), MBOX_NewObjectCam 8C03D0A8, MBOX_HideObject 8C03D0E6 (target
  interleaves the camlink copy with the pushes and branches into the body after the prologue).

- Explicit `== 0` / `!= 0` tests number the loaded operands differently from `!x || y` on a
  `return` expression (GetDefaultOption 8C04162C..38: r3/r0 then r2/r0 versus r2/r3, r1/r3).
- `volatile unsigned char` versus `volatile signed char` changes argument scheduling for an extern
  read as a call argument (InitSmoke 8C04F56C..72: unsigned sets r6/r5 before the stack push,
  signed pushes first); removing `volatile` rewrites the prologue. DC `gNumModels` is
  `volatile unsigned char` (textureanim and smoke agree).
- Independent integer statements in a loop body are emitted in source order (DecodeSlist 8C05176C:
  `next &= 0x1fff;` before `snum -= cnt + 1;` gives `sub` then `and`); DecodeSlist is content-exact
  and waits only on its neighbours' shared pool.
- Parked with first differing instruction: SetOptions 8C04190A (case-block address temps r1/r3/r1
  versus r3/r2/r1, recurring in every case; entry block matches), downleaf 8C05164E (two parameter
  sign-extensions numbered the other way), PlaceUnlockedCars 8C01BB72 (pure allocation order with no
  calls: target gives r14 to a model-pointer induction copy, ours to the gGameCar base; register,
  macro-address and declaration-order variants neutral or worse). CheckCPs/PassedCP/CarReportsGameOver
  prologues save fewer callee-saved registers in the target than our bodies need (not probed).
