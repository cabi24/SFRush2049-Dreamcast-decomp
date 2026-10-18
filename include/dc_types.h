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


/* Shared layouts integrated from frozen worker receipts, 2026-10-18. */
typedef struct tDLLE {
    void *next;
    void *prev;
} tDLLE;

typedef struct tDLList {
    char handle;
    char dbl;
    int numElements;
    void *head;
    void *tail;
} tDLList;

/* statll_types.h - proposed additions for include/dc_types.h.
 * DC evidence comes from statll, 0x8C05138C..0x8C05145A.
 * The private candidate uses this header until integration.
 */



typedef struct tStatLLE {
    void *next;            /* 0x00: GetStatLLE 8C0513DE mov.l @r5,r2 */
} tStatLLE;

typedef struct tStatList {
    char dbl;              /* 0x00: InitStatLL 8C051394 mov.b r0,@r4; GetStatLLE 8C0513E6 */
    char pad0[0x04 - 0x01];
    int numElements;       /* 0x04: InitStatLL 8C05138E mov.l r7,@(4,r4) */
    int elementSize;       /* 0x08: InitStatLL 8C051390 mov.l r6,@(8,r4) */
    void *list;            /* 0x0c: InitStatLL 8C05138C mov.l r5,@(12,r4) */
    void *head;            /* 0x10: InitStatLL 8C0513A0; GetStatLLE 8C0513FA */
    void *store;           /* 0x14: InitStatLL 8C05139C; GetStatLLE 8C0513D8 */
} tStatList;

typedef char dc_check_tDLLE_size[(sizeof(tDLLE) == 0x08) ? 1 : -1];
typedef char dc_check_tStatLLE_size[(sizeof(tStatLLE) == 0x04) ? 1 : -1];
typedef char dc_check_tStatList_size[(sizeof(tStatList) == 0x18) ? 1 : -1];


/* Dreamcast texture overlay.  Only fields accessed by this unit are named. */
typedef struct tTexture {
    char name[16]; /* mbox name key/copy: 8C03D4C0..4CC; stride0x30. */
    unsigned short width;  /* 0x10: 8C05B44E mov.w, 8C05B450 extu.w */
    unsigned short height; /* 0x12: 8C05B4A0 mov.w, 8C05B4A2 extu.w */
    char pad1[0x24 - 0x14];
    float uls;             /* 0x24: 8C05B45E/8C05B4B0 */
    float ult;             /* 0x28: 8C05B4A8/8C05B4B0 */
    unsigned int strip;    /* 0x2c: 8C05B392 load, 8C05B55C store */
} tTexture;

/* Stride 0x0c: index*3 then <<2 at 8C05B368..374, 8C05B542..548. */
typedef struct tTextureBase {
    char *name;            /* +0: 8C05B384 */
    tTexture *tex;          /* +4: 8C05B386, 8C05B390, 8C05B54C */
    unsigned int texBase;  /* +8: 8C05B394 store, 8C05B55A load */
} tTextureBase;

/* Stride 0x14: 8C05B3A8 and 8C05B55E. */
typedef struct tTextureSwap {
    signed short num;      /* +0: 8C05B356, 8C05B4F4 */
    signed short startnum; /* +2: 8C05B39C, 8C05B53E */
    signed short fd;       /* +4: 8C05B4E8 */
    signed short idx;      /* +6: 8C05B39E, 8C05B4EE..500 */
    float time;            /* +8: 8C05B398..39A, 8C05B4D0..4DE */
    float initTime;        /* +c: 8C05B388/396, 8C05B4E0/4E2 */
    tTextureBase *swapTexs;/* +10: 8C05B370, 8C05B38A, 8C05B540 */
} tTextureSwap;

/* Stride 0x14: 8C05B338 and 8C05B4B2. */
typedef struct tPaletteAnim {
    char *name;            /* +0: 8C05B30A, 8C05B330 */
    signed short start;    /* +4: 8C05B41A..420 */
    signed short end;      /* +6: 8C05B426/436 */
    signed char offset;    /* +8: 8C05B410 + signed word multiply */
    signed char state;     /* +9: 8C05B31A..31C */
    signed short speed;    /* +a: 8C05B3F0..3F8 */
    float time;            /* +c: 8C05B316..318, 8C05B3E0..3EE */
    void *ptr;             /* +10: 8C05B334, 8C05B404 */
} tPaletteAnim;

typedef char dc_check_tTexture_size[(sizeof(tTexture) == 0x30) ? 1 : -1];
typedef char dc_check_tTextureBase_size[(sizeof(tTextureBase) == 0x0c) ? 1 : -1];
typedef char dc_check_tTextureSwap_size[(sizeof(tTextureSwap) == 0x14) ? 1 : -1];
typedef char dc_check_tPaletteAnim_size[(sizeof(tPaletteAnim) == 0x14) ? 1 : -1];

/* Private proposal for include/dc_types.h, proven by countdown target instructions. */



