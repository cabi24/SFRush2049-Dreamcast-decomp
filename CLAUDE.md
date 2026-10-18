# CLAUDE.md — Rush 2049 Dreamcast matching decompilation

Project: byte-matching decompilation of San Francisco Rush 2049 (Sega Dreamcast, US v1.001, Midway 2000).
Target `1ST_READ.BIN`, raw little-endian SH-4 at `0x8C010000`, 1,123,492 bytes,
SHA-256 `336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688`.
The GameCube port decomp at `C:\rush2049-mat3` (99.6% matched) is the semantic reference: names,
types, algorithms, source bodies. It is **read-only**: never edit, build or commit there.

**Start here:** `HANDOFF.md` (state, resume commands, per-unit workflow), then `BUILDING.md`,
`map/README.md`, `include/README.md`, `pilot/lanes/toolchain/calibration.md`, `calib/drivetra/notes.md`.

## Hard rules

- Never commit game media, `pilot/orig/`, `tools/downloads/` archives, `tools/shc|ghidra|jdk|wibo`,
  `build/`, `map/raw/` (all git-ignored). `tools/downloads/MANIFEST.json` records every tool's SHA-256 and source.
- **Compiler flags are frozen** (below). Match by changing source shape, never flags. Do not add
  decomp.me's `-aggressive=2 -extra=asm=1800 -pic=0`; the game was not built with them.
- Progress is strict: a unit's bytes count only when they are byte-equal at the real address after a
  real link. No masked, fuzzy or "moved" matches, no alignment padding, no copied original bytes.
- Per function: at most three evidence-backed source-shape changes or ~45 minutes, then park it with
  the first differing instruction recorded. Compiler/ABI doubts are lane-wide stops; a drifted
  function is an isolated stop.
- Never change Windows security settings. Smart App Control blocks `shc-v5.1r13` and r03 natively;
  `tools/shc_cc.py` falls back to WSL (`rocky` distro + wibo) by itself. The MAT3 CodeWarrior compiler
  is blocked the same way.
- Record every new codegen lesson (in `calib/drivetra/notes.md` or `calib/lessons.md`) and keep the
  table in `HANDOFF.md` current. Commit per unit with the bytes/functions exact in the message.

## Toolchain (all in `tools/`, provisioned 2026-10-17)

| Tool | Use |
|---|---|
| `shc/shc-v5.0r10 … 5.1r13` | Hitachi SH SERIES C/C++ Compiler packages (decomp.me archive); `shc.exe`, `asmsh.exe`, `LNK.EXE`, `elfcnv.exe` |
| `shc_cc.py` / `shc_cc.cmd` | compile one file the decomp.me way (CRLF, bin copied to cwd, `SHC_LIB=.`) → SYSROF `.obj` + ELF via `rof2elf/rof2elf.py`; `.cmd` is the frozen config |
| `shc_link.py` + `sysrof_abs.py` | fixed-address link with `LNK.EXE` (`START P/D/B`, `DEFINE` externals), reads the absolute SYSROF output, compares unmasked |
| `dc_split.py`, `dc_build.py`, `dc_objdiff.py`, `dc_common.py` | the project loop: map → target objects → compile/link/compare → `build/report.json`, `objdiff.json` |
| `objdiff/objdiff.exe`, `objdiff-cli.exe` | 3.8.2, SuperH backend; open `objdiff.json` |
| `ghidra/` + `jdk/` + `ghidra.ps1` | Ghidra 12.1.4, project `ghidra-project/RushDC.gpr` fully labeled from the map; headless via `ghidra.ps1 -Headless … -postScript DumpRanges.java out.txt start-end` |
| `calib/sh4dis.py`, `calib/unitcmp.py`, `calib/run_calib.py` | word-exact SH-4 decoder (capstone lacks FPU ops), per-function unit comparison, calibration driver |

## Frozen compiler configuration

`shc-v5.1r11` (5.0r32, 5.1r01, r03, r04, r08, r11, r13 are byte-identical on everything tested;
5.0r10/r28 differ; 5.0r26/r31 reject the flags). Flags:

```
-comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis -string=const -speed -noinline
```
`-inline=16` instead of `-noinline` (with no pragma) reproduces every unit so far equally; which one the
game used is undecided. `-noinline` + `#pragma inline(interp)` is what the committed sources use.
Linker: H SERIES LINKAGE EDITOR 6.0E from the r13 package.

## Compiler and layout findings (verified against the target)

