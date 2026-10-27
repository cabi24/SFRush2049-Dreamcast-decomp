# Round 6 research — 473 qualifying targets, 27 short of 500

**The requested 500 attempts are not complete.** Four bounded lanes delivered
113 + 122 + 105 + 133 = **473 distinct qualifying targets**, leaving 27 slots
without a credible fresh evidence-backed attempt. Neutral builds, inspection-only
work, incidental shifts and exhausted historical shapes do not fill that gap.

The accepted checkpoints are separately on master: **+1,414 emitted bytes and ten
exact functions**. Current accepted totals: 89,978 game code/literal bytes,
159,309 image bytes, 579 exact game functions. Research adds no matching credit.
See [checkpoint1](../../round6-checkpoint1.md) and
[checkpoint2](../../round6-checkpoint2.md).

Categories: **229 body probes** (including one explicitly justified historical revisit),
**234 reconstructions**, and **10 causal placement-dependency attempts**.
All473 addresses are absent from the prior322-target exclusion; this does not
mean none was studied historically.

## Review contents

- [Lane1](lane1.md):113; body probes, one evidence-backed revisit and reconstructions.
- [Lane2](lane2.md):122; body probes and reconstructions, unresolved HUD data.
- [Lane3](lane3.md):105; direct probes/reconstructions and seven placement dependencies.
- [Lane4](lane4.md):133; body probes, two caller repairs, reconstructions and three placement dependencies.
- `ledger.json`: normalized identities, attempt categories, hypotheses and blockers.
- Four source-only deltas in `patches/`, with paths in `patch-manifest.json`.
- [Bounded compiler-context control](compiler-context/README.md): synthetic C,
  sanitized commands and hashed results; zero target/quota credit. Same-TU
  compilation selected BSR, while separate-real-object and DEFINE-only callers
  remained byte-identical JSR forms at the same near callee address. This is not
  a universal linker claim. The invalid duplicate-dos_src harness was discarded.

## Delta and dependency contract

These are **archived patches, not active repository source changes**. Each was
materialized from its immutable worker packet at a55b18a, then differenced only
on touched source/header paths against accepted checkpoint2 master
6c46ee07a58c26b2915e3849510cfdbf8260b92b. Already accepted asincos, mdrive,
audio, audiosfx and camera changes are not reapplied or reversed. The portable
camera provenance remains unchanged. Only new research remains in these deltas.

The four patches have disjoint paths and pass git apply --check separately and
together against that accepted tree. **This is not a four-lane compiled or audited
integration.** Lane1 effects research depends on lane3's texture header, which is
included exactly once, in lane3. Do not duplicate that header correction. Private
ABI proposals are research, not an extension of accepted matching credit.

Worker build/audit summaries are historical and scope-specific. Known regression,
unresolved-symbol and out-of-map limits remain explicit in the lane summaries.
Patch application success does not prove semantic correctness or recover credit.
Optional speculative variants and all raw proof directories are omitted.

No game/ROM bytes, raw assembly dumps, binary/compiler assets, credentials,
machine-local paths, denied HANDOFF payload or Linux backend are included. New
source proposals require independent semantic review, frozen compile/link replay,
strict emitted-byte audit and preservation of every accepted span. No whole-game
runtime, native/Linux raw-artifact parity, CI pass, automatic merge or watcher is
claimed. This draft preserves bounded research and an honest shortfall; it does
not declare the 500-target campaign complete.