/* Target initialized bytes at 8C0BD8C0 are FF FF FF C0. */
typedef union tRGBA {
    unsigned char c[4];
    unsigned int rgba;
    unsigned int word; /* sky alias, same 4-byte storage */
} tRGBA;

typedef struct tMatrix3 {
    float uvs[3][3];
    float pos[3]; /* 0x24/28/2c: ShowCountdown 8C0226E2..8C02270A. */
} tMatrix3;

typedef union tUVMatrix {
    tMatrix3 mat3;
    float uvs[4][3];
} tUVMatrix;

typedef struct tCamera {
    tUVMatrix mat;
    tUVMatrix n64; /* 0x30: ShowCountdown 8C0226DA passes matrix to U64WorldVector. */
    char pad0[0x98 - 0x60]; /* stride 0x98: literal 8C02279E, multiplication 8C0226C4. */
} tCamera;

typedef char dc_check_tRGBA_size[(sizeof(tRGBA) == 4) ? 1 : -1];
typedef char dc_check_tUVMatrix_size[(sizeof(tUVMatrix) == 0x30) ? 1 : -1];
typedef char dc_check_tCamera_size[(sizeof(tCamera) == 0x98) ? 1 : -1];



typedef tCamera dcSkyCamera;
typedef tMatrix3 dcSkyMat3;
/* Private carload layout proposal. C89; fields/strides follow DC accesses. */

typedef struct tCarLoadLink {
    char pad0[1];
    unsigned char car;       /* +1: 8C01B780..B782 store; 8C01B7C0..B7C4 extu.b */
    unsigned char PaintJob;  /* +2: 8C01B7C8 store; 8C01B988..B990 extu.b */
    char pad1[0x0a - 0x03];  /* stride 0x0a: 8C01B760..B768 = (i*4+i)*2 */
} tCarLoadLink;

/* This array starts at the drone_type field of model[0]. Its effective base and
   stride are proven here. The parent tModelData base and field offset still need
   independent evidence before this can become a shared tModelData extension. */
typedef struct tCarLoadDroneSlice {
    signed char drone_type; /* 8C01B748..B758, 8C01B9BA..B9CA: byte at 8C14D380+i*0x808 */
    char pad0[0x808 - 1];   /* stride: (i*0x100+i)*8 */
} tCarLoadDroneSlice;

typedef char dc_check_carload_link_size[(sizeof(tCarLoadLink) == 0x0a) ? 1 : -1];
typedef char dc_check_carload_drone_size[(sizeof(tCarLoadDroneSlice) == 0x808) ? 1 : -1];


typedef struct tPolyVert {
    float ob[3];           /* +0,+4,+8: 8C0691CC..91EC */
    float tc[2];           /* +0x0c,+0x10: 8C0690A4,913E */
    unsigned char cn[4];   /* +0x14..17: 8C0691FE,9208,9216,9222 */
} tPolyVert;               /* stride0x18: 8C06918A..9194 */

typedef struct tPoly {
    signed short nverts;  /* +0: 8C068F4A,8C068FD2..FD4 */
    unsigned short flags; /* +2: 8C068F86,8C069246 */
    unsigned short texidx;/* +4: 8C068F98,8C068FFE */
    signed short z;       /* +6: 8C069244 */
    tPolyVert verts[4];   /* +8: 8C06909A; stride0x68: 8C068F42,8C069272 */
} tPoly;

typedef union tViewport { struct { float vscale[4]; float vtrans[4]; } vp; } tViewport;
typedef struct tScissor { unsigned short ulx,uly,lrx,lry; } tScissor;
typedef struct tWindowInfo {
    tViewport *vp;         /* 8C06942C +0 pointer */
    tScissor *s;           /* 8C06942E +4 pointer */
    unsigned int flags;   /* 8C069608 +8 word */
    float whviewang;       /* 8C069616 +0C */
    float wvviewang;       /* 8C06961A +10 */
    float whvangtan;       /* 8C069626 +14 */
    float wvvangtan;       /* 8C069632 +18 */
    float wihvangtan;      /* 8C06964C +1C */
    float wivvangtan;      /* 8C069642 +20 */
    float wwidth;          /* 8C0695C2 +24 */
    float wheight;         /* 8C0695C6 +28 */
    float whcenter;        /* 8C0695CA +2C */
    float wvcenter;        /* 8C0695CE +30 */
    float aspect;          /* 8C0695D4 +34 */
    float frontclip;       /* 8C069434 +38 */
    float backclip;        /* 8C06943E +3C */
    signed short fogmin;  /* 8C06946C +40 word */
    unsigned short fogmax;/* 8C069470 +42 word */
    unsigned char fogcolor[4]; /* 8C069476..486 +44..47 bytes */
} tWindowInfo;             /* stride72, ScaleWindows 8C069590 add#72. */

typedef tWindowInfo dcSkyWindowInfo;
#endif /* DC_TYPES_H */
