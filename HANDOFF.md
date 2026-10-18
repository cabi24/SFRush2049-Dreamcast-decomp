# HANDOFF — Rush 2049 Dreamcast matching decompilation

Updated 2026-10-18. Read this first, then `BUILDING.md`, `map/README.md`, `include/README.md`,
`pilot/lanes/toolchain/calibration.md` (compiler facts) and `tools/README.md` (tool provenance).
`CODEX.md`, `overview.md` and `mat3-to-dreamcast-plan.md` are the earlier planning record.

## State

- Target: US v1.001 `1ST_READ.BIN` (SHA-256 `336c97aa…`), raw little-endian SH-4 at `0x8C010000`,
  1,123,492 bytes; local copy `pilot/orig/1ST_READ.BIN` (git-ignored; `retail-v1.001-evidence/verify_retail.py` re-extracts it).
- Compiler: Hitachi SHC 5.x (5.0r32–5.1r13 byte-identical on everything tested). **Frozen flags**
  `-comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis -string=const -speed -noinline`
  (`-inline=16` is an equivalent alternative so far). Never add decomp.me's `-aggressive=2 -extra=asm=1800 -pic=0`.
  Linker: H-series `LNK.EXE` 6.0E. Details and all compiler quirks in `pilot/lanes/toolchain/calibration.md`
  and `calib/drivetra/notes.md` (float literals truncate; `short / short-typed` → `__divws`; products written left to right; `-speed` loop shape; per-block address loads).
- Map: `map/units.json`, 154 contiguous units, 4,726 functions, 102 of 124 MAT3 GameSource files placed
  (`map/README.md` lists the 22 unplaced and the five weakest boundaries). Ghidra project `ghidra-project/RushDC.gpr`
  carries the same names (`tools/ghidra.ps1`).
- Build: `python tools/dc_split.py && python tools/dc_build.py` (strict, unmasked, at real addresses). Current:

| unit | status | bytes exact | functions |
|---|---|---|---|
| ll | exact | 422/424 (+2 pad) | 4/4 |
| drivetra | partial | 796/1620 | 6/10 (enginetorque residual shifts the rest) |
| timer | partial | 0/372 (D section 16/16 exact) | 8/9 match, UpdateFrameTimer 2 bytes short shifts the pool |

Game code 1,218 / 569,664 bytes exact (0.21%). Two parked residuals: enginetorque's inlined-interp
temporaries use different stack slots; UpdateFrameTimer re-materialises one global address.

## Resume

```
cd C:\rush2049-dreamcast
python tools\dc_split.py
python tools\dc_build.py            # or: python tools\dc_build.py --unit NAME -v
tools\objdiff\objdiff.exe           # open C:\rush2049-dreamcast\objdiff.json
```
Headless disassembly of any range: `.\tools\ghidra.ps1 -Headless ghidra-project RushDC -process 1ST_READ.BIN -noanalysis -scriptPath tools\ghidra_scripts -postScript DumpRanges.java out.txt 8c0xxxxx-8c0yyyyy`.

## Per-unit workflow

1. Pick a unit from `map/units.json` (kind `game`, with a MAT3 source); read its functions in Ghidra
   or a DumpRanges listing and the MAT3 source in `C:\rush2049-mat3\decomp\src\GameSource\src`.
2. Write `src/<unit>.c` using `include/dc_types.h` (absolute include path; extend the header with
   evidence comments as offsets are proven) and MAT3 bodies; add externals to `src/externs.txt`.
3. `python tools\dc_build.py --unit <unit> -v`, iterate on source shape only (flags are frozen);
   record non-obvious codegen lessons in `calib/drivetra/notes.md`-style notes or the calibration report.
4. Commit per unit. Never count masked or fuzzy matches; alignment padding is never counted.

## Rules

- `C:\rush2049-mat3` is read-only reference. Never commit game media, `pilot/orig`, downloaded tools, `build/`, `map/raw/`.
- Windows Smart App Control blocks `shc-v5.1r13` and r03 natively; `tools/shc_cc.py` falls back to WSL (`rocky` + wibo). Do not change security settings.
- From the Claude desktop app, Ghidra must run through `tools/ghidra.ps1` (settings outside AppData).
