# Round 3 research: fifty source-probed targets

This archive contains four sanitized lane summaries, a fifty-target ledger and
four **nonpromotable source-only** research patches. It changes no active source,
header, map, tool or canonical evidence and earns no additional matching credit.

All fifty distinct functions received actual source probes: 13 audio, 13 gameplay,
12 spatial and 12 screens. The ledger sums to 96 associated candidate builds;
some targets share whole-TU builds, so this is **not** a count of unique compiles.

Accepted master gains are +2,930 emitted bytes / nine exact functions. Ledger
rows sum to 2,886 bytes: the remaining 44 bytes are the shared stunt pool, counted
once by the strict integration receipt. See [accepted results](../../round3-matching.md).

- [Audio](audio.md): +520 accepted bytes / two functions; manager diagnostics remain research.
- [Gameplay](gameplay.md): +1,244 accepted bytes / three functions, including 44 shared pool bytes.
- [Spatial](spatial.md): +792 accepted bytes / three functions; surface/heap near misses remain research.
- [Screens](screens.md): +374 accepted bytes / one function; other changes withheld.

## Patch scope and rejection boundary

Patches are historical source experiments, not instructions to apply them to
master. Audio-manager changes were based on the round-three starting source.
Spatial and ordinary screens research were based on their lane's accepted
positive state. The optional screens-withheld-goto patch is a cumulative
historical proposal; it includes the accepted delete change and must not be
applied wholesale atop master. No rejected gameplay patch is included.

The goto packet makes _FindFile technically exact (+36 bytes) but worsens its
nonmatching caller and lacks independent source-shape evidence. Those 36 bytes
are **explicitly excluded** from the accepted receipt, ledger credit and master.
Its preservation here does not endorse promotion. Other near misses can also
regress existing credit. New evidence and independent review are required before
any future source change, followed by frozen compilation, real fixed-address
links, strict emitted-byte audit and preservation of every accepted span.

MAT3 and N64 are reconstructed semantic references, not recovered original
source; Dreamcast target evidence decides layout and ABI. No target bytes, raw
assembly dumps, binaries, private proof directories, local paths or credentials
are included. Historical scoped lane audits do not replace the separate full
114-unit integration audit. No whole-game execution, native/Linux raw-artifact
identity, automatic merge or CI monitoring is claimed.
