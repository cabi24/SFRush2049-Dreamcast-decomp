/* dc_types.h - Dreamcast (SHC 5.x, SH-4 little-endian) layouts of the Rush 2049 game structures.
 *
 * PARTIAL OVERLAYS. Every named field sits at an offset proven from the DC binary (1ST_READ.BIN,
 * base 0x8C010000); unproven space is an explicit char padN[...] gap. The evidence address after
 * each field is the target instruction or literal-pool word that fixes its offset (pool words are
 * the mov.w displacement constants SHC emits for offsets above the 4-bit/8-bit immediate range).
 * The MAT3 (GameCube) name and type are kept wherever the DC evidence agrees with them; MAT3 offsets
 * are NOT evidence by themselves. See include/README.md for the rules.
 *
 * C89 only (SHC): no // comments, no stdint.h, no anonymous unions.
 */
#ifndef DC_TYPES_H
#define DC_TYPES_H

/* ------------------------------------------------------------------ tCar (MAT3 _tCar) */
/* Only the drivetrain tail is proven. MAT3 size 0xb4 is consistent (downshiftangvel ends at 0xb4)
   but the struct is never indexed on DC yet, so its size is not evidence-backed. */
typedef struct tCar {
    char pad0[0xa0];
    float enginvmi;         /* 0xa0  whatslips pool 8C022EE8 (0x00a0), mov.l @r4 = m->car */
    float dwratio;          /* 0xa4  transmission pool 8C022FDE (0x00a4), find_best_gear 8C0230A2 */
    float clutchmaxt;       /* 0xa8  whatslips pool 8C022EE2 (0x00a8) */
    float upshiftangvel;    /* 0xac  autoshift pool 8C022FD4 (0x00ac) */
    float downshiftangvel;  /* 0xb0  autoshift 8C022F28 add #4 from upshiftangvel */
} tCar;

/* ------------------------------------------------------------------ tTune (MAT3 anon_diffbl_weight_scale_2c) */
typedef struct tTune {
    char pad0[0x08];
    float torque_scale;     /* 0x08  enginetorque (drivetra_part), m->tune->torque_scale */
    float trans_scale;      /* 0x0c  transmission 8C022FB6 mov #0xc; fmov.s @(r0,r3) */
    float clutchmaxtk;      /* 0x10  whatslips 8C022E44 mov #0x10 */
    float shiftk;           /* 0x14  autoshift 8C022F16 mov #0x14 */
} tTune;

/* ------------------------------------------------------------------ tTire (MAT3 anon_tradius_springK_5c) */
typedef struct tTire {
    char pad0[0x14];
    float invmi;            /* 0x14  drivetrain pool 8C022DCE (0x04fc = 0x430 + 2*0x5c + 0x14), 8C022DD0 (0x0558) */
    char pad1[0x48 - 0x18];
    float angvel;           /* 0x48  drivetrain 8C022CC0..CCA: base 0x430, 0x15c = 3*0x5c+0x48, 0x100 = 2*0x5c+0x48 */
    char pad2[0x5c - 0x4c]; /* size 0x5c: stride 0x58c-0x530 and 0x558-0x4fc in drivetrain */
} tTire;

/* ------------------------------------------------------------------ tFastIn (MAT3 anon_modelIteration_modelTime_10) */
/* size not proven on DC (MAT3: 0x10); nothing after it in tModelData is used yet. */
typedef struct tFastIn {
    char pad0[0x04];
    float modelTime;        /* 0x04  upshift/downshift pool 8C0230A8 (0x0714 = fastin 0x710 + 4) */
} tFastIn;

