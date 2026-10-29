# Physics, driving, AI/path, checkpoint reference survey

## Result

**The sampled gameplay algorithms are overwhelmingly shared lineage, not a clean MAT3-versus-N64 winner.** Use MAT3 as the readable named/type-rich starting point and native matched N64 bodies as a serious native-platform triangulation reference. In this six-function sample: **5 semantic ties, 1 limited MAT3 preference**, with no demonstrated N64-exclusive algorithm advantage. The MAT3 preference is checkpoint sound-ID/call fidelity, not a claim that the entire checkpoint implementation is verified. N64 model field offsets are notably close to DC in these bodies; do not discard that evidence because names are less readable.

All six MAT3 counterparts and all six substantive native N64 counterparts were located (6/6 each); the N64 AdjustSpeed body is inlined into its MaxPathControls root, not an independent out-of-line body. Three DC functions have current report exact-code status, one has earlier target-disassembly identification, and two were inspected against target disassembly during this survey. Total sampled DC mapped code: **1,982 bytes**; current report exact spans: **734 bytes / 3 functions**. This is a purposive family sample, not a random estimate of whole-game closeness or source coverage.

## Pins and method

Survey performed read-only on 2026-10-27, without a new build, game-source edit or runtime execution. DC local workflow revision `8dee72f5752ca059a5fb91746cd34e40dcc7d0bd` (source baseline: public `5fa10cd187ace77de5f630185357a90d502d8bad`); MAT3 `1cfe284a1d6bbf66b2f8c52fc16c941a75961b2f`; N64 `09da2f064cf8c2443d5de60a2b2b67867360715e`.

DC paths below are repository-relative. `M/` abbreviates MAT3 `decomp/src/GameSource/src/`; `N/` abbreviates N64 `src/blob/`. Public pins and evidence limitations are in the [index](README.md). Report: `build/report.json`, generated 2026-10-26T20:52:31, target SHA256 `336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688`. SHA256 of the four current source files agrees with their report entries (`drivetra`, `drivsym`, `maxpath`, `checkpoint`). Report match status is saved build evidence, not a fresh rebuild. Current source is explicitly MAT3-derived, so textual similarity alone was NOT counted as proof of DC ancestry.

Target-supported structure comes from exact mapped spans in that report, independently inspected target-side disassembly with `python -B tools/fncmp.py UNIT FUNCTION`, and the earlier torque pilot. Raw disassembly is not included in this source-only research package. For whatslips and antispin the linked symbol extents include trailing pools whereas the mapped target function does not: fncmp's additional rows are not mismatches inside the report-exact code span. MaxPathControls has identical 94-word extents and zero differing words. No N64 rebuild/ROM re-verification was performed: N64 native-match provenance here is the checked-in source/group metadata, not a new scoring claim.

## Function table

| DC function / address / mapped bytes | MAT3 counterpart | Native N64 counterpart | Verdict / confidence | Evidence |
|---|---|---|---|---|
| whatslips / `8C022D94` / 334 | `M/drivetra.c:36` | `N/groups/physics_clutch/group.c:23`, `func_800E2AC4` | Tie / high algorithm confidence | DC report exact (`report.json:7489`). Both idle clamp ~104.719666, clutch threshold .8, gain 1.25, neutral/zero-clutch early exit, signed torque, single XOR crossing test, effective inertia and output torque. |
| enginetorque / `8C0230B0` / 290 | `M/drivetra.c:143` | `N/func_800E2D18.c:51` | Tie / high semantic, not byte-match | DC report differs 177 bytes (`:7549`); independent pilot identifies 1150/1149 RPM, throttle 14/13, row stride 12 shorts, clamping, three interpolations, tune/table scale. Both references contain precisely that structure. |
| antispin / `8C02393C` / 212 | `M/drivsym.c:175` | `N/func_800E1AA0.c:47` | Tie / high algorithm confidence | DC report exact (`:7714`): appearance bit 0x10; road types 8; drift+.5*brake capped 1; speed threshold 100; 12000 and 120 steering moment; reverse exit; yaw/lateral test; controller/tuning split. Both references agree. |
| MaxPathControls / `8C03A54C` / 188 | `M/maxpath.c:85` | `N/groups/maxpath_controls/group.c:550`, `func_800E4B58` | Tie / high semantic confidence | DC exact (`:13329`), fresh read-only fncmp zero differences. FindInterval → TargetSpeed → TargetSteerPos → AvoidAreas → gravity speed clamp/scale → AdjustSpeed → AdjustSteer; abort after time>1 and idx>10 or \|x/y\|>100. N64 inlines helpers; DC preserves calls. |
| AdjustSpeed / `8C03BC20` / 230 | `M/maxpath.c:484` | `N/groups/maxpath_controls/group.c:662`, `func_800E3984` inline helper | Tie / medium | DC source not exact (`:13399`), but target pools/call/control shape support same 1.05 target scaling, gravity/tyre40 gate, magnitude, .5 negative-error gain, .183 integrator, -.75 braking threshold, /4, clamps and output stores. Neither source establishes exact SH-4 expression association. |
| CheckCPs / `8C01BCF2` / 728 | `M/checkpoint.c:75` | `N/groups/car_checkpoints/car_checkpoints_func_800F8EC8.c:94`, `func_800F8EC8` | MAT3 narrowly / medium | Both mileage update, owner0/6 and crash/resurrection gates, plane-side crossing/radius, interpolated crossing time, maxpath+placement calls, 3-second leader logic. Independently decoded DC target at `8C01BF9E` loads sound ID `0x8e` and calls `PlayGameSound` (`8C011606`), matching MAT3; native N64 uses its sound wrapper with ID16. Entire DC function remains unmatched (`:5421`). |

