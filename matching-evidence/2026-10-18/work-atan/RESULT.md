# atan bounded follow-up, 2026-10-18

RESULT: no earned change. Canonical src/atan.c was never edited. Baseline remains 286/368 emitted text/literal bytes and 3/4 exact functions. Three source shapes were tested; stop rule reached.

## Validation

From C:\rush2049-dreamcast, ran:

    python -B matching-evidence/2026-10-18/work-atan/trial.py baseline
    python -B matching-evidence/2026-10-18/work-atan/trial.py locals
    python -B matching-evidence/2026-10-18/work-atan/trial.py addnegative
    python -B matching-evidence/2026-10-18/work-atan/trial.py denomreverse

trial.py imports dc_build functions without invoking its main, redirects C.BUILD/OBJ_DIR/LINK_DIR/SRC into this private evidence directory, expands the canonical header, and calls build_unit for atan only. This uses the unchanged native SHC5.1r11 compiler and LNK with frozen flags, at START P=8C0113C0. All reports, pp source, native objects and absolute outputs are private. No canonical report, objdiff, audit receipt, headers, externs or map were modified. No full game build or game execution.

Each trial directory contains report.json (strict full fixed-address comparison), words.txt (target and emitted little endian words), atan.c (expanded compiler source), obj and link. summary.json records native output hashes.

## Trials

- baseline: 3 functions and 286 bytes exact. fxatan 74, fatan2 114, fatan 34 and external literal pool 64 all exact. fsatan same address and82-byte size, 9 differing bytes in8 words.
- locals: splitting numerator and denominator into explicit float locals saves an extra callee-saved FP register and expands fsatan by4 bytes. This shifts following functions and pool; zero fixed-address exact spans. Rejected.
- addnegative: replace arg-1.0f with arg+-1.0f. Identical baseline output, including9-byte residual. Rejected.
- denomreverse: replace arg+1.0f with1.0f+arg. Identical baseline output, including9-byte residual. Rejected.

## Residual

Only 8C01143C..8C01144C differs in baseline, and only register selections differ. The target uses fr2 for numerator and fr1 for denominator; compiler uses fr0 and fr3. Both compute arg-1 then arg+1 then divide in the call delay slot. Target:

    8C01143C F39D fldi1 fr3
    8C01143E F2FC fmov fr15,fr2
    8C011440 F34D fneg fr3
    8C011442 F230 fadd fr3,fr2
    8C011444 F1FC fmov fr15,fr1
    8C011446 F1E0 fadd fr14,fr1
    8C011448 F42C fmov fr2,fr4
    8C01144A BFB9 bsr fxatan
    8C01144C F413 fdiv fr1,fr4

Actual baseline uses fr1 for the temporary minus-one literal, fr0 for numerator and fr3 for denominator. No flag changes, pragma, assembly or hardcoded register solution attempted. The recovered MAT3 source has the same original expression. A future attempt needs independent declaration/lifetime evidence rather than further algebraic rewrites.
