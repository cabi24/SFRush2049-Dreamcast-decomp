# HANDOFF â€” Rush 2049 Dreamcast matching decompilation

Updated 2026-10-18. Read this first, then `BUILDING.md`, `map/README.md`, `include/README.md`,
`pilot/lanes/toolchain/calibration.md` (compiler facts) and `tools/README.md` (tool provenance).
`CODEX.md`, `overview.md` and `mat3-to-dreamcast-plan.md` are the earlier planning record.

## State

- Target: US v1.001 `1ST_READ.BIN` (SHA-256 `336c97aaâ€¦`), raw little-endian SH-4 at `0x8C010000`,
  1,123,492 bytes; local copy `pilot/orig/1ST_READ.BIN` (git-ignored; `retail-v1.001-evidence/verify_retail.py` re-extracts it).
- Compiler: Hitachi SHC 5.x (5.0r32â€“5.1r13 byte-identical on everything tested). **Frozen flags**
  `-comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis -string=const -speed -noinline`
  (`-inline=16` is an equivalent alternative so far). Never add decomp.me's `-aggressive=2 -extra=asm=1800 -pic=0`.
  Linker: H-series `LNK.EXE` 6.0E. Details and all compiler quirks in `pilot/lanes/toolchain/calibration.md`
  and `calib/drivetra/notes.md` (float literals truncate; `short / short-typed` â†’ `__divws`; products written left to right; `-speed` loop shape; per-block address loads).
- Map: `map/units.json`, 154 contiguous units, 4,726 functions, 102 of 124 MAT3 GameSource files placed
  (`map/README.md` lists the 22 unplaced and the five weakest boundaries). Ghidra project `ghidra-project/RushDC.gpr`
  carries the same names (`tools/ghidra.ps1`).
- Build: `python tools/dc_split.py && python tools/dc_build.py` (strict, unmasked, at real addresses). Current:

| Unit | Exact text/literals | Initialized data | Exact functions |
|---|---:|---:|---:|
| ll | 422 | 0 | 4/4 |
| drivetra | 794 | 0 | 6/10 |
| timer | 0 | 16 | 0/9 strict |
| statll | 130 | 0 | 2/3 |
| atan | 286 | 0 | 3/4 |
| sky | 458 | 7 | 3/3 |
| countdown | 770 | 69 | 2/2 |
| textureanim | 418 | 0 | 1/2 |
| carload | 1,024 | 0 | 1/1 |
| zoidtexture | 280 | 0 | 7/7 |
| tracks | 1,116 | 407 | 3/3 |
| demo | 842 | 45 | 7/7 |

Game text/literal credit is **6,540 / 569,664 (1.1480%)**, plus 544 initialized data bytes.
Whole-image credit is **7,084 / 1,123,492 (0.6305%)**, with 39 exact functions. All padding,
including internal linker fill and string alignment, is excluded. Historical drivetra 796 is now
794; this corrects accounting without changing generated code. BSS earns no image credit.

The first nine new units were committed individually through `1db7611`; shared layouts and the
credit fix are in `48f6ed2`. Evidence: `matching-evidence/2026-10-18/integration-first9.json`.
`python -B tools/audit_report.py` validates saved links, source hashes and disjoint earned spans;
`python -B tools/test_progress_credit.py` exercises the accounting rules (five tests pass).

The six-worker matching wave is ongoing. Canonical totals above exclude later private packets.
Each lane owns one unit and writes private source/header/map proposals; the root alone integrates
canonical shared files and replenishes completed assignments. Compiler flags remain frozen;
per function stop after three evidence-backed shape changes or about 45 minutes.

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
- `tools/shc_cc.py` is native-only. On a policy denial, stop that action and report the exact command/reason. Use normal reviewed approval; never change security settings or use a WSL/wibo fallback.
- From the Claude desktop app, Ghidra must run through `tools/ghidra.ps1` (settings outside AppData).
