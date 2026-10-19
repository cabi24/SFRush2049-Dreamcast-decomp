# Round 2 mbox investigation

Result: no new strict matches. Restored src/mbox.c byte-for-byte, hash a78c06f4afc3b21dec4287b6209fe669de8bf2050564606737ded59f4d942df1. Final native private link reproduces baseline 6/13 exact functions, 530 text/literal and 523 initialized-data bytes. No shared reports or canonical outputs modified.

Native commands: python matching-evidence/2026-10-18/round2-mbox/run.py LABEL. Harness redirects dc_build C.BUILD/OBJ_DIR/LINK_DIR and calls build_unit directly with frozen toolchain/flags, retaining full fixed-address unmasked comparison. Summary JSON records source, object and absolute-image hashes per attempt.

MBOX_NewObjectCam target D0A6-D0AC spills/reloads signed-short third argument to stack+4. Baseline instead sign-extends and computes camflags in a register, emitting 36 rather than 38 bytes and moving GetChild/GetNext two bytes early. Three source shapes tested: volatile argument (40 bytes), direct expression (36), direct expression with volatile argument (38 but still 19 bytes differ). None accepted; parked at three shapes. Volatile shape had an initial unsuccessful PowerShell Set-Content call and unchanged compile; successful rerun replaced that same label with the actual variant.

MBOX_FindTexture_WORK target keeps lastModel in r11 across builtin strcpy, re-sign-extends it at bottom loop comparison, and caches ModelActive in r10; baseline hoists signed bound and has a separate entry test. Three shapes tested: register locals, explicit goto test/body loop, register lastModel parameter. All differ; register/goto regress other functions through shared pool movement and were discarded.

Residual baseline: NewObjectCam 32 differing bytes; HideObject 76; FindObject 95; FindTexture 106; palette search 102. GetChild/GetNext are byte-identical moved functions and remain uncredited. Target and baseline full unit size both 1476 due compensating allocation/pool placement.

A dependency-free compare_listing.py uses pilot/tools/sh4.py. Initial name dis.py shadowed stdlib dis; renamed before successful use. No dependency installs, downloads, flag changes, ASM byte emission, masking, global audit or full game builds.
