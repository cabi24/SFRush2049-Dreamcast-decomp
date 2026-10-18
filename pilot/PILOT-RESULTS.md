# Calibration update — 2026-10-18

The toolchain blocker is resolved and the compiler family is established: **Hitachi SHC 5.x**
(byte-identical output for 5.0r32 through 5.1r13 at the frozen flags). Frozen configuration:
`pilot/config/toolchain.json` (external pin `frozen-toolchain.sha256`), flags
`-cpu=sh4 -fpu=single -endian=little -macsave=0 -division=cpu -string=const -comment=nonest -sjis -speed -noinline`.
Full account: [lanes/toolchain/calibration.md](lanes/toolchain/calibration.md); trials in `../calib/trials.jsonl`.

| Slot | Set | Emission | Exact local link |
|---|---|---|---|
| bsearch | Training | parked: SDK library code (needs `-macsave=1`, foreign allocation) | — |
| InsertLLE | Training | **EXACT** 198 bytes (3 source-shape changes) | **yes**, inside the exact ll.c unit (422 bytes at 0x8C03A1B8) |
| interp | Training | **EXACT** inside the drivetra.c partial unit (pool placement reproduced) | pending enginetorque |
| BodyVector | Training | **EXACT** 94 bytes, MAT3 body verbatim | **yes**, at 0x8C026B00 |
| enginetorque | Held out | 170/290 masked bytes differ; prologue, both divisions (16-bit helpers), clamps, indexing, FP tail exact; inline temporaries' slots differ | no |
| UpdateFrameTimer | Held out | 2 instructions differ (one pool address re-materialised); the other 8 timer.c functions match, CountdownTimeOut exact, D section links exact | no |
| GetWaveByIndex | Held out | not located (unchanged) | — |

**Counters:** I=5/7 (+1 API counterpart); byte-exact emission 3/7 (InsertLLE, BodyVector, interp in its unit);
exact local links 2 (ll.c unit, BodyVector) plus timer.c's data section; held-out passes 0/3; flag configurations 22 (cap 16 exceeded by the inline-threshold sweep, see calibration.md).
No fixtures were run, so E remains unclaimed under the plan's definition. Expansion gates are not met.
The earlier text below is the pre-toolchain static pilot and is kept as written.

---

# First bounded pilot — 2026-10-18

The MAT3 bridge has produced useful Dreamcast identities and concrete platform/source drift. It has not yet produced matching C. The static mapping and strict comparison infrastructure are complete for this pass; target compilation is blocked by the missing reviewed SH-4 toolchain.

| Slot | Set | Dreamcast VA | Code bytes | Result |
|---|---|---|---:|---|
| bsearch | Training | 0x8C09BC04 | 118 | API identified by two callers; different DC library algorithm, parked for direct MAT3 source reuse |
| InsertLLE | Training | 0x8C03A1C8 | 198 | Strong semantic identity; all flag modes, graph writes, count and neighbor layout agree |
| interp | Training | 0x8C0231D2 | 20 | Arithmetic identity; full32 R0 result observed, original int/short declaration unresolved |
| BodyVector | Training | 0x8C026B00 | 94 | Strong row-transform identity; reverse FMUL/FMAC order and alias-sensitive reloads |
| enginetorque | Held out | 0x8C0230B0 | 290 | Strong identity from 1150/1149, throttle14, clamps, row stride and final scale; identification only |
| UpdateFrameTimer | Held out | 0x8C01010A | 108 | Strong identity; DC retrace callback counter replaces MAT3 scheduler field |
| GetWaveByIndex | Held out | — | — | Not located; a GC-added helper is plausible, not proven |

**Conservative counters:** I=5/7 accepted MAT3 semantic identities, plus 1 API counterpart with confirmed source drift. E=0/7, exact local links=0/7, emitted SH objects=0. Historical original-object identity cannot be measured. Five accepted code spans total 710 nonoverlapping bytes; drifted bsearch adds 118 separately. Shared literal dependencies total 60 bytes assigned once to timer/drivetrain groups. These are identification/dependency inventories, not recovered-C coverage. Strict earned source code/data bytes remain 0, and full-image source coverage is 0/1,123,492 bytes.

