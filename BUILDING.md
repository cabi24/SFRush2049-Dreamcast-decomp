# Building and comparing units

The build system compiles each decompiled translation unit with the frozen compiler configuration,
links it at the unit's real address with the Hitachi linker, and compares the result with the
target `pilot\orig\1ST_READ.BIN` byte for byte. Everything runs from the project root, and every
output goes to `build\` (git-ignored).

```
python tools\dc_split.py                 # target side: build\target\<unit>.o + symbols.txt
python tools\dc_build.py                 # compile + link + compare every unit that has src\<unit>.c
python tools\dc_build.py --unit ll -v    # one unit, side-by-side listing of the first differing function
python tools\dc_build.py -u timer -v -f UpdateFrameTimer   # list a chosen function
python tools\dc_objdiff.py               # objdiff.json for tools\objdiff\objdiff.exe
```

`dc_build.py` runs the split first and refreshes `objdiff.json` afterwards, so usually it is the
only command you need. Run Python **without `-I`**: the `--verbose` listing imports
`calib\sh4dis.py`, which needs capstone from the user site.

## Inputs

| Path | What |
|---|---|
| `map\units.json` | The unit map (name, kind, text range, functions, pools, data/BSS ranges). Every tool takes `--map`; the default is `map\units.json`, or `map\units.seed.json` (three calibrated units) while the real map does not exist. |
| `src\<unit>.c` | One C file per unit, named after the map's unit name. A unit is built only if this file exists. |
| `src\externs.txt` | `symbol address` lines for externals that no mapped unit defines yet. They take precedence over the map. Names without a leading `_` get one. |
| `include\`, `src\include\` | Headers. Quoted `#include "x.h"` (also the absolute `C:\rush2049-dreamcast\include\dc_types.h` form) is expanded by `dc_build.py` into `build\pp\<unit>.c` before compiling, searching next to the including file, then `src\include\`, then `include\`. This is needed because SHC's `-include=` rejects any path containing `-` and the compiler runs on a temp copy of the source. |

## Outputs

| Path | What |
|---|---|
| `build\target\<unit>.o` | Target bytes of the unit's text range as an ELF32 LE SH relocatable (`.text` at offset 0, one sized FUNC symbol per mapped function using the linker name `_Name`, plus `.data`/`.bss` and OBJECT symbols when the map gives them). No relocations. |
| `build\target\symbols.txt` | `<linker symbol> 0x<address>` for every mapped function and data symbol. Used for LNK `DEFINE`. |
| `build\obj\<unit>.elf` / `.obj` | Compiler output: the rof2elf relocatable and the raw SYSROF object. |
| `build\link\<unit>.{abs,map,json}` | `shc_link.py` output: absolute SYSROF file, LNK map, and its own run-by-run comparison. |
| `build\obj\<unit>.o` | The **linked** unit as an ELF relocatable. This is the objdiff base side, so both sides hold absolute pool words. |
| `build\report.json` | Per-unit results and the progress summary (see below). Results are kept for units that were not rebuilt this run only if both the map and the unit's source are unchanged. |
| `objdiff.json` | objdiff project: `target_dir` `build/target`, `base_dir` `build/obj`, one unit per mapped unit. |

## How a unit is built

1. **Compile** with the frozen configuration and nothing else:
   `python tools\shc_cc.py -v shc-v5.1r11 src\<unit>.c -o build\obj\<unit>.elf --keep-obj build\obj\<unit>.obj --no-default-flags -- -speed -noinline`.
   `dc_build.py` has no option to change the version or flags.
2. **Link** with `tools\shc_link.py`: `START P` at the map's text start, and `START D/B/C` at the
   first map range of every data/BSS section the object has. Every symbol the object leaves
   undefined gets `DEFINE`d from `src\externs.txt`, then `build\target\symbols.txt`. Anything left
   over is reported as `unresolved` and the unit cannot be exact.
3. **Compare** the linked image with the target over the map's ranges. The text range is cut into
   functions (a function owns any pool inside it), pools, and gaps. Each data range is compared too,
   and each BSS extent is checked against the linked B section.

## What "exact" means

- A range is **exact** only if every byte in it was produced by this build and equals the target
  at the same address. Nothing is masked: pool displacements, pool contents, and branch offsets all
  count. Bytes inside a linked section that LNK leaves unwritten (alignment holes) are taken as
  `0x00`, the linker's fill. Bytes outside every linked section were not built and never match.
- **align**: at most 3 trailing bytes after a unit that the unit's own link does not produce (the
  next unit's section alignment), and that are zero in the target. They are listed separately and
  are **not** counted as exact. Example: `ll` is 422 exact of 424 bytes, with 2 align bytes.
- A function is `exact`, `differs`, or `missing`. **`moved`** means identical bytes that were linked
  at another address (an earlier function changed size). It is information only and is never
  counted.
- A **unit is `exact`** when every function, pool, gap, and data range is exact (align excepted),
  BSS extents match, there are no unresolved externals, and no built byte falls outside the unit's
  mapped ranges. Otherwise it is `partial` (built but not exact) or `failed` (compile or link
  error). `unbuilt` means the source exists but no current result is available.

## Progress metric

`report.json` → `summary`:

- `game_code`: exact text bytes of units with `kind: "game"`, divided by the text bytes of all game
  units in the map.
- `image`: exact text + data bytes of all units, divided by `image_size` (1,123,492).
- `functions`: exact functions over mapped functions. `map_coverage`: how much of the image the map
  describes.

Only bytes produced by the build count. Original bytes, masked comparisons, `moved` functions,
alignment padding, and objdiff's fuzzy match percentages are never counted. The denominators come
from the map, so the percentages describe only what the map covers until it covers the whole image.

## Rules

- Frozen flags only: SHC 5.1r11, `--no-default-flags -- -speed -noinline` (see
  `pilot\lanes\toolchain\calibration.md`). Source-shape changes are how a unit is matched; compiler
  options are not.
- No masking, no fuzzy matching, and no original bytes counted as progress.
- Never commit anything under `build\`, the target image, or compiler binaries.

## objdiff

`objdiff.json` lists every mapped unit. A unit with source gets `base_path` `build/obj/<unit>.o`
and `metadata.complete` is true only when `report.json` says the unit is exact. The GUI rebuilds a
unit with `python tools/dc_build.py --no-objdiff build/obj/<unit>.o` (`custom_make`/`custom_args`).
To check a unit from the command line:

```
tools\objdiff\objdiff-cli.exe diff -p C:\rush2049-dreamcast -u ll -o build\ll_diff.json --format json-pretty
```

objdiff's match percentages tolerate differences that the strict comparison does not (for example
pool displacements), so use them as a guide only. Static functions and variables are not exported
by SHC objects, so they are missing on the base side.

## Limitations

- Each unit is linked on its own. Inter-unit alignment padding is therefore never produced, and the
  whole image is not relinked as one program yet.
- Angle-bracket `#include <...>` (SHC's own headers) is not wired up.
- The map's function and pool boundaries decide how a unit is cut into ranges. Because each range
  counts only when all of it is exact, wrong boundaries can change both the per-function results and
  the exact byte count. Fix the map, not the comparison.
