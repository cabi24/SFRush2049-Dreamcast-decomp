# Zoidpoly matching followup

ZOID_RemovePoly at 8C06926C..8C06929C is now an exact fixed-address match.
The final source rewrites the do/while compound condition into an unconditional
loop with sequential break tests, preserving the original evaluation order.
SHC now reloads the count into r3 at 8C069292 (6352) and tests r3 at
8C069294 (4315), rather than the baseline r1 pair (6152/4115).

Only src/zoidpoly.c changed. All seven previous function matches, pools,
the 900-byte P section, and BSS placement remain exact. Earned credit grows
from 848 to 896 emitted bytes, excluding the same four fill bytes. One new
48-byte function is exact. No initialized data credit or BSS credit is added.

## Reproduction

Run `python -B matching-evidence/2026-10-18/work-zoidpoly/run.py confirmed`.
The runner calls the standard build_unit routine but redirects BUILD, OBJ_DIR
and LINK_DIR into this private folder. It does not run dc_build.py main,
dc_split, the report writer, objdiff writer, or audit receipt writer.
Native compiler is frozen SHC5.1r11 with --no-default-flags -- -speed -noinline.

Baseline and three candidates are retained as expanded source, native objects,
native absolute links, and JSON result records in their private subfolders:

* baseline: 7/8 exact, 848 credit, two differing bytes in RemovePoly.
* direct-global: direct global references add a reload and regress the tail.
* temp-condition: explicit condition temporary induces stack traffic and regresses.
* explicit-breaks: 8/8 exact, 896 credit, no differing bytes.
* confirmed: independently repeated successful native compilation/link.

The optional listing is unavailable under the default Python because capstone
is missing. words.py compares all raw 16-bit words in the final function and
pool; fixed-address strict comparisons and credit work without capstone.

No headers, maps, externs, shared build outputs, commits or pushes were changed.
Integration into the canonical report remains the parent's responsibility.
