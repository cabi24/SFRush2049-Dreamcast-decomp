# SH-4 toolchain calibration — 2026-10-17/18

Status after the first execution pass with a real compiler: **the gameplay compiler family is Hitachi
SH SERIES C/C++ Compiler 5.x, three of the four training slots are byte-exact, one whole translation
unit (ll.c, 422 bytes, four functions) links exactly at its target address, and a configuration is
frozen.** bsearch is SDK library code. Both held-out slots are close but not exact and are parked with
recorded residuals. Expansion gates in the plan are not yet met (two exact local links, no held-out pass).

Everything below is reproducible from `C:\rush2049-dreamcast\calib` (`run_calib.py`, `trials.jsonl`,
`out/<label>/<version>/<slot>.txt` side-by-side listings) and `C:\rush2049-dreamcast\tools`
(`shc_cc.py`, `shc_link.py`, `sysrof_abs.py`, README with hashes and provenance).

## Tools and recipe

Compiler packages: shc-v5.0r10 … shc-v5.1r13 from the decomp.me archive (SHA-256 in
`tools/downloads/MANIFEST.json`). Invocation reproduces decomp.me's Dreamcast recipe (CRLF source,
package `bin/` copied into the work dir, `SHC_LIB=. SHC_TMP=.`), then `rof2elf.py` for an ELF
relocatable. Windows Smart App Control blocks shc-v5.1r13 and r03's `shcmdl.exe`; those run through
wibo in WSL (`--launch wsl`, automatic). Linker: `LNK.EXE` (H SERIES LINKAGE EDITOR 6.0E, from the
r13 package) driven by a subcommand file (`INPUT/OUTPUT/FORM A/START P(addr)/PRINT/EXIT`); the absolute
SYSROF output is read by `tools/sysrof_abs.py` because `elfcnv 1.0G` rejects it and `rof2elf.py`
refuses ABS files.

Comparison in `run_calib.py` is instruction-exact except the 8-bit displacement of
`mov.w/mov.l @(disp,pc)` literal loads, which is fixed by pool placement at link time. `shc_link.py`
compares linked bytes with no masking at all.

## Configuration search (13 distinct flag sets, 86 compiles, cap 16)

| Step | Flags (on top of `-comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis -string=const`) | Finding |
|---|---|---|
| 1 | decomp.me extras `-aggressive=2 -extra=asm=1800 -pic=0` | BodyVector EXACT, interp exact modulo pool, InsertLLE/bsearch differ |
| 2–6 | `-macsave=1` (+`-speed`/`-size`/`-optimize=0`, with/without extras) | bsearch's prologue saves MACL, but macsave=1 breaks interp → bsearch is not built with the game's flags (SDK libc) |
| 7–10 | `-speed`, `-size`, `-nospeed`, no extras | `-speed` gives the target's top-tested loop shape and exact size for InsertLLE; `-extra=asm=1800` (not `-aggressive=2`) causes the `bf; bra` entry form and 16-byte pool padding the target lacks |
| 11 | no extras + `-speed` | InsertLLE exact after source reshaping; identical output on all seven usable releases |
| 12 | no extras + `-speed -extra=asm=1800 -pic=0` | confirms `-extra=asm=1800` is the entry-branch culprit |
| 13 | **no extras + `-speed -noinline`** (frozen) | ll.c unit: InitLL must tail-call ClearLL, not inline it |

Version sweep at the frozen flags: shc-v5.0r32, 5.1r01, r03, r04, r08, r11, r13 are byte-identical on
every span tested; 5.0r10 and 5.0r28 differ; 5.0r26 and 5.0r31 reject the flag set. The exact release
is therefore a range, not a single version.

## Training slots

