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
| atan | 286 | 0 | 3/4 |
| blit | 1,038 | 4 | 8/10 |
| bstree | 220 | 0 | 6/6 |
| carload | 1,024 | 0 | 1/1 |
| checkpoint | 118 | 24 | 3/8 |
| circuit | 144 | 376 | 1/5 |
| controls | 1,190 | 104 | 5/11 |
| controlsys | 438 | 122 | 5/11 |
| countdown | 770 | 69 | 2/2 |
| demo | 842 | 45 | 7/7 |
| drivetra | 794 | 0 | 6/10 |
| drones | 46 | 0 | 2/4 |
| front | 1,880 | 1,106 | 5/5 |
| gfx | 576 | 88 | 8/10 |
| headlights | 308 | 172 | 1/4 |
| ll | 422 | 0 | 4/4 |
| mb_wad | 182 | 0 | 3/4 |
| mbox | 530 | 523 | 6/13 |
| model | 118 | 1 | 2/3 |
| options | 756 | 0 | 1/4 |
| pane | 1,082 | 480 | 3/5 |
| password | 884 | 305 | 5/6 |
| powerups | 1,580 | 8 | 8/8 |
| rekon | 770 | 0 | 4/4 |
| road | 18 | 0 | 0/3 |
| skids | 2,186 | 20 | 6/8 |
| sky | 458 | 7 | 3/3 |
| smoke | 92 | 154 | 2/6 |
| state | 290 | 12 | 5/6 |
| statll | 130 | 0 | 2/3 |
| streaks | 1,116 | 16 | 4/5 |
| text | 450 | 7 | 3/3 |
| textureanim | 418 | 0 | 1/2 |
| timer | 0 | 16 | 0/9 |
| tracks | 1,116 | 407 | 3/3 |
| warningscr | 0 | 24 | 0/2 |
| weapons | 600 | 1,252 | 3/5 |
| world | 378 | 0 | 5/6 |
| worlddam | 500 | 80 | 2/3 |
| zoidblit | 464 | 0 | 7/7 |
| zoidpoly | 848 | 0 | 7/8 |
| zoidtexture | 280 | 0 | 7/7 |
| zoidwindow | 2,666 | 224 | 15/15 |

Game text/literal credit: **28,008 /569,664 (4.9166%)**, plus **5,646 initialized data bytes**.
Whole-image credit: **33,654 /1,123,492 (2.9955%)**, **174 exact source functions**, 43 source units.
New integration contributes 32,422 image bytes and 164 functions relative to
the strict inherited baseline (1,216 text +16 data,10 functions). Two historical drivetra fill bytes
were removed. All linker fill, gaps, alignment and proven initialized record padding are excluded; BSS earns no image credit.

Per-unit commits and complete fourth-batch audit are recorded through `48eba24`.
`matching-evidence/2026-10-18/integration-fourth.json` records current source/header/compiler-input,
tool and output hashes plus disjoint earned address spans. `python -B tools/audit_report.py`
validates saved outputs without compiling. Six padding-credit tests pass.

Map reviews count bstree's six source functions, merge mbox's generated comparator epilogue,
and transfer ReadCheckpointData from password to pathcommon. The original final backing-array
ends in streaks, skids, smoke and circuit remain unclaimed. DoDrones is a separate source wrapper;
AnimateWeapon owns its generated epilogue. Pane data credit excludes32 explicit record-padding bytes.
The target-proven interrupt helper is8C010080; retained address aliases preserve existing dispatch calls. Total/game function denominators stay4,726/1,564.

Six private lanes continue on disjoint units. Later private packets are excluded until integration.
Capped functions remain parked without new evidence, including original enginetorque/UpdateFrameTimer.
Ghost, initiali and tires retain zero strict text credit privately. MAT3 stays read-only.
No full game image has been built; validation uses separate fixed-address translation-unit links.

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
