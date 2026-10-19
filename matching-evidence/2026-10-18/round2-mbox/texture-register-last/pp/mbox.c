/* mbox.c - model box: object and texture lookup by name, and object hiding.
   Original: D:\rush_ngc\Rush_NGC\GameSource\src\mbox.c; assembly: build/2049LE/asm/GameSource/src/mbox.s */
#line 1 "mbox_private.h"
#line 1 "dc_types.h"
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

 char pad0[0xc];
 float invi[3]; /* +00C: calcaa50AE/50B2(+c),50B4/50BC(+10),50C6/50CA(+14). */
 float springrate[4]; /* +018: 3EFE mov#18,3F04 loadFR7; following tire calls+4. */
 float farspringrate; /* +028: 3EF4 mov#28/3EFA loadFR8. */
 char pad1[0x4];
 float cdamping[4]; /* +030: 3EE6 mov#30/3EF0 loadFR9; following tire calls+4. */
 float rdamping[4]; /* +040: 3EF8 add#40/3EFC loadFR10; following tire calls+4. */
 char pad2[0x8];
 float srefpcybo2; /* +058: 4166 mov#58/load4168. */
 float rollresist; /* +05C: 417C mov#5c/load4180. */
 char pad3[0x10];

    float tireR[4][3]; /* road8C04AC44..AC52: +70. */
    float enginvmi;         /* 0xa0  whatslips pool 8C022EE8 (0x00a0), mov.l @r4 = m->car */
    float dwratio;          /* 0xa4  transmission pool 8C022FDE (0x00a4), find_best_gear 8C0230A2 */
    float clutchmaxt;       /* 0xa8  whatslips pool 8C022EE2 (0x00a8) */
    float upshiftangvel;    /* 0xac  autoshift pool 8C022FD4 (0x00ac) */
    float downshiftangvel;  /* 0xb0  autoshift 8C022F28 add #4 from upshiftangvel */
} tCar;

/* ------------------------------------------------------------------ tTune (MAT3 anon_diffbl_weight_scale_2c) */
typedef struct tTune {
    float diffbl; /* drivsym42E0/42E6. */
    char pad0[4];
    float torque_scale;     /* 0x08  enginetorque (drivetra_part), m->tune->torque_scale */
    float trans_scale;      /* 0x0c  transmission 8C022FB6 mov #0xc; fmov.s @(r0,r3) */
    float clutchmaxtk;      /* 0x10  whatslips 8C022E44 mov #0x10 */
    float shiftk;           /* 0x14  autoshift 8C022F16 mov #0x14 */
    char pad_antispin[0x20-0x18];
    float skidk; /* mdrive F96E..972: tune+20. */
    float antispin; /* drivsym39F4/39FC. */
} tTune;

/* ------------------------------------------------------------------ tTire (MAT3 anon_tradius_springK_5c) */
typedef struct tTire {
    float tradius; /* tire+0: rekon 8C047DA4/DB2. */
    char pad0[0x14-4];
    float invmi;            /* 0x14  drivetrain pool 8C022DCE (0x04fc = 0x430 + 2*0x5c + 0x14), 8C022DD0 (0x0558) */
    char pad1[0x48 - 0x18];
    float angvel;           /* 0x48  drivetrain 8C022CC0..CCA: base 0x430, 0x15c = 3*0x5c+0x48, 0x100 = 2*0x5c+0x48 */
    char pad2[0x50-0x4c];
    float sideforce,traction; /* mdrive F8CC..F948: tire+50/+54. */
    signed char slipflag; /* mdrive: tire+58, stride5c. */
    char pad_slip[3];
} tTire;

/* ------------------------------------------------------------------ tFastIn (MAT3 anon_modelIteration_modelTime_10) */
/* tFastIn occupies model+710..720; modelrun is at+71c. */
typedef struct tFastIn {
    int modelIteration; /* drivsym34C8. */
    float modelTime;        /* 0x04  upshift/downshift pool 8C0230A8 (0x0714 = fastin 0x710 + 4) */
    float dt; /* drivsym33C8,model+718. */
    signed short modelrun; /* mdrive, model+71c; natural tail alignment to720. */
} tFastIn;

typedef struct tRekonState {
    float base_rwa[3]; /* model+734,738,73c: pools8C047EDA,EE0,EE4 */
    float base_rwv[3]; /* model+740,744,748: pools8C047EEA,EEE,EF2 */
    float base_rwr[3]; /* model+74c,750,754: pools8C047EF6,EFA,EFE */
    signed short rwmph; /* model+758: pool8C047ED0, ftrc then mov.w8C047DBE..DC2 */
    char pad0[2];
    float suscomp[4]; /* model+75c: pool8C047ED2; four-element loop8C047DC4..E0A */
    float airdist[4]; /* model+76c: pool8C047ED6, same loop */
    float rwa[3]; /* model+77c: 8C047E0C..22, destination source+48 */
    float rwv[3]; /* model+788: 8C047E24..3A */
    float rwr[3]; /* model+794: pool8C047D62; 8C047E3C..52 */
    float uvs[3][3]; /* model+7a0: pool8C047D6C,EC6, CopyUV */
} tRekonState;
typedef struct tResurrect {
    float pos[3],save_pos[3],uvs[3][3]; /* model+660/66c/678. */
    float quat_start[4],quat_end[4]; /* model+69c/6ac. */
    int velocity; /* model+6bc: target converts integer to float. */
    float crash_time; /* model+6c0. */
    signed short moving_state; /* model+6c4. */
    char pad0[2];
    float resurrect_time; /* model+6c8. */
    signed char autoAbort,waterAbort; /* model+6cc/6cd. */
    char pad1[2];
} tResurrect;