| Slot | Result | Source shape notes |
|---|---|---|
| interp | exact modulo pool displacement (20/20 instructions) | MAT3 body, `signed short` return; the target's pool entry sits at 0x8C023228 inside `engine`, a merged drivetra.c pool, so an exact link needs the whole TU |
| InsertLLE | **EXACT** (198 bytes) and **exact local link** inside the ll.c unit | 3 shape changes from MAT3: (1) no cached `prevh`/`prev` locals, re-read `b4->prev`, `list->head`, `list->tail`, `(*prevh)` each use; (2) ternary `prev` selection — rejected; (3) the store written in each `if/else` arm (target shows a tail-merged `bra; nop` into one delay-slot store) |
| BodyVector | **EXACT** (94 bytes) and **exact local link** at 0x8C026B00 | MAT3 body verbatim; SHC evaluates the textual sum right-to-left with FMUL + two FMAC, so no DC_FMAC primitive is needed |
| bsearch | parked: SDK library implementation | DC algorithm from the lane notes compiles to the same structure but with MAT3-foreign register/stack allocation; its MACL save needs `-macsave=1`, which the game code does not use |

ll.c as a unit (InitLL, ClearLL, InsertLLE, RemoveLLE; 0x8C03A1B8–0x8C03A35E): **EXACT, 422 bytes**,
linked with `LNK.EXE` at 0x8C03A1B8 and byte-compared unmasked (`calib/link/ll_tu.json`). RemoveLLE
needed the same shape as InsertLLE plus MAT3's operand order `element == list->head`.

## Held-out slots (compiled after the freeze; flags and version untouched)

| Slot | Result | Shapes tried | Residual |
|---|---|---|---|
| enginetorque | 170 masked bytes differ of 290; prologue, both divide/clamp sequences, torque-table indexing and the FP epilogue are exact | (1) `unsigned int throttle` — target has no `exts.w` and uses a different divide helper; (2) MAT3 `promoted[]` array verbatim — worse (real stack array); (3) `*(low_ptr+1)` pointer form — no change | the three `#pragma inline(interp)` expansions assign parameter temporaries to different stack slots/registers than the target; DC struct offsets established: tune +4 (torque_scale +8), handling +11, engine_type +12 |
| UpdateFrameTimer | 49 masked bytes differ of 108; every load/store and the FP tail match in order | (1) plain externs; (2) MAT3 volatile set — halves the diff; (3) MAT3 static/volatile file-scope declarations — same | target re-materializes global addresses from the pool per access where SHC here copies registers; likely needs the full timer.c unit (shared pool at 0x8C0101C0) |
| GetWaveByIndex | unlocated (unchanged) | — | — |

## Counters (pilot definitions)

- I identified: 5/7 (+ bsearch API counterpart), unchanged.
- Byte-exact emission: InsertLLE, BodyVector (2/7); interp exact except the link-time displacement.
- Exact local links (unmasked, fixed address, H-series linker): 2 — ll.c unit (contains InsertLLE) and BodyVector.
- Held-out passes: 0/3. Configuration trials: 13/16 flag sets.
- Earned bytes: ll.c unit 422 + BodyVector 94 = 516 bytes exact-linked; none integrated into a build.

## Unit-level pass — 2026-10-18 (continued in the same session)

Goal: resolve the two held-out residuals and interp's pool displacement by compiling whole translation
units. Tools added: `calib/sh4dis.py` (word-by-word decoder with the FPU ops capstone 5 lacks),
`calib/unitcmp.py` (per-function comparison of a unit by symbol), `tools/shc_link.py --define`
(LNK `DEFINE sym(addr)` for externals) and extra `--start` sections. Target listings came from the Ghidra
project (`calib/_ghidra_units.txt`, `_ghidra_helpers.txt`).

**Pool placement is the compiler's, and SHC reproduces it.** drivetra.c's merged pool for
enginetorque/interp/engine sits inside `engine` after its first unconditional branch (0x8C02321C); the
partial unit `calib/src3/drivetra_part.c` (enginetorque, interp, engine) puts it in the same place
unprompted, and interp is **EXACT inside the unit** (its displacement now resolves). `engine` matches
except for the 10-byte shift caused by enginetorque.

**timer.c unit** (`calib/src3/timer_tu.c`, 0x8C0100A4–0x8C01020E, nine functions): eight of nine
functions match modulo pool displacement, CountdownTimeOut is exact, and the D section (gTimerPaused,
gRetraceRate, gRetraceTime, sInited) links **EXACT** at 0x8C0B8C24. DC-specific facts recovered: a
retrace callback increments the counter at 0x8C12253C; InitializeTimer brackets itself with an
interrupt-mask call (15, restore) and registers the callback once with (0x900, callback, 0x58, 0);
`gSeconds = gFrameSeconds = 1.0f * gRetraceTime`; `static int sInited = 0` (D section); BSS order is
MAT3's declaration order with the counter and two unknown words first. The whole-unit link is not exact
only because UpdateFrameTimer is two bytes short, which shifts the shared pool.