## Evidence that changes the implementation plan

`bsearch` uses unsigned one-based bounds and lacks MAT3's size-zero rejection/final fallback comparison. For a nonempty, zero-size search, DC can return base where MAT3 returns null. Both model-object lookup (72-byte entries) and world lookup (20-byte entries) support the API name. Compiler flags cannot repair that source difference.

`interp` calls a signed32 division helper at 0x8C09B680 using R1 numerator/R0 divisor. The same expression appears three times inline in torque, with signed16 intermediates. The standalone function has no EXTS.W and no located out-of-line callers. Do not impose MAT3's signed-short declaration without more DC evidence.

`BodyVector` processes each row from component2 to1 to0 with one FMUL and two FMACs, reloading source values after each output store. Static integer-exact fixtures produce [14,32,50]; the transposed WorldVector control gives [30,36,42]. When source equals destination, the observed reload sequence gives [14,84,797], rejecting hoisted inputs. These fixtures establish addressing/order for those values, not general SH-4 rounding or FPSCR behavior. DC_FMAC remains unresolved in the C scaffold.

The timer's DC frame counter is incremented by a local registered retrace callback. MAT3 uses sc.frameCount. The resource finder/loader establish 64 wave instances at 0x8C15FA20, stride20 and unsigned wave field+6; 23 aligned base/field literal-load references were examined without locating a standalone getter. The only MAT3 source caller is in NGC rendering code. Keep this slot unresolved instead of substituting another field access.

All six located code spans have verified SHA256s and no unknown opcodes under the focused decoder. Their identity scores are prioritization annotations, not probabilities. Boundary conclusions remain static and await independent analyzer/compiler corroboration. Narrow returns, arithmetic overflow, FP/FPSCR and the target linker layout remain open.

## Validation and limits

Fourteen focused synthetic strict-comparison/relocation controls and sixteen decoder controls pass. Negative controls reject changed code/literal/relocation bytes, missing coverage, overlaps, empty spans, PPC/wrong-endian ELF and ET_REL linked inputs. A synthetic SH ET_EXEC positive control passes. All 31 previously pinned MAT3 references still hash identically; the target executable hash is unchanged. [Validation log](analysis/validation.json), [reference recheck](analysis/reference-recheck.json), [span checks](analysis/span-checks.json).

No SH compiler invocation, emitted object, target link, held-out compilation/tuning, full build, watcher, install, game execution, MAT3 write, or publication occurred. No speedup comparison was timed, so the mapping results do not support a numerical productivity claim. The new decoder is partial and does not validate itself merely by decoding all words in a chosen span. Source/ABI and strict linked-byte acceptance remain separate gates.

## Blocker and next bounded step

**Not attempted / blocked:** compiling the four training scaffolds and linking fixed-address Dreamcast spans. Target: US v1.001 raw SH-4 image pinned above. Reason: no reviewed SH-4 compiler, assembler, linker, runtime/header package or known version/ABI configuration was located in the checked environment. The available signed Microsoft LLVM 19.1.5 registers ARM/AArch64/x86 targets only. No configuration trials were spent (0/16). The correct next input is the path to a lawfully held period Katana/Hitachi SHC package, or a separately provisioned official GNU SH compiler/Binutils calibration environment. A runtime/SDK banner and Hitachi ABI convention do not settle compiler identity: GCC 2.95.2 also supports -mhitachi.

**Denied / unchanged:** read-only `C:\Windows\system32\wsl.exe --list --verbose` returned exit1, `Wsl/E_ACCESSDENIED`. WSL distributions and any tools inside them remain unknown. This was a runtime denial, not an automatic approval-review rejection; it was not retried or worked around. All independent mapping/verifier work completed.

After tool provisioning, inventory/hash/help first, then establish argument/return widths, pointer/list layout, stack/callee-save and FP/helper contracts on training cases. Use no more than 16 justified configurations. Freeze the configuration with an external SHA256 before held-out compilation. Add the target-specific fixed-address mini-link and map checks once actual generated objects exist. Expand only after the original plan's nontrivial local-link and held-out gates pass. No full-game build is needed for that next step.