typedef struct tRoadInit {
    signed short polepos; /* model+6d0. */
    char pad0[2];
    float pos[3];      /* poolsACCE/ACD2/ACD6=6D4/6D8/6DC. */
    float uvs[3][3];   /* poolACC4=6E0, initin base6D0. */
    float offset[3];   /* poolACCC=704. */
} tRoadInit;
typedef struct tMainIn {
 volatile float wheel; /* +720:poolE81C/loadE756/storeE816. */
 volatile float clutch; /* drivsym33CE,model+724. */
 volatile float brake; /* +728:poolE61C,storeE6C4. */
 volatile float throttle; /* +72c:poolE61A,storeE590. */
 volatile signed char gear; /* +730:poolE538,load/storeE49C..4CE. */
 volatile signed char wing; /* +731:poolEBDE/storeEB94. */
 volatile signed char abort; /* +732:poolEBE0/storeEBBC. */
 char pad1[0x14-0x13]; /* mainin region to734; extent named only through732. */
} tMainIn;

typedef struct surftyp {
 unsigned short flags; /* stree PointInSurf and headlights83: +0. */
 unsigned short snd_flags; /* +002: 3B78/3B7A. */
 signed short uvs[3][3]; /* +004: 3A80..3AAC +4/+6/+8 signedshort. */
 unsigned short vli; /* stree PointInSurf,+16. */
} surftyp;

