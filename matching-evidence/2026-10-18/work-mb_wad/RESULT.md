# MB WAD matching follow-up

RESULT: No new strict credit. Baseline preserved: 182 exact text bytes, 3/4 exact functions.
MBSetupWad at 0x8C03CF94 remains parked after three source-shape trials.

Baseline: 70-byte MBSetupWad differs in 11 bytes solely through r6/r7 allocation;
target preserves fixup in r7 and uses r6 for entry address, baseline does the reverse.

1. Swap fixup and e declaration order: unchanged 11-byte residual.
2. Move fixup assignment after TOCentries load: residual reduces to 3 bytes in the
first two copies only. All remaining instructions exactly match, and the other three
functions remain strict exact. Target at CF98/CF9A is mov r5,r7; mov r7,r2;
variant02 is mov r5,r2; mov r5,r7. Saved source and listing are in variant02/.
3. Load TOC through fixup rather than wad->base: compiler removes a copy and emits
68 bytes; following functions move by -2 and earn no strict credit. Rejected.

CHANGES: src/mb_wad.c restored to the baseline bytes. Only this private evidence
directory remains added. No canonical reports, objdiff, headers, map or receipts touched.

VALIDATION: python -B matching-evidence/2026-10-18/work-mb_wad/run.py LABEL
for baseline, variant01, variant02, variant03. The runner imports existing build
functions, redirects BUILD/OBJ_DIR/LINK_DIR to this private directory, and invokes
only build_unit, never canonical main/split/report generation. Compiler frozen at
shc-v5.1r11 --no-default-flags -- -speed -noinline; linker START P=8C03CF94.
Each LABEL contains source.c, result.json, pp/, obj/, link/ and native link report.
python -B matching-evidence/2026-10-18/work-mb_wad/list.py LABEL creates the listing
using the existing dependency-free pilot SH4 decoder. Equality comes from complete
fixed-address byte comparison, not the decoder. No masks or assembly used.

BLOCKERS: Source-shape stop rule reached; variant02 is a useful 3-byte near miss for
a future evidence-backed pass, not a strict match and not promoted. No tool failure.
