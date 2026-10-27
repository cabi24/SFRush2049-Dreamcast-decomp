# Round 5 research: one hundred source-probed targets

Four lanes completed 25 fresh targets each: [platform](platform.md),
[presentation](presentation.md), [simulation](simulation.md), and [screens](screens.md).
The normalized ledger contains 100 unique target addresses, all absent from the
prior 199-target exclusion set. Every target has real compiled probe evidence.
There are 128 associated probe records, **not 128 unique compiler invocations**;
whole-TU builds can serve multiple targets, and some records document neutral
variants that were not counted as actual attempts.

Accepted matching source is already on master: **+3,980 emitted bytes / 35 exact
functions**. Game/save contribute 2,218 bytes / 13 functions; HAL_text/stree
contribute 1,762 bytes / 22 functions. Only 18 selected targets became exact;
the remaining 17 exact functions are auxiliary/collateral gains, not extra target
attempts. See [matching results](../../round5-matching.md) and the dedicated
integration receipt. This documentation and patch archive adds no matching credit.

## Compact source-only archive

Four patches preserve selected nonexact research, not recommended production
changes. Platform/screens patches refer to the round-five starting source;
simulation research is separate from accepted game/save changes. Presentation's
patch is a delta against its accepted tree. Each presentation shape was compiled
individually; **the combined collection is not claimed as a verified build**.
Do not apply any research patch to reproduce accepted totals.

Historical lane reports retain their original scoped/full-build distinctions and
worker-stage statements. Referenced accepted, diagnostic and preservation packet
files are not all distributed here: the accepted changes are in master, and the
seven exhaustive platform probe patches remain local. This compact archive
contains only four reports, one normalized ledger and four selected patches.

Moved-only equality, relocated diagnostic similarity and target-sized bodies earn
no credit. Platform SDK-return predicates remain hypotheses, not recovered
assertions. Accepted HAL residual validity checks have independent target/semantic
review; their original assertion spelling remains unknown. No new error handler,
volatile qualifier, padding or historical macro is asserted.

New work requires semantic review, frozen full-TU compilation and fixed-address
linking, strict emitted-byte audit, and preservation of every accepted span.
No raw proof directories, original private ledgers, binary/compiler assets, target
bytes, assembly dumps, credentials or machine-local paths are included. No full
game runtime, native/Linux raw-artifact equivalence, CI pass, automatic merge or
watcher is claimed. Linux backend/HANDOFF publication is not part of this PR.