/* ------------------------------------------------------------------ tModelData */
typedef struct tModelData {
    tCar *car;              /* 0x000 whatslips 8C022DEE mov.l @r4,r3 then car fields */
    tTune *tune;            /* 0x004 transmission 8C022FA0 mov.l @(0x4,r4) */
    unsigned char body_type; /* +8: headlights 8C030712/716. */
    signed char trans;      /* 0x009 transmission 8C022F96 mov.b @(0x9,r4) */
    signed char autotrans;  /* 0x00a drivetrain 8C022CAC mov.b @(0xa,r14) */
    signed char handling;   /* 0x00b enginetorque (drivetra_part) */
    signed char engine_type;/* 0x00c enginetorque (drivetra_part) */
    signed char tire_type,frame,tireRims; /* mdrive DADC/DAF2/DB16: model+d/e/f. */
    float f[3],m[3],a[3],aa[3],v[3]; /* drivsym4962..4972,3E4C. */
    float w[3]; /* rekon 8C047D0C..2A. */
    float d[3]; /* drivsym4118..411C. */
    float tireForce[4][3];  /* 0x064 drivetrain 8C022CDE add #0x64; [3][1] at +0x28, [2][1] at +0x1c; pool 8C022DC6 (0x80), 8C022DCA (0x8c) */
    float tireV[4][3]; /* drivsym3FA0/3E1E. */
    float bodyForce[4][3]; /* road8C04B0E6/B0F0. */
    float bodyR[4][3]; /* road pool8C04AEC0. */
 float centerForce[3]; /* +124: pool 8C023832/load 8C0236EC. */
 float centerForceLast[3]; /* +130:38AE base124,38B2 add#c/store38B4; nextcomponents38BC/C4. */
 float centerMoment[3]; /* +13C: pool 8C02499A/load 8C024916. */
 float peak_body_force[2][3]; /* +148: pool 8C023838/load 8C02374C. */
 float peak_center_force[2][3]; /* +160: pool 8C023834/load 8C0236F0. */
 float tireuvs[4][3][3]; /* +178: pool 8C023FA2/load 8C023E22. */
 float g[3]; /* +208: pool 8C02457C/load 8C02442A. */
    float rwa[3]; /* rekon pool 8C047EDC. */
    float rwv[3]; /* rekon pool 8C047D60. */
    float rwr[3]; /* rekon pool 8C047D64. */
    float rwr2[3]; /* drivsym4C70..C92. */
    float tireRwr[4][3],bodyrwr[4][3],bodyrwr2[4][3]; /* road8C04ACE6/AEBC/AEC2. */
    float grw[3],stuntW[3]; /* drivsym457E/4D8A. */
    float uvs[3][3]; /* CopyUV source 8C047D6E. */
    float roaduvs[4][3][3]; /* road8C04B1BE/B13A..148. */
    struct streetyp *lasttp[4]; /* stree pool8C0529B6, model+3a0. */
    float steerangle; /* drivsym36A8/3630. */
    float torque[4];        /* 0x3b4 drivetrain pool 8C022DC8 (0x03bc = torque[2]), add #4 -> torque[3] */
    float steergain; /* drivsym33CC/333C. */
    char pad_clutch[4];
    float clutch;           /* 0x3cc whatslips pool 8C022DD6 (0x03cc) */
    float throttle;         /* 0x3d0 autoshift pool 8C022FD2 (0x03d0) */
    float brake; /* drivsym33D6/3388. */
    char pad_brake[4];
    float brakegain[4]; /* drivsym34CE/346E. */
    char pad_magvel[4];
    float magvel; /* drivsym3A18/396E. */
    signed short gear;      /* 0x3f4 pools 8C022EE4, 8C022FD8, 8C0230A0 (mov.w @(r0,r4)) */
    signed short commandgear; /* 0x3f6 autoshift pool 8C022FD6 (0x03f6) */
    signed short wing; /* stunt55BD8 and drivsym4212. */
    char pad5[2];
    int wingSnd; /* mdrive D9AA/DA62: model+3fc. */
    float catchup; /* model 8C040CD2/8C040C8C. */
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
    unsigned short tiresli[4]; /* skids 8C04EF10..F20. */
    signed char magicdiff;  /* 0x5a8 drivetrain pool 8C022DC2 (0x05a8), mov.b @(r0,r14) */
    signed char nothrusttorque; /* drivsym430E. */
    char pad_drift[2];
    float drift; /* drivsym3A14. */
    char pad_offroad[4];
    float offroadtires,wingEffect,weight,mass; /* drivsym4218/4996/499C/3B28. */
    char pad_suscomp[8];
    float suscomp[4]; /* physical: rekon 8C047ED4. */
    float tpcomp[4]; /* road pool8C04ACEA. */
    float airdist[4]; /* physical: rekon 8C047ED8. */
    float airvel[4]; /* road pool8C04B308. */
    int roadcode[4];        /* 0x60c engine (drivetra_part) */
    unsigned short sviscode[4]; /* skids 8C04EF00..F0C. */
    signed short roadboost[4]; /* road8C04ACF0. */
    unsigned short sound_flags[4]; /* road8C04ACF2. */
    float dt;               /* 0x634 whatslips pool 8C022EEC (0x0634) */
    float idt; /* road8C04B30C/B24A. */
    float crashthreshold; /* drivsym5344/52D6. */
    signed char crashflag;  /* 0x640 engine (drivetra_part) */
    signed char topscrape,thumpflag; /* drivsym543A/52CE. */
    signed char mthumpflag; /* audio pool8C0134AA,model+643. */
    signed short thump_side; /* drivsym5342/52C6. */
    char pad_surface[2];
    surftyp *movingSurface; /* road8C04B1BC/B120. */
    char pad_bodyforces[4];
    float bodyForces; /* drivsym4584/4540. */
    float colrad; /* targets pool8C0588AE,model+654. */
    float shifttime;        /* 0x658 upshift/downshift pool 8C0230A6 (0x0658) */
    signed short bogState; /* drivsym36AE/368E. */
    char pad11[2];
    tResurrect resurrect; /* moving_state+6c4: headlights 8C0305C4/C6. */
    tRoadInit initin; /* road base6D0, pools8C04ACCE..704. */
    tFastIn fastin;         /* 0x710 (modelTime at 0x714, pool 8C0230A8) */
    tMainIn mainin; /* controls wheel720..abort732, region to734. */
    tRekonState reckon; /* +734..7c4, all fields proven in rekon. */
    signed short obResPt; /* model+7c4. */
    signed short slot; /* +7c6: checkpoint 8C01BBBA. */
    signed short in_game; /* +7c8: rekon 8C047C2A. */
    signed short we_control; /* drones pool8C025C0A. */
    signed char drone_type; /* +7cc: checkpoint 8C01C3BC. */
    unsigned char PaintJob; /* mdrive DB20/DB4C: model+7cd. */
    char pad_rpm[2];
    signed short rpm; /* drivsym369E/35EE. */
    char pad_appearance[2];
    unsigned int appearance; /* checkpoint 8C01C28C. */
    float stuck_time; /* model+7d8. */
    signed char started; /* checkpoint 8C01C0DA. */
    signed char data_valid,can_we_cheat; /* model+7dd/7de. */
    signed char hide_car; /* +7df: headlights 8C0305DC/DE. */
    signed short please_abort_me; /* model+7e0. */
    signed short last_cp_index; /* +7e2: checkpoint 8C01C0D8. */
    signed short next_cp_index; /* +7e4: checkpoint 8C01BE80. */
    signed short drone_target; /* drones8C0259D4/5954. */
    signed char laps,cpSign,collidable,solid; /* +7e8..7eb: checkpoint 8C01C0DC/1BFCA/1C28A/1C226. */
    float drone_scale,time_boost; /* drones8C02579E/57A0. */
    float time_fudge; /* +7f4: rekon pools 8C047D5E/EE6. */
    float skidIntensity[4]; /* audio pool8C0124E0,model+7f8; indexed stride808 unchanged. */
} tModelData;

/* Layout self-checks (typedefs only, no code or data). SHC rejects address constants such as
   &((T *)0)->f in array bounds, so only sizes are checked; they catch padding arithmetic errors
   because every overlay ends on its last proven field. */
