RESULT

blit: baseline 8/10 functions, 1038 text/literal bytes, 4 data bytes. Selected 9/10, 1118 text/literal bytes, 4 data bytes. DrawBlits at 8C01882A..8C01887A is newly exact, +80 emitted bytes. Existing exact functions remain exact. gfx: baseline and retained 8/10, 576 text/literal bytes and 88 data bytes; no gain.

CHANGES

Only canonical change is DrawBlits empty diagnostic condition spelled `blit->hide != 0`. Native SHC emits identical test code but uses target r2 for subsequent NumBlits load, rather than r3. NewBlit trials removed register, changed local declaration order, and used parameters directly; best reduced mismatch 44 to 11 bytes, entirely register allocation; no new exact range, so all NewBlit changes restored. gfx render-time split/reversed/chained assignments did not fix its two-byte register mismatch. Three config shapes (late pointer, direct field, mid pointer) improved local mismatch to 22 but none exact. gfx restored byte-for-byte from baseline.

VALIDATION

`python matching-evidence/2026-10-18/round2-blit-gfx/run.py blit selected` imports dc_build.build_unit without main, frozen native compiler/linker, private build/round2-blit-gfx outputs. Selected JSON records strict fixed-address unmasked results. No canonical report, objdiff or audit artifacts modified. Baseline and variant JSON/text evidence are retained here; compiler/link artifacts and trial source snapshots are private in build/round2-blit-gfx.

BLOCKERS

NewBlit register allocation, gfx render callback register allocation, and gfx config instruction scheduling remain. No shared headers, externs, maps or tools changed. Global integration audit remains prime responsibility.
