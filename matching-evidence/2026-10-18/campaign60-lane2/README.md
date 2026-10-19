# Campaign 60 lane 2: retained results

Ten assignments completed. Retained gains:28 exact functions and3,008 text/literal bytes, with no initialized-data delta. Only src/fmath.c and src/game.c differ from entry. All eight other owned source files were restored byte-for-byte. See progress.md for bounded blockers and manifest.json for source/object/ABS hashes and independent raw validation.

## fmath (+24 functions, +2,480 bytes)

invmagnitude originally copied vec[0],vec[1],vec[2] into scalar locals before computing the squared length. The target saves the vector pointer in R14 and schedules its loads within the expression. Removing those scalar locals and directly writing vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2] reproduces the44-byte function, corrects the shared literal pool, and restores all25 functions through U64WorldVector at their original addresses. Credit rises28?2,508; initialized data remains72 bytes.

Three subsequent NormalVector experiments were rejected: explicit component pointers, inline magnitude, and existing inline component-length helper. They preserved the25 matches but did not match NormalVector; the simple direct-invmagnitude candidate was restored and freshly rebuilt as selected. NormalVector requires an8-byte stack frame with cached +4/+8 component pointers; selected emits70 bytes versus86 target. No padding or synthetic stack objects were introduced.

## game (+4 functions, +528 bytes)

Target GameLoop rematerializes the gFrames address at each access; baseline cached its address in a callee-saved register, adding prologue/register and pool drift. Accessing gFrames through a volatile unsigned-int lvalue matches target code exactly. This restores GameLoop, OncePerFrame, func_8C0279EA and UpdateFrame at their original addresses. Initialized data remains163 bytes. The changes only qualify those two accesses inside GameLoop; no shared declarations were modified.

## Reproduction

From C:\rush2049-dreamcast:

```
python matching-evidence/2026-10-18/campaign60-lane2/run.py fmath selected
python matching-evidence/2026-10-18/campaign60-lane2/run.py game selected
python matching-evidence/2026-10-18/campaign60-lane2/validate.py
```

run.py calls dc_build.build_unit with BUILD/OBJ_DIR/LINK_DIR redirected to build/campaign60-lane2/<unit>/<label>. It never calls dc_build.main, split, objdiff or audit_report. It inherits the frozen native SHC5.1r11 configuration. No masks, assembler, target-byte fallback, game execution, full-image build, tool changes, downloads, commits or pushes were used.

validate.py rereads raw ABS outputs and independently compares every claimed exact function and every previously exact function/data/pool span at its actual address. All old exact spans survive. The builder excludes fill/gap/BSS credit. The private selected entries are fmath-selected.json and game-selected.json. Parent integration and global audit remain necessary before reporting overall totals.

One experiment failed with SHC rc1,2118 Prototype mismatch when a volatile parameter qualifier was initially applied only to the definition; applying it to the same source-file forward declaration permitted the experiment. It earned no strict improvement and was reverted. A write_stdin read of the initial baseline terminal encountered EXEC_CALLER_UNIDENTIFIED; its completed output files were read directly, without replaying the build command.