typedef char dc_check_tTire_size[(sizeof(tTire) == 0x5c) ? 1 : -1];
typedef char dc_check_tModelData_size[(sizeof(tModelData) == 0x808) ? 1 : -1];
typedef char dc_check_tCar_size[(sizeof(tCar) == 0xb4) ? 1 : -1];
typedef char dc_check_tTune_size[(sizeof(tTune) == 0x28) ? 1 : -1];


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
    tUVMatrix fcam; /* camera+60. */
    float view2spring; /* camera8C018E50/8E42,+90. */
    unsigned char slot; /* camera8C018E4E/8E94,+94. */
    char pad0[3];
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
typedef struct tVisual tVisual;
struct tVisual {
    tVisual *next;
    signed short index, objnum, slot;
    unsigned int data;
    float timeStamp;
    void (*func)(tVisual *, signed short);
}; /* 0x18; +6/+8/+14 at 8C030576,5AC,5B0; stride at8C02FE02. */
typedef struct anon_active_invsTime_10 {
    unsigned int active; /* car+3A4: 8C095DAA, read8C095D8C, mask8C095E4C..52 */
    float invsTime,shldTime,shldScale; /* target59 timing stores at3A8/3AC/3B0. */
} anon_active_invsTime_10;
typedef struct anon_weapon_health_30 {
    struct { signed char type,ammoCnt; } weapon; /* targets battle+0/+1. */
    signed short health; /* targets battle+2. */
    signed short damageThisFrame; /* mdrive EC2C/ECFE: battle+4. */
    char pad0[2];
    anon_active_invsTime_10 defense;
    signed char shoot; /* car+3B4: 8C095DAE, 8C095E58 */
    unsigned char fadeAlpha; /* car+3B5: 8C095DB0, extu.b8C095E04 */
    signed char fadeState; /* car+3B6: 8C095DAC */
    signed char Points; /* gameover8C02A34C..350,car+3b7. */
    char pad1[0x20-0x1c];
    float fadeTime; /* car+3BC: 8C095E8A, 8C095DF2/DF4 */
    float reloadingTime; /* car+3c0, battle+24. */
    char pad2[0x30-0x28];
} anon_weapon_health_30;
typedef struct tPathPoint {
    float xrel,yrel,cyrel,len,tgtspd,tgtpos[3],interval_time;
    signed short idx,newIdx,path,basePath;
} tPathPoint; /* car+32c..358; maxpath/resurrect target offsets. */

typedef struct tCarData {
    int modelIteration; /* mdrive F078..F084: car+0. */
    float modelTime; /* checkpoint 8C01BF02. */
    float dr_pos[3];       /* 8C030770/780/792 +8/C/10. */
    float dr_vel[3]; /* smoke8C04F712..728. */
    float dr_acc[3]; /* mdrive F3C6..F3DC: car+20/24/28. */
    float dr_uvs[3][3];    /* 8C02FFCA base+2C; row2 at+44/+48/+4C. */
    float dr_car_uvs[3][3]; /* camera View3Cam,+50. */
    float dr_tirepos[4][3]; /* skids 8C04E8C2. */
    float v[3]; /* camera RearCamera,+a4. */
    float tireR[4][3]; /* mdrive F29E..F35E: car+b0, stridec. */
    float pitch,roll; /* mdrive F26E/F402: car+e0/e4. */
    unsigned int appearance; /* 8C0305D4/5D6. */
    signed char data_valid,restart; /* car+ec/ed. */
    signed char place,place_locked; /* +ee/+ef: checkpoint 8C01BCF0/BCEC. */
    float score; /* +f0: checkpoint 8C01C3B6. */
    int objnum; /* camera,+f4. */
    signed short rwmph; /* smoke8C04F6B4 and controls8C01E53A. */
    signed short lastResPath,lastResIdx,resIdx; /* car+fa/fc/fe. */
    float distance; /* +100: checkpoint 8C01BCEE. */
    float last_distance; /* car+104. */
    float mileage; /* +108: checkpoint 8C01BBB6. */
    float crashtime; /* car+10c. */
    tVisual visuals[22];   /* 8C02FDE4+FDEC =+110; visual[1] adds18. */
    signed char collidable,solid; /* checkpoint 8C01C286/C212. */
    char pad4[2];
    float collide_time; /* +324: checkpoint 8C01C288. */
    signed char collide_count; /* car+328. */
    signed char collide_state; /* +329: checkpoint 8C01C21A. */
    char pad_mpctl[2];
    tPathPoint mpctl; /* camera+32c,idx350/path354. */
    float fwd_progress_t; /* car+358. */
    unsigned short sound_flags[4]; /* car+35c. */
    char pad_crash[0x36e-0x364];
    signed short weight_index; /* drones8C025798/56C2. */
    signed char crash_state,dead; /* +370/+371: checkpoint 8C01BE7C/1BBBC. */
    signed char in_tunnel; /* drivsym420C/4126. */
    signed char slot; /* camera,+373. */
    signed char camera,view; /* smoke8C05066E/670. */
    signed char saveview; /* camera,+376. */
    signed char carMine; /* targets8C059DD4,+377. */
    char pad_stunt[0x37c-0x378];
    float strayAngle; /* car+37c. */
    float stuntW[3]; /* stunt553C6/C8/CA. */
    char pad_player[0x398-0x38c];
    struct tPadState *player; /* drivsym4994/48BE..48E0. */
    anon_weapon_health_30 battle; /* powerups +39c, stride3cc. */
} tCarData;

