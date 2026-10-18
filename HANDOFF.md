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
| HAL_Support | 32 | 0 | 1/1 |
| HAL_time | 234 | 1 | 4/4 |
| Init | 342 | 12 | 3/7 |
| TB_Vector | 132 | 0 | 2/2 |
| TB_text | 492 | 329 | 10/21 |
| asincos | 64 | 16 | 0/3 |
| atan | 286 | 0 | 3/4 |
| audio | 580 | 4,857 | 13/30 |
| audiosfx | 58 | 0 | 0/30 |
| blit | 1,038 | 4 | 8/10 |
| bstree | 220 | 0 | 6/6 |
| camera | 108 | 1,642 | 1/23 |
| carload | 1,024 | 0 | 1/1 |
| cheatscr | 1,482 | 52 | 4/5 |
| checkpoint | 118 | 24 | 3/8 |
| circuit | 144 | 376 | 1/5 |
| controls | 1,190 | 104 | 5/11 |
| controlsys | 438 | 122 | 5/11 |
| countdown | 770 | 69 | 2/2 |
| demo | 842 | 45 | 7/7 |
| drivetra | 794 | 0 | 6/10 |
| drivsym | 244 | 44 | 1/17 |
| drones | 46 | 0 | 2/4 |
| fmath | 28 | 72 | 1/31 |
| front | 1,880 | 1,102 | 5/5 |
| gameoverscr | 122 | 7 | 1/16 |
| gfx | 576 | 88 | 8/10 |
| headlights | 308 | 172 | 1/4 |
| ll | 422 | 0 | 4/4 |
| mb_wad | 182 | 0 | 3/4 |
| mbox | 530 | 523 | 6/13 |
| model | 118 | 1 | 2/3 |
| options | 756 | 0 | 1/4 |
| optionscr | 166 | 2,092 | 1/11 |
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
| stree | 58 | 72 | 1/25 |
| stunt | 1,016 | 0 | 6/13 |
| targetpaths | 302 | 4 | 2/12 |
| targets | 0 | 525 | 0/32 |
| text | 450 | 7 | 3/3 |
| textureanim | 418 | 0 | 1/2 |
| timer | 0 | 16 | 0/9 |
| tracks | 1,116 | 407 | 3/3 |
| unzip | 26 | 362 | 1/11 |
| visuals | 44 | 600 | 0/60 |
| warningscr | 0 | 24 | 0/2 |
| weapons | 600 | 1,252 | 3/5 |
| world | 378 | 0 | 5/6 |
| worlddam | 500 | 80 | 2/3 |
| zip | 6,838 | 475 | 19/25 |
| zoidblit | 464 | 0 | 7/7 |
| zoidpoly | 848 | 0 | 7/8 |
| zoidtexture | 280 | 0 | 7/7 |
| zoidwindow | 2,666 | 224 | 15/15 |

Game text/literal credit: **40,376 /569,664 (7.0877%)**, plus **16,804 initialized data bytes**.
Whole-image credit: **57,180 /1,123,492 (5.0895%)**, **245 exact source functions**, 64 source units.
The seventh batch adds 1,914 text/literal bytes, 64
initialized data bytes and 8 exact functions. The audited batch is locally committed
through `7f641b2`. No remote push was performed.

`matching-evidence/2026-10-18/integration-seventh.json` records current source/header/compiler-input,
tool/output hashes and disjoint earned address spans. `python -B tools/audit_report.py` validates
saved outputs without compiling. No linker fill, gaps, alignment, explicit record padding or BSS
earns image credit. Full fixed-address equality remains required; no comparison masks are used.
Compiler, linker and flags remain frozen. Only units whose compiler input changes need a fresh
native link; byte-identical inputs may retain the already-verified native artifacts.

The audio/stree generated epilogues are part of their source functions. Current total/game
function denominators are 4,725/1,563. The two original gameover tail atoms and their
external pool belong to gamerecords; identities stay conservative pending mapping review.
Asincos selected03 retains16 C bytes and64 independently bounded external literal bytes;
no fourth variant was run. The original `_func_8C05145C` alias of InitSTree remains for world.
Four uncertain front zdepth high-halfword bytes remain conservatively excluded from credit.

Target-backed camera, full surface and audio fields preserve model0x808/car0x3CC strides.
Camera profile+0x48 remains distinct from circuit+0x3C. CarParts uses the proved0x60 stride.
Six private matching lanes continue on disjoint units; later private packets are excluded until
integration. MAT3 stays read-only. Validation uses separate fixed-address translation-unit links;
no full game image build or game execution was performed.

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