/* ------------------------------------------------------------------ tModelData */
typedef struct tModelData {
    tCar *car;              /* 0x000 whatslips 8C022DEE mov.l @r4,r3 then car fields */
    tTune *tune;            /* 0x004 transmission 8C022FA0 mov.l @(0x4,r4) */
    char pad0[0x09 - 0x08];
    signed char trans;      /* 0x009 transmission 8C022F96 mov.b @(0x9,r4) */
    signed char autotrans;  /* 0x00a drivetrain 8C022CAC mov.b @(0xa,r14) */
    signed char handling;   /* 0x00b enginetorque (drivetra_part) */
    signed char engine_type;/* 0x00c enginetorque (drivetra_part) */
    char pad1[0x64 - 0x0d];
    float tireForce[4][3];  /* 0x064 drivetrain 8C022CDE add #0x64; [3][1] at +0x28, [2][1] at +0x1c; pool 8C022DC6 (0x80), 8C022DCA (0x8c) */
    char pad2[0x3b4 - 0x94];
    float torque[4];        /* 0x3b4 drivetrain pool 8C022DC8 (0x03bc = torque[2]), add #4 -> torque[3] */
    char pad3[0x3cc - 0x3c4];
    float clutch;           /* 0x3cc whatslips pool 8C022DD6 (0x03cc) */
    float throttle;         /* 0x3d0 autoshift pool 8C022FD2 (0x03d0) */
    char pad4[0x3f4 - 0x3d4];
    signed short gear;      /* 0x3f4 pools 8C022EE4, 8C022FD8, 8C0230A0 (mov.w @(r0,r4)) */
    signed short commandgear; /* 0x3f6 autoshift pool 8C022FD6 (0x03f6) */
    char pad5[0x404 - 0x3f8];
    float engtorque;        /* 0x404 whatslips pool 8C022EEA (0x0404) */
    float engangvel;        /* 0x408 whatslips pool 8C022DD4, autoshift 8C022FDA (0x0408) */
    const signed short *torquecurve; /* 0x40c engine (drivetra_part) */
    float dwtorque;         /* 0x410 drivetrain pool 8C022DC4 (0x0410) */
    float dwinvmi;          /* 0x414 whatslips pool 8C022EF4 (0x0414) */
    float efdwinvmi;        /* 0x418 drivetrain pool 8C022DCC, whatslips 8C022EF6 (0x0418) */
    float dwangvel;         /* 0x41c drivetrain pool 8C022DC0, find_best_gear 8C0230A4 (0x041c) */
    float clutchtorque;     /* 0x420 whatslips pool 8C022EE6 (0x0420) */
    float clutchangvel;     /* 0x424 whatslips pool 8C022EF2 (0x0424) */
    float transratio;       /* 0x428 transmission pool 8C022FDC (0x0428) */
    float totalratio;       /* 0x42c whatslips pool 8C022DD2, 8C022EEE (0x042c) */
    tTire tires[4];         /* 0x430 drivetrain pool 8C022DBC (0x0430) used as the array base */
    char pad6[0x5a8 - 0x5a0];
    signed char magicdiff;  /* 0x5a8 drivetrain pool 8C022DC2 (0x05a8), mov.b @(r0,r14) */
    char pad7[0x60c - 0x5a9];
    int roadcode[4];        /* 0x60c engine (drivetra_part) */
    char pad8[0x634 - 0x61c];
    float dt;               /* 0x634 whatslips pool 8C022EEC (0x0634) */
    char pad9[0x640 - 0x638];
    signed char crashflag;  /* 0x640 engine (drivetra_part) */
    char pad10[0x658 - 0x641];
    float shifttime;        /* 0x658 upshift/downshift pool 8C0230A6 (0x0658) */
    char pad11[0x710 - 0x65c];
    tFastIn fastin;         /* 0x710 (modelTime at 0x714, pool 8C0230A8) */
} tModelData;

/* Layout self-checks (typedefs only, no code or data). SHC rejects address constants such as
   &((T *)0)->f in array bounds, so only sizes are checked; they catch padding arithmetic errors
   because every overlay ends on its last proven field. */
typedef char dc_check_tTire_size[(sizeof(tTire) == 0x5c) ? 1 : -1];
typedef char dc_check_tModelData_size[(sizeof(tModelData) == 0x718) ? 1 : -1];
typedef char dc_check_tCar_size[(sizeof(tCar) == 0xb4) ? 1 : -1];
typedef char dc_check_tTune_size[(sizeof(tTune) == 0x18) ? 1 : -1];

#endif /* DC_TYPES_H */