typedef struct anon_woff_wname_14 {
    unsigned int woff; /* +0:8C065E0E */
    char wname[16]; /* +4:8C065DBA/C0,8C065DD6; size20:8C065DDA */
} anon_woff_wname_14;
typedef struct anon_tag_address_c {
    unsigned int tag;        /* +0: MBFindInWad8C03D036 */
    unsigned int address;    /* +4: MBSetupWad8C03CFC8,MBGet8C03D088 */
    unsigned int length;     /* +8: MBGetFromWad8C03D084 */
} anon_tag_address_c;
typedef struct anon_base_TOCentries_c {
    unsigned int *base;      /* +0: MBSetupWad8C03CF9C */
    int TOCentries;          /* +4: MBSetupWad8C03CFA6 */
    anon_tag_address_c *TOC; /* +8: MBSetupWad8C03CFA0/CFAC */
} anon_base_TOCentries_c;

typedef struct tPS_File tPS_File;
typedef struct tCircuit {
    char pad0[4];
    unsigned char active; /* +4:8C01C5C2 */
    unsigned char death; /* +5:8C01C5D0 */
    unsigned char temp; /* +6:8C01D354 */
    unsigned char difficulty; /* +7:8C01C5DA */
    signed char type; /* +8:8C01C5B6 */
    signed char track; /* +9:8C01CF04/08 */
    char pad1[2];
    unsigned int seed; /* +c:8C01C5BE */
    float totalTime; /* +10:8C01CF0C/12 */
    unsigned char ranks[6][9]; /* +14,row9:8C01CE40..6A;6car loop */
} tCircuit;
typedef struct tSaveData {
    char pad0[0x6f4];
    tCircuit circuit; /* +6f4:word8C01C686 */
} tSaveData;
typedef struct tCircuitCar {
    unsigned char name; /* +0:8C01C6DC */
    unsigned char totalPlace; /* +1:8C01CF66 */
    unsigned short totalPoints; /* +2:8C01CB08 */
    unsigned char places[24]; /* +4:8C01CB4C..52 */
    unsigned char points[24]; /* +1c:8C01CB54..5C */
    signed short skill[12]; /* +34:8C01C7A4..A8 */
} tCircuitCar;
typedef struct tCircuitTrack {
    unsigned char trackNum; /* +0:8C01C8BE */
    unsigned char backward; /* +1:8C01C8D8 */
    unsigned char mirror; /* +2:8C01C8F4 */
    unsigned char fog; /* +3:8C01CA6E */
    unsigned char wind; /* +4:8C01CA9C */
} tCircuitTrack;

typedef struct tPlayerProfile {
    char pad0[8];
    tPS_File **fileh; /* +8: 8C050EF2 mov.l @(8,r4),r4. */
    char pad_name[8];
    unsigned char name[13]; /* gameover8C029B14/16,+14,ghost bound13. */
    signed char controls[11]; /* profile+21..2b, controlscr byte accesses. */
    signed char rumble; /* controls8C01ED3C. */
    char pad_datah[0x3c-0x2d];
    tSaveData **datah; /* circuit8C01C57E, profile+3c. */
    /* Profile stride40; camera accesses the same datah handle at+3c. */
} tPlayerProfile;
typedef struct tPadState {
    unsigned char slot; /* checkpoint 8C01BF66..6A; demo byte stores. */
    unsigned char controller; /* controls8C01E472/476. */
    char pad0[2];
    unsigned int edges,levels,autoEdges; /* controls+4/+8/+c. */
    float analog[2]; /* controls+10/+14. */
    unsigned int controls[11]; /* controls8C01EDA8 +18,stride4. */
    unsigned short numCrashes; /* player+44. */
    unsigned char trans; /* mdrive DAA8/DAB2: player+46. */
    char pad_controls[1];
    unsigned int listener; /* audio184A/184C,+48. */
    tPlayerProfile **save; /* state 8C050EE0..EE2; stride50. */
} tPadState;
typedef char dc_check_full_model[(sizeof(tModelData)==0x808)?1:-1];
typedef char dc_check_car_data[(sizeof(tCarData)==0x3cc)?1:-1];
typedef char dc_check_pad_state[(sizeof(tPadState)==0x50)?1:-1];

typedef struct tZoidObjectDC {
    unsigned int flags; /* headlights8C03065E..662. */
    char pad0[4];
    tUVMatrix *mat; /* mbox8C03D2D8. */
    float scale,w; /* smoke8C05001C..24 and8C04FC80..8E. */
    char pad_child[0x2a-0x14];
    signed short child,next; /* mbox8C03D114/11C,3D182/18A. */
    char pad_texture[0x34-0x2e];
    tTexture *texture[4];  /* 8C069300..308 stores +34/+38/+3C/+40. */
    char pad1[0x48 - 0x44];
    void *field48;         /* 8C06931E r0=48; store at 8C069326. */
    void *palette;         /* 8C0692E6 r0=4C; store at 8C0692EE. */
    tRGBA color;           /* 8C0692B4 r0=50; word store at 8C0692BC. */
    tRGBA envcolor;        /* 8C0692D6 r0=54; word store at 8C0692DE. */
} tZoidObjectDC;

