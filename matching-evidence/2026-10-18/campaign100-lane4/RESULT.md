# Campaign100 lane4 result

Completed all17 assigned targets across9 units. Retained one source expression change in src/pakerr.c: _DrawPakErrNoPak uses choice1 ? 0 : 1 rather than choice1 == 0. Native frozen SHC5.1r11 output matches the full534-byte fixed-address function; emitted credit532 bytes excludes2 fill bytes. Net +1 exact function,+532 text,+0 data. All other trials restored.

Fresh independent native builds ran for controls,options,optionscr,pakerr,pausescr,resurrect,road,save,smoke with run.py UNIT final-independent. record.py UNIT final-independent verified every baseline exact mapped text/pool/data span remained exact and preserved every baseline exact function. final_review.py verified current source SHA256, expanded include freshness, native build source hashes, and totals. Exit0,TOTAL1 532 0. Individual JSON files contain current source/object/linked image/preprocessed source SHA256.

Validation command sequence: python -B matching-evidence/2026-10-18/campaign100-lane4/run.py UNIT final-independent; python -B matching-evidence/2026-10-18/campaign100-lane4/record.py UNIT final-independent NOTE; python -B matching-evidence/2026-10-18/campaign100-lane4/final_review.py.

Residuals and attempt counts are in progress.md. Controls dependent placement, options register allocation, optionscr loop scheduling, pausescr preceding body placement, resurrect semantic/accumulator ordering, road force/init ordering, save upstream size/call form, smoke matrix loop scheduling remain unmatched. No shared headers/maps/tooling/global audit outputs changed; only private native unit builds used.
