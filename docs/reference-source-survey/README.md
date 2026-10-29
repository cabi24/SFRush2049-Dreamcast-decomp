# Reference-source survey: MAT3 and N64 for Dreamcast

Survey date: **2026-10-27**. This is research documentation, not a matching-source change.

## Recommendation

**Use MAT3 as the default semantic/type reference, N64 for native-platform
triangulation, and the Dreamcast target to decide.** MAT3's readable names and
recovered types are useful even where both references contain the same algorithm.
N64 contributes model offsets, integer narrowing, alternative expression structure
and counterexamples to MAT3-only assumptions. Neither is a drop-in Dreamcast
implementation or an independent pristine original-source witness.

The survey covers **20 target-backed cases**, plus one unresolved texture-upload
candidate excluded from the findings. This deliberately selected sample does not
support whole-game winner percentages or claims about historical ancestry.

| Family | Reading route | Strongest finding |
|---|---|---|
| [Physics, AI and checkpoints](physics-ai.md) | MAT3 plus N64 | Five semantic ties and one narrow MAT3 preference: target checkpoint sound ID `0x8e`, not N64 ID 16. N64 model offsets and arithmetic structure remain useful. |
| [Battle, records, save and state](gameplay-state.md) | MAT3 first, with exceptions | Four practical MAT3 preferences, one narrow N64 loop-shape preference, three ties. FindWinner tie reset and SaveInit policy favor MAT3; InitRecords' `12×3×5` loops favor N64 structure. |
| [Camera, HUD, resources and platform](render-platform.md) | MAT3 for shared code; target first for native backends | Three MAT3 preferences, two ties, one neither. Synchronous LoadWave, HUD teardown and viewport reset favor MAT3; neither donor supplies native Audio64 setup. Texture upload remains unresolved. |

These are reference-selection judgments, not donor build scores. A shared algorithm
can still differ in layout, platform IDs, synchronization and compiler output.

## What was checked—and what was not

- Existing DC exact mapped spans were reviewed against a saved build report and
  current source hashes. The gameplay review also rechecked three linked spans
  directly against the target. Nonmatching functions support only their specifically
  inspected branches, constants and dataflow, not complete-body correctness.
- Survey evidence used `build/report.json` generated `2026-10-26T20:52:31` and the
  paired `build/linux-strict-receipt.json`. These are local generated evidence paths,
  **not files supplied by this research package**. Report line references in the
  detailed notes identify that snapshot, not a promise of stable line numbers in a
  future build. Reproduction requires a lawfully held target and supported toolchain.
- Target identity: SHA-256
  `336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688`.
  No target bytes, raw disassembly, binaries, proprietary tools or generated linked
  outputs are included.
- No fresh MAT3-versus-N64 donor A/B compilation, reference rebuild, runtime test or
  new matching credit was performed. Native N64 match metadata was read, not freshly
  revalidated. An exact mapped span does not automatically include adjacent pools or
  the full linked symbol.
- Existing DC C borrows MAT3; some N64 shots, state and path reconstructions explicitly
  borrow MAT3/DC. Their textual agreement is circular as ancestry evidence. Target
  observations constrain behavior; N64 provides **native-platform triangulation**,
  not independent original-source provenance.
- A counterpart marked “not located” is a bounded search result, not proof of absence.
  Anonymous names and legacy labels require behavior/caller verification.

## Reproducible source pins

| Repository | Public source pin | Path convention |
|---|---|---|
| [Dreamcast](https://github.com/cabi24/SFRush2049-Dreamcast-decomp/tree/5fa10cd187ace77de5f630185357a90d502d8bad) | `5fa10cd187ace77de5f630185357a90d502d8bad` | Unprefixed DC paths are repository-relative. |
| [MAT3](https://github.com/cabi24/SFRush2049-mat3-decomp/tree/1cfe284a1d6bbf66b2f8c52fc16c941a75961b2f) | `1cfe284a1d6bbf66b2f8c52fc16c941a75961b2f` | `M/` is defined separately in each report. |
| [N64](https://github.com/cabi24/SFRush2049-n64-decomp/tree/09da2f064cf8c2443d5de60a2b2b67867360715e) | `09da2f064cf8c2443d5de60a2b2b67867360715e` | `N/` is defined separately in each report. |

The survey ran at local DC workflow commit
`8dee72f5752ca059a5fb91746cd34e40dcc7d0bd`, which added the Linux/wibo backend.
Its `src/`, `include/` and `map/` trees are unchanged from the public DC pin above;
this was checked with `git diff 5fa10cd 8dee72f -- src include map` (empty).
The local revision is an audit identifier, not a required public commit link.
Survey findings describe that source/build snapshot and must not be read as the
latest matching-progress counters after later commits.

## Next-use strategy

1. Start unmatched BattleDamage2 and FindWinner work from MAT3 intent, preserving
   target-supported DC offsets and branch policy. Keep CheckCPs' observed sound ID.
2. Use N64 to challenge model-layout, narrowing and expression-order assumptions.
   InitRecords is the clearest narrow structural counterexample to MAT3-first.
3. Keep DC platform deltas explicit: save/training policy, viewport/scissor constants,
   synchronous loading and interrupt masking cannot be imported wholesale.
4. Read prior failed probes before reopening matching work. Only a concrete new
   target/dataflow discriminator justifies another bounded SHC experiment; otherwise
   park the function. Native audio/rendering needs target/call-contract research.

The three detailed reports preserve the function addresses, mapped sizes, reference
paths and confidence limits underlying this recommendation. The original document's
contents are presented here as reviewable Markdown rather than an opaque DOCX binary.