typedef struct _tBlit _tBlit;
struct _tBlit {
    const char *name; /* +0:8C018898 */
    void *palette; /* +4:8C0188AE */
    tTexture *texture; /* +8:8C0188A6 */
    unsigned short texIndex; /* +c:8C0188A4 */
    signed short x; /* +e:8C018A0C */
    signed short y; /* +10:8C018A10 */
    unsigned short z; /* +12:8C0188EE */
    signed short width; /* +14:8C0188E2 */
    signed short height; /* +16:8C0188E6 */
    unsigned char alpha; /* +18:8C0188F4 */
    unsigned char flip; /* +19:8C018A18 */
    signed char hide; /* +1a:8C018A14 */
    signed char init; /* +1b:8C018A1E */
    signed short top; /* +1c:8C0188F8 */
    signed short bot; /* +1e:8C0188FA */
    signed short left; /* +20:8C0188FE */
    signed short right; /* +22:8C018902 */
    signed short color; /* +24:8C018906 */
    char pad0[2];
    int (*animFunc)(_tBlit *); /* +28:8C018842 */
    unsigned int animID; /* +2c:8C018A24 */
    int animDTA; /* +30:8C018A26 */
    unsigned short blidx; /* +34:8C0187F2/7F6 */
    char pad1[2];
    void *data; /* +38:8C018A28 */
    _tBlit *child; /* +3c:8C018814 */
}; /* stride40:8C0187A2/7AC; bound200:8C01887A/9B2 */
typedef struct tBlitDef {
    const char *texname; /* +0:8C018BAA */
    signed short dulx,duly; /* +4/+6:8C018BA4/8B98 */
    signed short width,height; /* +8/+a:8C018BD0/8BF6 */
    signed short top,bot; /* +c/+e:8C018BC2/8BCC */
    signed short left,right; /* +10/+12:8C018BBC/8BC8 */
    unsigned int zdepth; /* +14:8C018BB6 narrowword store; front descriptor evidence */
    unsigned int alpha; /* +18:8C018BAC */
    int (*animfunc)(_tBlit *); /* +1c:8C018C06 */
    unsigned int animid; /* +20:8C018B9A */
} tBlitDef; /* stride24:8C018B86..96 */

typedef struct tPadInfo {
 signed char attached,incompatible; /* F266/F326/F332 */
 unsigned int availButtons,buttons; /* F282/F3A4,+4/+8 */
 signed char stick1x,stick1y,stick2x,stick2y; /* F43E/F450/F47E */
 unsigned char analog1,analog2; /* F486/F48E;+10/+11 */
} tPadInfo;

typedef struct CarParts {
 int frame; /* visualsEE7C. */
 char pad04[4];
 int tsheen; /* visualsEF0E,+8. */
 char pad0c[8];
 int headlight[4]; /* headlightsFE3A/58/94/B2,+14. */
 int extraparts[3]; /* headlights306E8,+24. */
 int hood,sheen; /* visualsEE7C..EEBA/EEE2,+30/+34. */
 char pad38[4];
 int tires[4]; /* visuals,+3c. */
 char pad4c[0x60-0x4c];
} CarParts;

#endif /* DC_TYPES_H */
#line 2 "mbox_private.h"
/* Private C89 overlays: offsets and widths from the DC mbox instruction stream. */
#ifndef MBOX_PRIVATE_H
#define MBOX_PRIVATE_H
typedef tZoidObjectDC tMboxInstance;

           /* stride88 from targetD0C8/D0CC andD2BC/D2BE. */
typedef struct tModelDef {
    char name[16];         /* comparator name at+0, fixed copy16 atD3D6. */
    float radius;          /* D386..D38A: float+10. */
    char pad0[0x48-0x14];
} tModelDef;               /* D41C bsearch size72 and D476 pointer divide72. */
                /* D50A search size48; stackkey size48 atD4BA. */
typedef struct tMboxPalette {
    char name[16];
    char pad0[0x18-0x10];
} tMboxPalette;            /* D5CE search size24; stackkey size24 atD57E. */
typedef struct anon_romObjDefs_count_8 { tModelDef *romObjDefs; int count; } anon_romObjDefs_count_8;
typedef struct anon_texDefs_count_8 { tTexture *texDefs; int count; } anon_texDefs_count_8;
typedef struct anon_palDefs_count_8 { tMboxPalette *palDefs; int count; } anon_palDefs_count_8;
extern tMboxInstance ObjInstanceList[]; /* target poolD1BC/D340=8C160870. */
signed short ZOID_AddObject(int objidx,tUVMatrix *mat,signed short parent,unsigned int flags);
unsigned int ZOID_ObjectFlags(signed short objnum);
void ZOID_SetObjectFlags(int objnum,unsigned int flags);
signed short ZOID_TrueObjParent(signed short objnum);
int ModelActive(int idx);
void *bsearch(void *key,void *base,unsigned long nmemb,unsigned long size,int (*compar)(void *,void *));
char *strcpy(char *dest,const char *src);
char *strncpy(char *dest,const char *src,unsigned int count);
int strcmp(const char *a,const char *b);
int strncmp(const char *a,const char *b,unsigned int count);
/* SHC builtins observed in the installed front end; target dynamic strcpy
 * uses r0/r1 (8C03D4C0..4CC), and constant copy is inline (D3B4..D3CC). */
