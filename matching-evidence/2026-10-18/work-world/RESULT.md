# World follow-up, 2026-10-18

RESULT: no new exact bytes or functions. Original source restored byte-for-byte.

Validation: `python -B matching-evidence/2026-10-18/work-world/run.py baseline`
imports dc_build build_unit with only C.BUILD, C.OBJ_DIR and C.LINK_DIR redirected
inside this private packet. No split, canonical report, objdiff or audit receipt writer runs.
Native SHC 5.1r11 frozen flags and LNK fixed START P=8C065C78, existing externs.
The target SHA-256 is checked by dc_common. No masks and no copied target bytes used.

Baseline: 692-byte P section; 5/6 functions exact; 378 emitted exact text bytes.
Only NewWorldObject (8C065C78..8C065DB0) differs, 205 byte mismatches.
First differing word 8C065C8A: target 6E43 (mov r4,r14), built 6D43 (mov r4,r13).
Target stores incoming Wnode at stack [r15] (2F42 at 8C065C90), preserves Parent
in r8 (6853 at 8C065C92); built instead keeps Wnode in r13 and stores short Parent
on stack (2F51). This induces register allocation/scheduling differences throughout.
Target and baseline source have equal overall section size and five later exact functions.

Three bounded source-shape attempts:
1. Make NewWorldObject Parent int (definition and private prototype). P=684 bytes,
   0 strict exact functions/bytes, 300 mismatches in first target range; restored.
2. Preserve original Parent short, declare SetupTarget second argument int based on
   target passing the raw preserved register. P=684, same strict regression; restored.
3. Preserve original types and add register storage hint to Parent parameter to
   favor the target register-resident allocation. P=704 bytes, 0 strict exact
   functions/bytes, 260 mismatches in first range and 12 out-of-map bytes; restored.

No fourth attempt: parked at repository three-shape limit. Source improvement is
not proved, so none is retained. Original source SHA-256:
39d4f0ae9f628259495a9fa01ebe553a124d4d9cde2db29a9f78f9dadbaa521e

Artifacts: each baseline/attemptN subfolder has complete private .obj/.elf/.o,
.abs/.map/.json, preprocessed source and result.json. These generated outputs are
review artifacts only and must not be committed. The attemptN-source.c snapshots
are source-shape evidence. Original source is original.c. run.py is reproducible.

BLOCKERS: native objdiff 3.8.2's displayed SH instruction words are byte-swapped
relative to raw LE target and SYSROF words (e.g. raw target 2FE6 prologue is shown
as mov #0x2f,r6 / E62F); its formatted assembly was not used as matching evidence.
words.py compares raw little-endian 16-bit words directly. Capstone remains absent
from default Python, and no installation or alternate policy launch was attempted.
