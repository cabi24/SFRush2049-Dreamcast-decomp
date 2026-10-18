# Dreamcast Rush 2049 toolchain (provisioned 2026-10-17)

Everything in this folder was downloaded and verified on 2026-10-17 to unblock the plan in
[../mat3-to-dreamcast-plan.md](../mat3-to-dreamcast-plan.md) and the pilot in [../pilot](../pilot).
Exact SHA-256 hashes and source URLs for every archive are in `downloads/MANIFEST.json`.
`downloads/` keeps the original archives; the siblings are the extracted/working copies.

| Folder | What | Source |
|---|---|---|
| `shc/shc-v5.0r10 … shc-v5.1r13` | Hitachi SH SERIES C/C++ Compiler packages (11 releases): `shc.exe`, `asmsh.exe` assembler, `LNK.EXE` / `optlnksh.exe` linkers, `elfcnv.exe` (SYSROF to ELF/DWARF), `elf2bin.exe`, `LBR.EXE`, C++ headers | decomp.me compiler archive (`decompme/compilers` GitHub release `compilers`), the same packages decomp.me's Dreamcast platform uses |
| `rof2elf/rof2elf.py` | SYSROF object to ELF32 relocatable converter used by decomp.me so objdiff can read SHC output | gist by masonc15 pinned to the revision decomp.me's Dockerfiles fetch |
| `objdiff/objdiff-cli.exe`, `objdiff.exe` | objdiff 3.8.2 CLI and GUI; has the SuperH backend (MAT3's bundled 3.6.1 copy is untouched) | encounter/objdiff release v3.8.2 |
| `ghidra/ghidra_12.1.4_PUBLIC` | Ghidra 12.1.4 with the stock `SuperH4` processor (`SuperH4:LE:32:default`) | NSA GitHub release, SHA-256 matches the release page |
| `jdk/jdk-25.0.4.1+1` | Temurin JDK 25 (Ghidra 12.1.4 requires Java 25) | adoptium.net API, checksum verified |
| `wibo/wibo` | wibo 1.2.0 Linux x86_64 Win32 loader, used inside WSL for compiler builds Windows refuses to launch | decompals/wibo release 1.2.0 |
| `shc_cc.py` | Compile wrapper reproducing the decomp.me Dreamcast recipe end to end (see below) | written here |
| `ghidra.ps1` | Launcher that pins the JDK and keeps Ghidra's settings/cache under `../ghidra-home` | written here |

Python extras: `capstone` 5.0.x (`pip install capstone`) is installed in the user site and decodes SH4
little-endian (`CS_ARCH_SH`, `CS_MODE_SH4 | CS_MODE_LITTLE_ENDIAN`). Note that `python -I` ignores the
user site, so scripts that need capstone must run without `-I`.

## Compiling: the decomp.me Dreamcast recipe

decomp.me's cromper definition (`DREAMCAST_CC`) is, verbatim:

```
cat "$INPUT" | unix2dos > dos_src.c
cp -r ${COMPILER_DIR}/bin/* .
SHC_LIB=. SHC_TMP=. shc.exe dos_src.c -comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little \
        -macsave=0 -sjis -string=const ${COMPILER_FLAGS} -object=dos_src.obj
python3 rof2elf.py dos_src.obj ${OUTPUT} --isa=sh4
```

decomp.me's default `COMPILER_FLAGS` for Dreamcast scratches are `-aggressive=2 -extra=asm=1800 -pic=0`
(the `-aggressive=2` and `-fpu=single` options do not exist in shc-v5.0r10). Its UI exposes
`-round=zero|nearest`, `-fpu=single|double`, `-aggressive=2` and the common old-SHC flag set
(`-optimize=0|1`, `-speed`/`-size`, `-debug`, …; see `cromper/cromper/flags.py` in `decompme/decomp.me`).

`shc_cc.py` does all of that from Windows:

```
python tools\shc_cc.py -v shc-v5.1r11 src\interp.c -o build\interp.elf
python tools\shc_cc.py -v shc-v5.1r11 src\interp.c -o build\interp.elf -- -optimize=1 -speed
python tools\shc_cc.py -v shc-v5.1r13 src\interp.c -o build\interp.elf --launch wsl
```

Facts learned while bringing it up:

- **`SHC_LIB` must be set** or every version fails with `3321 (F) Illegal environment variable`.
  SHC 5.0 additionally insists on the decomp.me layout (binaries copied into the working directory,
  `SHC_LIB=.`); pointing `SHC_LIB` at the package `bin` directory only works for 5.1.
- **Smart App Control is ON** on this machine (`HKLM\SYSTEM\CurrentControlSet\Control\CI\Policy`
  `VerifiedAndReputablePolicyState = 1`). It blocks `shc-v5.1r13\bin\shc.exe` natively
  (`WinError 4551`, Code Integrity event 3077), while the other ten `shc.exe` builds and all of r13's
  other tools launch fine. The same policy also blocks MAT3's `decomp\build\compilers\GC\2.6\mwcceppc.exe`.
  Turning SAC off is a one-way user decision; this folder does not change it.
- **WSL + wibo works around the block**: the `rocky` distro (Rocky Linux 10.2, no python/gcc inside)
  runs `tools/wibo/wibo ./shc.exe` from `/mnt/c/...`. `shc_cc.py --launch auto` (default) falls back to
  it automatically on `WinError 4551`. shc-v5.1r03 reports `3021 (F) Memory overflow` when run natively
  because SAC blocks its `shcmdl.exe` sub-tool (Code Integrity log), so use `--launch wsl` for r03.
- **Smoke test (pilot slot `interp`, target VA `0x8C0231D2`, 20 bytes)**: shc-v5.0r32 and every 5.1
  release (r01, r03, r04, r08, r11, r13) with the decomp.me flags emit exactly the target's instruction
  sequence; the only byte that differs is the PC-relative displacement of the literal-pool load, which is
  fixed at link time (the target's pool entry lives at `0x8C023228`). shc-v5.0r10 emits a different
  schedule. This agrees with the pilot's note that the division helper takes R1/R0, the Hitachi runtime
  convention. One 20-byte function does not pin a release; it only confirms the family and the flag base.

## Analysis

- Ghidra project: `..\ghidra-project\RushDC.gpr`, program `1ST_READ.BIN` imported as raw
  `SuperH4:LE:32:default` at base `0x8C010000` and auto-analyzed headlessly. Open it with
  `.\tools\ghidra.ps1`. The launcher sets `JAVA_TOOL_OPTIONS` so Ghidra's settings/cache live in
  `..\ghidra-home`; without that, launching from inside the Claude desktop app fails with Felix's
  "The data file must be inside the data dir" because the app's MSIX container virtualizes AppData.
- objdiff: `tools\objdiff\objdiff-cli.exe diff -1 target.elf -2 base.elf -o out.json --format json-pretty`
  reads the rof2elf ELF objects (SuperH arch). The GUI `objdiff.exe` wants a project config; none exists yet.
- capstone for quick scripted disassembly of raw spans (see `shc_cc.py` and the pilot's `tools/sh4.py`).

## Linking and calibration (added 2026-10-18)

- `shc_link.py --obj x.obj --start P=8C03A1B8 --name x` runs `LNK.EXE` (H SERIES LINKAGE EDITOR 6.0E,
  r13 package, launches natively) with a generated subcommand file, reads the absolute SYSROF output
  with `sysrof_abs.py` (elfcnv 1.0G rejects it; rof2elf refuses ABS) and compares every located run
  against the target image unmasked. Reports land in `..\calib\link\<name>.json`.
- `shc_cc.cmd` is the frozen pilot configuration (SHC 5.1r11, decomp.me base flags without its
  `-aggressive=2 -extra=asm=1800 -pic=0` extras, plus `-speed -noinline`); `pilot\config\toolchain.json`
  points at it. `-extra=asm=1800` pads literal pools to 16 bytes and changes short-branch selection; the
  game was not built with it.
- `ghidra_scripts\DumpRanges.java` dumps complete SH4 listings for address ranges through
  `ghidra.ps1 -Headless ... -postScript DumpRanges.java out.txt start-end ...` (capstone 5 misses most
  FPU instructions: `fmov.s @(r0,rm)`, `float`, `ftrc`, `lds/sts fpul`).
- `shc_link.py --define SYM=HEXADDR` resolves undefined externals (LNK `DEFINE`), and `--start D=...`
  / `--start B=...` place data/BSS; the timer.c unit's D section links exactly this way.
- Compiler facts learned at unit level: float literals are truncated, not rounded (`9.549305f` gives
  0x4118C9F4, the target's 0x4118C9F3 needs e.g. `9.5493045f`); `short / short-typed` with a short
  result calls `__divws`/`__modws`, int-typed division calls `__divls`/`__modls`; `-inline=N` with
  N in about 9..31 reproduces the game's inlining choices as well as `-noinline` + `#pragma inline`.
- `..\calib\sh4dis.py` decodes the FPU/system instructions capstone 5 lacks; `..\calib\unitcmp.py`
  compares a compiled unit function by function.
- Results: see `..\pilot\lanes\toolchain\calibration.md` and `..\calib\README.md`.

## Not done / open

- Assembler (`asmsh.exe`) and inter-module optimizer (`optlnksh.exe`) untested; no section/BSS/data
  layout or multi-object link beyond one unit; no ELF ET_EXEC path for `pilot/tools/compare.py`.
- No GNU SH binutils/GCC (not needed for the SHC family; an independent decoder cross-check only).
- Literal pools are placed by the compiler across functions (drivetra.c's pool sits inside `engine`),
  so exact links must be done per translation unit.

## Build-and-compare system (dc_* tools, added 2026-10-18)

Repeatable per-unit build over the unit map; full description, outputs and rules in
[../BUILDING.md](../BUILDING.md).

- `dc_split.py` cuts the target into `build\target\<unit>.o` (ELF32 LE SH relocatables, sized FUNC
  symbols per mapped function, optional .data/.bss) and writes `build\target\symbols.txt`.
- `dc_build.py` compiles every unit with `src\<unit>.c` at the frozen configuration, links it with
  `shc_link.py` at the map's addresses (externals `DEFINE`d from `src\externs.txt` and
  `symbols.txt`), compares function by function with nothing masked, and writes
  `build\report.json` (`--unit`, `-v` side-by-side listing via `..\calib\sh4dis.py`, `--check`).
- `dc_objdiff.py` writes `..\objdiff.json` for `objdiff\objdiff.exe` (`objdiff-cli diff -p .. -u <unit>`).
- `dc_common.py` holds the shared map loader (`map\units.json`, falling back to
  `map\units.seed.json`), the ELF writer/reader, and the symbol-file formats.
- Found while building it: SHC's `-include=` rejects any path containing `-`, so `dc_build.py`
  expands quoted `#include`s itself (into `build\pp\`) instead of passing include paths.