#define strcpy _builtin_strcpy
#define strcmp _builtin_strcmp
typedef char checkMboxInstance[(sizeof(tMboxInstance)==0x58)?1:-1];
typedef char checkMboxModel[(sizeof(tModelDef)==0x48)?1:-1];
typedef char checkMboxTexture[(sizeof(tTexture)==0x30)?1:-1];
typedef char checkMboxPalette[(sizeof(tMboxPalette)==0x18)?1:-1];
#endif
#line 4 "mbox.c"

anon_romObjDefs_count_8 gRomObjDefPtrs[64] = { 0 };
anon_texDefs_count_8 gTexDefPtrs[64];
anon_palDefs_count_8 gPalDefPtrs[64];
unsigned char gNumModels;

#pragma inline(MBOX_GetChild)
#pragma inline(MBOX_GetNext)

static int objcmp(const tModelDef *obj1, const tModelDef *obj2);
static int texcmp(const tTexture *ta, const tTexture *tb);

/* 0x80028b40, size 0x24 */
signed short MBOX_NewObject(int obj, tUVMatrix *mat, int parent, unsigned int flags)
{
    signed short dlidx; /* r0 */
    dlidx = ZOID_AddObject(obj, mat, parent, flags);
    return dlidx;
}

/* 0x80028b64, size 0x38 */
signed short MBOX_NewObjectCam(int obj, tUVMatrix *mat, signed short camlink, int parent, unsigned int flags)
{
    signed short dlidx; /* r0 */
    unsigned int camflags;
    camflags = (camlink << 8) ^ 0xf00;
    dlidx = ZOID_AddObject(obj, mat, parent, flags | camflags);
    return dlidx;
}

/* 0x80028b9c, size 0x18 */
int MBOX_GetChild(int dlidx)
{
    return ObjInstanceList[dlidx].child;
}

/* 0x80028bb4, size 0x18 */
int MBOX_GetNext(int dlidx)
{
    return ObjInstanceList[dlidx].next;
}

/* 0x80028bcc, size 0x114 */
void MBOX_HideObject(int dlidx, int mode, int camlink)
{
    unsigned int flags; /* r4 */
    int tmpidx; /* r0 */
    if (mode == 2) {
        flags = ZOID_ObjectFlags(dlidx);
        flags |= camlink << 8;
        ZOID_SetObjectFlags(dlidx, flags);
        tmpidx = MBOX_GetChild(dlidx);
        if (tmpidx != -1) {
            MBOX_HideObject(tmpidx, 3, camlink);
        }
    } else if (mode == 0) {
        flags = ZOID_ObjectFlags(dlidx);
        flags |= 0x80000000;
        ZOID_SetObjectFlags(dlidx, flags);
    } else if (mode == 1) {
        flags = ZOID_ObjectFlags(dlidx);
        flags |= camlink << 8;
        ZOID_SetObjectFlags(dlidx, flags);
    } else if (mode == 3) {
        do {
            flags = ZOID_ObjectFlags(dlidx);
            flags |= camlink << 8;
            ZOID_SetObjectFlags(dlidx, flags);
            tmpidx = MBOX_GetChild(dlidx);
            if (tmpidx != -1) {
                MBOX_HideObject(tmpidx, 3, camlink);
            }
        } while ((dlidx = MBOX_GetNext(dlidx)) != -1);
    }
}

/* 0x80028ce0, size 0x124 */
void MBOX_ShowObject(int dlidx, int mode, int camlink)
{
    unsigned int flags; /* r4 */
    int tmpidx; /* r0 */
    if (mode == 1) {
        flags = ZOID_ObjectFlags(dlidx);
        flags &= ~((camlink << 8) | 0x80000000);
        ZOID_SetObjectFlags(dlidx, flags);
        tmpidx = MBOX_GetChild(dlidx);
        if (tmpidx != -1) {
            MBOX_ShowObject(tmpidx, 2, camlink);
        }
    } else if (mode == 0) {
        flags = ZOID_ObjectFlags(dlidx);
        flags &= ~((camlink << 8) | 0x80000000);
        ZOID_SetObjectFlags(dlidx, flags);
    } else if (mode == 0) {
        /* unreachable: tests mode == 0 a second time */
        flags = ZOID_ObjectFlags(dlidx);
        flags &= ~0x80000000;
        flags |= camlink << 8;
        ZOID_SetObjectFlags(dlidx, flags);
    } else if (mode == 2) {
        do {
            flags = ZOID_ObjectFlags(dlidx);
            flags &= ~((camlink << 8) | 0x80000000);
            ZOID_SetObjectFlags(dlidx, flags);
            tmpidx = MBOX_GetChild(dlidx);
            if (tmpidx != -1) {
                MBOX_ShowObject(tmpidx, 2, camlink);
            }
            dlidx = MBOX_GetNext(dlidx);
        } while (dlidx != -1);
    }
}