**New compiler facts**
- SHC truncates float literals (double→float round-toward-zero): `9.549305f` → 0x4118C9F4 (what the
  GameCube binary holds) but the DC target holds 0x4118C9F3, so the DC literal lies in
  [9.549304008, 9.549304962); `9.5493045f` reproduces it.
- Division helper choice: `short / short-typed` with a short result → `__divws`/`__modws` (16-bit,
  0x8C09B7DC / 0x8C09B9AC); anything int-typed → `__divls`/`__modls` (0x8C09B680 / 0x8C09B82C).
  The target's `throttle / 14` uses the 16-bit pair, so the DC source divides by a short-typed 14
  (`throttle / (signed short)14` or a short variable); `rpm / 1150` is the 32-bit pair, as MAT3 has it.
  `throttle` is therefore `signed short` as in MAT3 (an earlier unsigned guess was wrong; engine's
  plain `ftrc` conversion confirms a signed prototype, and SHC rejects prototype/definition mismatches).
- `-inline=N` with N in roughly 9..31 inlines interp into enginetorque while leaving ClearLL out of
  InitLL, i.e. it explains both units without `#pragma inline`; `-noinline` + pragma is byte-identical
  for everything tested, so the two cannot be told apart yet. The frozen configuration keeps `-noinline`.
- SHC 5.0r28 differs on these units; 5.0r32–5.1r13 remain byte-identical, including on the residuals,
  so the residuals are not version-dependent. `-aggressive=2`, `-size`, default-speed: no effect on them.

**Residuals (parked)**
- enginetorque: 170 of 290 masked bytes; prologue, both divisions (now with the 16-bit helpers),
  clamps, table indexing and the FP tail are exact. The three inline expansions of interp assign their
  parameter temporaries to different stack slots/registers (target slot order b, low_ptr, rem,
  rindex·2, total; ours total, b, rem, left, rindex·2). Shapes tried: `promoted[]` verbatim (worse),
  pointer-arithmetic arguments (no change), unsigned throttle (wrong), short divisor (right, no effect
  on the slots). Stop rule reached.
- UpdateFrameTimer: 2 instructions (4 bytes, 50 masked bytes after the shift): in the `else` arm the
  target re-materialises `&gFrameCounter` into r0 from the pool while SHC here reuses r1 from the
  first statement. MAT3's volatile set is confirmed (three other sets are all worse); no flag or
  release changes it. Stop rule reached.

**Configuration count.** The trial log now holds 22 distinct flag sets (147 compiles). The plan's cap
of 16 was exceeded by the `-inline=N` sweep (6 values) and the `-aggressive=2` / default-speed /
`-size` checks on the units; each was a single-hypothesis test and none changed the frozen result.
No further flag permutations will be run; remaining work is source-shape or unit-context evidence.

## Validator note

`pilot/tools/validate.py` used to assert the blocked state (probe dry run exit 3). It now expects the
frozen state (exit 0); all four groups pass as of 2026-10-18. No other pilot tool was changed.

## Not verified / open

- No fixtures were executed (the plan's E state asks for behaviour tests); exactness is compiler evidence only.
- No assembler or inter-module optimizer (`optlnksh`) usage; no section/BSS/data layout work; literal-pool
  placement across functions is compiler-driven (drivetra.c's pool sits inside `engine`), so TU-level
  compiles are the unit for exact links from here on.
- `elfcnv.exe` cannot convert these absolute files; ELF ET_EXEC output for `pilot/tools/compare.py` is
  not available yet (`shc_link.py` compares directly instead).
- Next bounded step: compile drivetra.c and timer.c as whole units (downshift/enginetorque/interp/engine;
  the timer functions around 0x8C0100A4–0x8C0101C0) to resolve the two held-out residuals and earn a
  third exact local link with a shared pool. Expansion beyond the seven slots stays closed until then.