1. **Pools are placed by the compiler per translation unit**, after unconditional branches/returns and
   merged across neighbouring functions (drivetra's pool for three functions sits inside `engine`).
   Compile whole units; single-function pool displacements are only comparable masked.
2. **Float literals are truncated, not rounded** (double→float toward zero): `9.549305f` → 0x4118C9F4
   but the target has 0x4118C9F3, so write `9.5493045f`; `0.925f` → use `0.92500002f`; `0.8f` → `0.80000002f`.
3. **Division helpers:** `short / short-typed` with a short result → `__divws`/`__modws` (16-bit,
   0x8C09B7DC/0x8C09B9AC); anything int-typed → `__divls`/`__modls` (0x8C09B680/0x8C09B82C). The game
   divides `throttle` by a short-typed 14 (`(signed short)14`) and `rpm` by int 1150.
4. **Float products are written left to right.** SHC rewrites `a * (b * c)` as `(a * b) * c`; the target
   never shows the rewritten form. `x * y` loads `y` first and multiplies into `x`'s register.
5. **`-speed`** gives top-tested loops (`tst; bt exit; body; tst; bf body`) and the exact sizes seen.
6. **Address loads happen per basic block**: the original re-reads `list->head`, `b4->prev`, `*prevh`
   before every use instead of caching them in locals (InsertLLE/RemoveLLE needed that shape); a store
   written in each `if/else` arm tail-merges into `bra; nop` + one delay-slot store.
7. **`-macsave=0`** (interp does not save MACL). `bsearch` saves it: it is SDK libc, not game code.
8. **Volatile set** for timer.c is MAT3's inferred one (frame counter, gSeconds, gFrameSeconds,
   gLastFrameCount, gLastFrameCounter); other sets are worse.
9. **Struct layouts recovered so far equal the GameCube ones** (tModelData, tTire 0x5C, tCar, tTune,
   tDLList, tDLLE). Treat MAT3 offsets as hints; prove each with a displacement or stride (rules in `include/README.md`).
10. **Headers:** C89 only; include with the absolute backslash path
    `#include "C:\rush2049-dreamcast\include\dc_types.h"` (SHC rejects `-` in `-include=` paths and
    forward slashes); `dc_build.py` pre-expands quoted includes.
11. **Data placement:** BSS and initialized data follow declaration order (timer.c D section:
    `gTimerPaused`, `gRetraceRate`, `gRetraceTime`, `sInited = 0`); a function-local static with an
    initializer lands in D. Section table in startup at 0x8C0A77B4: D = 0x8C0B8C20..0x8C0D2480, B = 0x8C1224C0..0x8C1A9014.
12. **Link order:** startup stub, timer.c, Dreamcast glue, unzip.c, then GameSource files in
    case-insensitive alphabetical order (atan.c … zoidwindow.c), then the Dreamcast platform group
    (Audio64 wrappers, paksysdc.c, Kamui renderer), front-end screens, late gameplay files, C runtime, Audio64.
13. **Source drift from MAT3 is real but small:** DC `bsearch` is a different (library) algorithm;
    `InitializeTimer` brackets itself with an interrupt-mask call and registers a retrace callback
    (counter at 0x8C12253C) in place of `sc.frameCount`; `engine`'s constant is 9.5493045 not 9.549305.
14. **SHC error 2118** rejects prototype/definition mismatches, so declaration tricks are not an option.

## Status (2026-10-18)

Game code exact: 1,218 / 569,664 bytes. Units: `ll` exact (422/424), `drivetra` 6/10 functions
(796/1620), `timer` 8/9 functions but 0/372 strict (one 2-byte residual shifts its pool).
Parked residuals: `enginetorque` (three inlined `interp` expansions put their temporaries in different
stack slots: target order b, low_ptr, rem, rindex·2, total) and `UpdateFrameTimer` (the `else` arm
re-materialises `&gFrameCounter` into r0 from the pool where SHC reuses r1). Both are allocator
behaviour, unchanged across releases, optimization modes and `-aggressive=2`; likely source-shape.

## Map (`map/units.json`)

154 contiguous units over the whole image; 4,726 functions; 102 of 124 MAT3 GameSource files placed
(905 high / 256 medium / 22 low confidence names). Weak spots listed in `map/README.md`: the Dreamcast
platform group 0x8C069E34..0x8C072820 (10 role-named units), the first 0x320 bytes, several small
units placed mainly by alphabetical position, a few edge functions that may belong to the neighbour,
and the SDK/data splits. Anchors verified: ll, drivetra, timer, BodyVector 0x8C026B00, bsearch 0x8C09BC04.
Regenerate with `map/scripts/` (see its README); Ghidra labels via `tools/ghidra_scripts/ApplyUnitMap.java`.

## Plan from here

1. Match units in the order small/integer first, then physics and FP, then screens; platform wrappers
   (Audio64/Kamui/GDFS/Maple) last and from interface contracts, not GC bodies.
2. Extend `include/dc_types.h` as offsets are proven; one definition per struct, evidence comments.
3. Resolve the two parked residuals when a sibling unit shows the same pattern (e.g. another
   `#pragma inline` user or another volatile-global function), not by flag search.
4. Decide `-inline=16` vs `-noinline` + pragma once a unit distinguishes them.
5. Later infrastructure gates: per-unit D/B/C section ranges and data symbols in the map so pool words
   link exactly everywhere; then full-image relink with original-byte fallbacks for unmatched units
   (never counted as progress).