/* 0x80028e04, size 0x3c */
static int objcmp(const tModelDef *obj1, const tModelDef *obj2)
{
    if (!obj1 || !obj2) {
        return -1;
    }
    return strncmp(obj1->name, obj2->name, 15);
}

/* 0x80028e40, size 0xc4 */
void MBOX_GetPosition(float *pos, int dlidx)
{
    pos[0] = ObjInstanceList[dlidx].mat->uvs[3][0];
    pos[1] = ObjInstanceList[dlidx].mat->uvs[3][1];
    pos[2] = ObjInstanceList[dlidx].mat->uvs[3][2];
    while ((dlidx = ZOID_TrueObjParent(dlidx)) != -1) {
        pos[0] = ObjInstanceList[dlidx].mat->uvs[3][0] + pos[0];
        pos[1] = ObjInstanceList[dlidx].mat->uvs[3][1] + pos[1];
        pos[2] = ObjInstanceList[dlidx].mat->uvs[3][2] + pos[2];
    }
}

/* 0x80028f04, size 0x54 */
float MBOX_GetObjectRadius(int mbidx)
{
    int model_num; /* r4 */
    int object_num; /* r6 */
    model_num = mbidx >> 10;
    object_num = mbidx & 0x3ff;
    if (model_num >= gNumModels) {
        return 0.0f;
    }
    if (object_num >= gRomObjDefPtrs[model_num].count) {
        return 0.0f;
    }
    return gRomObjDefPtrs[model_num].romObjDefs[object_num].radius;
}

/* 0x80028f58, size 0x140 */
/* Callers pass a fourth argument (see overrides.json); the DWARF definition has three parameters. */
int MBOX_FindObject_WORK(const char *name, signed char firstModel, signed char lastModel, int flag)
{
    int i; /* r27 */
    tModelDef odef; /* stack */
    tModelDef *ptr; /* r31 */
    ptr = 0;
    if (!name || !name[0]) {
        strcpy(odef.name, "AAANULLOBJ");
    } else {
        strncpy(odef.name, name, 16);
    }
    if (firstModel < 0) {
        firstModel = 0;
    }
    if (lastModel < 0 || lastModel >= gNumModels) {
        lastModel = gNumModels - 1;
    }
    for (i = firstModel; i <= lastModel; i++) {
        if (ModelActive(i)) {
            if ((ptr = bsearch(&odef, gRomObjDefPtrs[i].romObjDefs, gRomObjDefPtrs[i].count, sizeof(tModelDef),
                          (int (*)(void *, void *))objcmp)) != 0) {
                break;
            }
        }
    }
    if (!ptr) {
        if (flag) { }
        return -1;
    }
    return (ptr - gRomObjDefPtrs[i].romObjDefs) | (i << 10);
}

/* 0x80029098, size 0x20 */
static int texcmp(const tTexture *ta, const tTexture *tb)
{
    return strcmp(ta->name, tb->name);
}

/* 0x800290b8, size 0x128 */
/* Callers pass a fifth argument (see overrides.json). */
tTexture *MBOX_FindTexture_WORK(const char *name, unsigned short *index, signed char firstModel, register signed char lastModel, int flag)
{
    int i; /* r26 */
    tTexture ti; /* stack */
    tTexture *ptr; /* r31 */
    ptr = 0;
    strcpy(ti.name, name);
    if (firstModel < 0) {
        firstModel = 0;
    }
    if (lastModel < 0 || lastModel >= gNumModels) {
        lastModel = gNumModels - 1;
    }
    for (i = firstModel; i <= lastModel; i++) {
        if (ModelActive(i)) {
            if ((ptr = bsearch(&ti, gTexDefPtrs[i].texDefs, gTexDefPtrs[i].count, sizeof(tTexture),
                          (int (*)(void *, void *))texcmp)) != 0) {
                break;
            }
        }
    }
    if (!ptr) {
        *index = 0;
        if (flag) { }
        return 0;
    }
    *index = (ptr - gTexDefPtrs[i].texDefs) | (i << 10);
    return ptr;
}


/* Anonymous mapped entry: target-proven 24-byte record search. */
tMboxPalette *func_8C03D56E(const char *name, unsigned short *index, signed char firstModel, signed char lastModel, int flag)
{
    int i; /* r26 */
    tMboxPalette ti; /* stack */
    tMboxPalette *ptr; /* r31 */
    ptr = 0;
    strcpy(ti.name, name);
    if (firstModel < 0) {
        firstModel = 0;
    }
    if (lastModel < 0 || lastModel >= gNumModels) {
        lastModel = gNumModels - 1;
    }
    for (i = firstModel; i <= lastModel; i++) {
        if (ModelActive(i)) {
            if ((ptr = bsearch(&ti, gPalDefPtrs[i].palDefs, gPalDefPtrs[i].count, sizeof(tMboxPalette),
                          (int (*)(void *, void *))texcmp)) != 0) {
                break;
            }
        }
    }
    if (!ptr) {
        *index = 0;
        if (flag) { }
        return 0;
    }
    *index = (ptr - gPalDefPtrs[i].palDefs) | (i << 10);
    return ptr;
}