## Concrete comparisons and cautions

### Clutch / FP association / layout

`src/drivetra.c:50-94` is target-exact after DC-specific FP spelling/order changes. MAT3 uses `dt * (engtorque * enginvmi)` and `dt * (enginvmi * (engtorque-clutchtorque))`; the DC source uses left-linear engine-torque/inverse-inertia then dt, like N64's native body. This is useful N64 shaping evidence, **not evidence that N64 unmodified C would match SHC**. The N64 0.8 subtraction/gain association differs too. The constants, branches and logical field roles tie overall. N64 model clutch at decimal972 (`0x3cc`), engine speed1032 (`0x408`), parameter/tuning pointers+0/+4 agree with DC field annotations `include/dc_types.h:162,176`. Never copy complete PPC/MIPS structs blindly.

### Torque

`pilot/lanes/integer/notes.md:11-21` documents independent DC target identification, shared literal pools, three signed16 interpolations, and a full-register return narrowed by the caller. Both donor declarations return short; that alone does not settle original DC declaration or narrowing. DC final target FP order is table-value*tune-scale followed by interpolation-result*product. MAT3's named engine_type/handling fields and N64's offset-indexed `unkC/unkB` represent the same two-dimensional scale lookup. N64 file has explicit native-code reasoning for compound-assignment and inline helper behavior; its usability extends beyond a names-only guess.

### Anti-spin

N64's header comment incorrectly says this is “N64-only” with no arcade ancestor found. The full body is a clear counterpart to MAT3/DC antispin. Its offsets +1548/+1552 road types, +1824 wheel, +2004 appearance, +1452 drift map directly onto DC roles. FP multiplication order differs across reconstructed donor bodies: the DC exact low-speed branch multiplies `(wheel*speed)*drift*120`; MAT3 and N64 associate differently. Thus the exact target proves common algorithm but not literal source identity.

### Path controller and speed

N64 `maxpath_controls/group.c:1-61` documents native inlining/IPA and openly says its reconstruction used GC/DC ports as reference; therefore it is not an independent original-source witness. `group.json` reports all ten group members EQUAL; this is stronger than a generic draft but not freshly revalidated here. Native model slot `+0x7c6`, wheel `+0x720`, brake `+0x728`, throttle `+0x72c` agree with DC annotations `dc_types.h:221-224`. N64 helper TargetSpeed accepts m+cp while MAT3 uses cp alone; on N64 it is inlined. DC is structurally closer to MAT3's out-of-line call organization, but compiler inlining alone does not justify declaring closer source lineage. AdjustSpeed target literal locations include `8C03BD14` (~1.05), `BD24` (.5), `BD28` (~.183), `BD2C` (-.75), `BD30` (.75), `BD34` (4). These support shared algorithm without asserting the whole unmatched function correct.

### Checkpoints

Both donors calculate `modelTime - frameSeconds * dot/(dot-lastDot)` after side change inside xz radius and then update last dot. The sound-ID difference is real platform/resource drift, verified directly in the DC target, not inferred from reconstructed DC C. MAT3's sound ID is preferable for DC here; N64 is still highly useful for the checkpoint geometry and branch semantics. Do not overgeneralize a single platform sound constant to all checkpoint logic.

### Misleading N64 symbols

N64 `symbol_addrs.us.txt:1309-1310` calls `800AB7D8` engine_torque_calc and `800ABA18` clutch_slip_calc. Those legacy labels are not sufficient identity evidence. The actual `N/groups/level_objects/group.c:167-184` engine_torque_calc body sets up metadata, textures and animation nodes, not drivetrain. Likewise the legacy MaxPathControls address `800A1000` is not the native matched controller above. Prefer actual native body + grouped-match provenance over convenient names or header declarations.

## Recommendation within this family

Keep MAT3 as semantic/type dictionary; consult N64 for offset-level model layouts, integer narrowing and alternate FP association. For the next unmatched DC torque/AdjustSpeed/CheckCPs work, compare the **two donors against target behavior first**, then test SHC expression shapes in isolated probes. Do not import N64 platform IDs, inlining/IPA calling conventions, big-endian struct assumptions or arbitrary legacy symbol names. This survey establishes no new matching credit and needs no build to support its bounded conclusions.

### Bounded next-use candidates (not authorization to reopen exhausted probes)

1. **CheckCPs (`8C01BCF2`, unmatched):** use the freshly target-confirmed MAT3 leader sound ID `0x8e`/PlayGameSound call as a fixed constraint while comparing N64's explicit plane-crossing/time sequence and native model fields. The new useful distinction is that N64's ID16 is not interchangeable despite the otherwise near-identical checkpoint algorithm. Do not change canonical code or run another shape search merely because this row exists; first compare existing failed-probe logs and only proceed if a concrete unexplored target discrepancy remains.
2. **AdjustSpeed (`8C03BC20`, unmatched):** consult the native N64 inline helper's explicit model input offsets (`+0x720/+0x728/+0x72c`) and separate magnitude/intermediate/error updates alongside MAT3's concise body. Fresh target pool reads establish the common constants but do not choose a winning expression tree. This is a diagnostic cross-check, not evidence that another unbounded reassociation campaign will help. If these shapes have already been exhausted, park it until target instruction/dataflow supplies a new differentiator.
