# Round 2: hud outcomes

Historical bounded experiments, not an instruction to apply unmatched source.
Accepted gains are separately documented in ../../round2-matching.md.
Private diagnostics and raw build artifacts are intentionally omitted.

## 21. Hidden — negative

Unit: `hud`; target: `8C038FAC`. Reported accepted delta: 0 emitted bytes / 0 functions.

- MAT3 body with DC-verified field types and global addresses, whole-TU appended in target order among implemented bodies

**Residual:** Target 8C038FAC, actual linked 8C035704; missing preceding HUD bodies prevent native placement

**Resume only with:** Recover intervening HUD source in true TU order; no padding or relocated-credit workaround

## 22. ShowCheckpointTime — negative

Unit: `hud`; target: `8C038F7E`. Reported accepted delta: 0 emitted bytes / 0 functions.

- MAT3 body with DC-verified field types and global addresses, whole-TU appended in target order among implemented bodies

**Residual:** Target 8C038F7E, actual linked 8C035720; missing preceding HUD bodies prevent native placement

**Resume only with:** Recover intervening HUD source in true TU order; no padding or relocated-credit workaround

## 23. animBones — negative

Unit: `hud`; target: `8C035F94`. Reported accepted delta: 0 emitted bytes / 0 functions.

- MAT3 body with DC-verified field types and global addresses, whole-TU appended in target order among implemented bodies

**Residual:** Target 8C035F94, actual linked 8C035720; missing preceding HUD bodies prevent native placement

**Resume only with:** Recover intervening HUD source in true TU order; no padding or relocated-credit workaround

## 24. animRadar — negative

Unit: `hud`; target: `8C038238`. Reported accepted delta: 0 emitted bytes / 0 functions.

- MAT3 body with DC-verified field types and global addresses, whole-TU appended in target order among implemented bodies

**Residual:** Target 8C038238, actual linked 8C035720; missing preceding HUD bodies prevent native placement

**Resume only with:** Recover intervening HUD source in true TU order; no padding or relocated-credit workaround

## 25. animElapsedTimer — negative

Unit: `hud`; target: `8C036D78`. Reported accepted delta: 0 emitted bytes / 0 functions.

- MAT3 body with DC-verified field types and global addresses, whole-TU appended in target order among implemented bodies
- Correct target-specific z assignment to 1 from instruction at 8C036E6E; initial transcription used 0 and was rejected

**Residual:** Target 8C036D78, actual linked 8C035720; missing preceding HUD bodies prevent native placement

**Resume only with:** Recover intervening HUD source in true TU order; no padding or relocated-credit workaround

## 26. animStuntPoints — negative

Unit: `hudextra`; target: `8C093C88`. Reported accepted delta: 0 emitted bytes / 0 functions.

- Move TB_SetTextColor before player pointer materialization and recover DC screen Y minus4 before division from 8C093E08
- Use per-iteration coin pointer corresponding to target pointer spill at 8C093D74; no strict improvement

**Residual:** Baseline stack60 vs target68; candidate coin pointer enlarges output; repeated loop/unroll and lifetime differences remain. y-4 is a semantics finding only.

**Resume only with:** Resolve upstream TU layout and register lifetimes from actual target, not padding or flag changes

## 27. ShowHUDExtra — exact

Unit: `hudextra`; target: `8C0938E8`. Reported accepted delta: 382 emitted bytes / 1 functions.

- N64 native counterpart places w/2,h/2 in InitPane arguments; DC width spill and invariant division sequence confirm this shape rather than MAT3 pre-halved locals

**Residual:** See accepted matching note.

## 28. animStuntCoins — negative

Unit: `hudextra`; target: `8C094160`. Reported accepted delta: 0 emitted bytes / 0 functions.

- Extend backward predecessor pointer lifetime to function scope based on pointer traversal across 94428..4AC; emitted strict result unchanged

**Residual:** Stack28 vs target68; scope probe does not fix allocation or +upstream drift. Earlier MAT3 block-scope reconstruction remains accepted baseline.

**Resume only with:** Resolve upstream TU layout and register lifetimes from actual target, not padding or flag changes

## 29. animAmmoCounts — negative

Unit: `hudextra`; target: `8C0948BC`. Reported accepted delta: 0 emitted bytes / 0 functions.

- Promote x/y coordinate locals from signed short to int, target holds both in registers rather than current y spill; no strict improvement

**Residual:** Stack12 vs target8 and top-tested vs target bottom-tested entry; candidate does not repair upstream drift.

**Resume only with:** Resolve upstream TU layout and register lifetimes from actual target, not padding or flag changes

## 30. animWeaponIcons — negative

Unit: `hudextra`; target: `8C0949D8`. Reported accepted delta: 0 emitted bytes / 0 functions.

- Recover target screenPos Y +4.0f from fmac at 8C094BBC and constant pool
- Replace cached car pointer with direct indexed loads to shorten lifetime; larger output, no strict credit

**Residual:** Missing fourth saved FP register is repaired by Y+4 semantics, but integer allocation/pool/layout differences persist; no strict gains.

**Resume only with:** Resolve upstream TU layout and register lifetimes from actual target, not padding or flag changes

