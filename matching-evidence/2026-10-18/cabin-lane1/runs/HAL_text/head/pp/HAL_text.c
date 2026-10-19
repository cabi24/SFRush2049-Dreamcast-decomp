/* HAL_text.c - private target-backed DC candidate; MAT3 read-only semantic reference. */
#line 1 "HAL_text_types.h"
#ifndef HAL_TEXT_TYPES_H
#define HAL_TEXT_TYPES_H
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
#line 4 "HAL_text_types.h"
/* Font/glyph objects below describe externally supplied wave data; they allocate no storage.
   Unused labels version/type/totalHeight and unknown tail bytes are semantic reference names,
   not independently recovered field identities. Font header stride16: targetF0BA; byte fields+2/+3 F3B4/F3DC,+4 F32C,+8 F364,
   +9 F38C,+A F0D2,+B F154,+C F0C4. Glyph stride12 F0CA..0CE; value+4 F13C.
   Glyph texIdx+5/UV+6..9: _DrawTextFA04..FC3E. */
typedef struct anon_version_type_10 {
    unsigned char version; /* 0x0 */
    unsigned char type; /* 0x1 */
    unsigned char ascent; /* 0x2 */
    unsigned char descent; /* 0x3 */
    unsigned char leading; /* 0x4 */
    unsigned char totalHeight; /* 0x5 */
    unsigned char maxWidth; /* 0x6 */
    unsigned char spaceWidth; /* 0x7 */
    unsigned char spacing; /* 0x8 */
    unsigned char nonProp; /* 0x9 */
    unsigned char kern; /* 0xa */
    unsigned char numTextures; /* 0xb */
    unsigned char numChars; /* 0xc */
    unsigned char pad16[3]; /* 0xd */
} anon_version_type_10;
typedef struct anon_kerns_value_c {
    unsigned char *kerns; /* 0x0 */
    unsigned char value; /* 0x4 */
    unsigned char texIdx; /* 0x5 */
    unsigned char uls; /* 0x6 */
    unsigned char ult; /* 0x7 */
    unsigned char lrs; /* 0x8 */
    unsigned char lrt; /* 0x9 */
    signed short pad12; /* 0xa */
} anon_kerns_value_c;
typedef union anon_rgba_rgbaU32_4 {
    unsigned char rgba[4]; /* 0x0 */
    unsigned int rgbaU32; /* 0x0 */
} anon_rgba_rgbaU32_4;
typedef struct anon_fore_4 {
    anon_rgba_rgbaU32_4 fore; /* 0x0 */
} anon_fore_4;
typedef struct tFontTexDefs { tTexture *texDefs; char pad[4]; } tFontTexDefs; /* targetF17A index*8,+0 ptr; unknown secondword, no local allocation */
extern tFontTexDefs gTexDefPtrs[];
unsigned int SetInterruptMask(unsigned int);
void *mem_func_8C03FD8A(void *,unsigned int,void *,int);
int FindWaveInstance(int);
int LoadWave(int,int,int,int,void *);
void LockWaveInstance(int);
void UnlockWaveInstance(int);
void *GetWaveInstanceAddr(int);
void BlitClearColors(void);
void BlitSetAttribute(unsigned int);
void BlitClearAttribute(unsigned int);
void BlitZDist(unsigned short);
void BlitXLU(unsigned short);
void BlitScissor(int,int,int,int);
void BlitColors(void *,int,int,int);
void BlitDraw(tTexture *,int,int,int,int,int,int);
void *memcpy(void *,const void *,unsigned int);
char *strcpy(char *,const char *);
char *strncpy(char *,const char *,unsigned int);
typedef char checkFontHeader[(sizeof(anon_version_type_10)==16)?1:-1];
typedef char checkGlyph[(sizeof(anon_kerns_value_c)==12)?1:-1];
typedef char checkFontColor[(sizeof(anon_fore_4)==4)?1:-1];
#endif
#line 3 "HAL_text.c"
/* Real uninitialized globals are referenced individually; this unit allocates no BSS. */
#line 1 "HAL_text_globals.h"
#ifndef HAL_TEXT_GLOBALS_H
#define HAL_TEXT_GLOBALS_H
/* Target-backed references, not definitions or placement devices.
   133944..133948 remains unidentified and unallocated by this unit.
   Array bounds follow MAT3 object declarations plus DC index/limit evidence;
   no complete original BSS allocation/end is asserted. */
extern unsigned int halTextOldMask; /* target 8C133938 */
#define gOldMask halTextOldMask
extern int halTextFontDatWaveInst; /* target 8C13393C */
#define gFontDatWaveInst halTextFontDatWaveInst
extern int halTextFontTexWaveInst; /* target 8C133940 */
#define gFontTexWaveInst halTextFontTexWaveInst
extern anon_version_type_10 * halTextFontHeader; /* target 8C133948 */
#define gFontHeader halTextFontHeader
extern anon_kerns_value_c * halTextFontLetters; /* target 8C13394C */
#define gFontLetters halTextFontLetters
extern tTexture * halTextFontTextures[16]; /* target 8C133950 */
#define gFontTextures halTextFontTextures
extern signed short halTextCharMap[256]; /* target 8C133990 */
#define gCharMap halTextCharMap
extern anon_fore_4 halTextFontColor; /* target 8C133B90 */
#define gFontColor halTextFontColor
extern signed char halTextLeading; /* target 8C133B94 */
#define gLeading halTextLeading
extern signed char halTextSpacing; /* target 8C133B95 */
#define gSpacing halTextSpacing
extern int halTextFontAttribute; /* target 8C133B98 */
#define gFontAttribute halTextFontAttribute
extern signed short halTextFontXpos; /* target 8C133B9C */
#define gFontXpos halTextFontXpos
extern signed short halTextFontYpos; /* target 8C133B9E */
#define gFontYpos halTextFontYpos
extern signed char halTextFontIndex; /* target 8C133BA0 */
#define gFontIndex halTextFontIndex
extern int halTextBufLen; /* target 8C133BA4 */
#define gBufLen halTextBufLen
extern unsigned char halTextBuffer[800]; /* target 8C133BA8 */
#define gBuffer halTextBuffer
extern anon_fore_4 halTextLastFontColor; /* target 8C133EC8 */
#define lastFontColor halTextLastFontColor
extern float halTextLastTrans; /* target 8C133ECC */
#define lastTrans halTextLastTrans
extern signed short halTextLastFontXpos; /* target 8C133ED0 */
#define lastFontXpos halTextLastFontXpos
extern signed short halTextLastFontYpos; /* target 8C133ED2 */
#define lastFontYpos halTextLastFontYpos
extern signed char halTextLastFontIndex; /* target 8C133ED4 */
#define lastFontIndex halTextLastFontIndex
extern signed char halTextLastLeading; /* target 8C133ED5 */
#define lastLeading halTextLastLeading
extern signed char halTextLastSpacing; /* target 8C133ED6 */
#define lastSpacing halTextLastSpacing
#endif
#line 5 "HAL_text.c"
static signed char gFinished=1;
static void *gFontHeap=0;
static float gDepth=-1.0f;
static float gTrans=-1.0f;
static signed char gBufferInited=0;

void HAL_InitText(void);
void HAL_StartText(void);
void HAL_FinishText(signed char end);
int _HAL_SetFont(int fontID);
int HAL_SetFont(int fontID);
void HAL_PurgeFonts(void);
void _SetupFontData(int force);
void HAL_SetTextColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void HAL_SetTextDepth(float depth);
void HAL_SetMenuTextDepth(float depth);
void HAL_SetTextTranslucency(float xlu);
void HAL_SetTextPos(signed short x, signed short y);
void HAL_SetFontLeading(signed short leading);
void HAL_SetFontSpacing(signed short spacing);
signed char HAL_SetFontFixedWidth(signed char fixedWidth);
unsigned char HAL_GetFontAscent(void);
unsigned char HAL_GetFontDescent(void);
signed char HAL_GetFontLeading(void);
signed char HAL_GetFontSpacing(void);
void HAL_GetFontChars(unsigned char *out);
unsigned int HAL_GetCharWidth(unsigned short ch);
unsigned int HAL_GetTextWidth(void *text, signed short numChars);
void HAL_DrawText(void *text, signed short numChars);
void _DrawText(const unsigned char *text, unsigned short length);
void _SetDefaults(anon_fore_4 *fontColor, float *trans, signed short *fontXpos, signed short *fontYpos,
                         signed char *fontIndex, signed char *leading, signed char *spacing);
unsigned int _TextLen(const unsigned char *text, signed short numChars);
static void _InitSemaphore(void);
static void _GetSemaphore(void);
static void _ReleaseSemaphore(void);
#pragma inline(_InitSemaphore,_GetSemaphore,_ReleaseSemaphore)
/* 0x8001a31c, size 0x28 */
void HAL_InitText(void)
{
    _InitSemaphore();
    gFontIndex = -1;
    gFontHeap = mem_func_8C03FD8A(0,200000,0,16);
}

/* 0x8001a344, size 0x68 */
void HAL_StartText(void)
{
    _GetSemaphore();
    _SetDefaults(&gFontColor, &gTrans, &gFontXpos, &gFontYpos, &gFontIndex, &gLeading, &gSpacing);
    _HAL_SetFont(gFontIndex);
    gFontAttribute = 0;
    gFontAttribute |= 0x8000;
    gBufferInited = 0;
    gFinished = 0;
    _ReleaseSemaphore();
}

/* 0x8001a3ac, size 0x1dc */
void HAL_FinishText(signed char end)
{
    int i; /* r30 */
    int length; /* r29 */
    unsigned int aLong; /* stack */
    if (gBufferInited) {
        _GetSemaphore();
        gFinished = end;
        _SetDefaults(&gFontColor, &gTrans, &gFontXpos, &gFontYpos, &gFontIndex, &gLeading, &gSpacing);
        _HAL_SetFont(gFontIndex);
        gFontAttribute &= 0x8000;
        for (i = 0; i < gBufLen;) {
            switch (gBuffer[i++]) {
                case 0:
                    _HAL_SetFont(gBuffer[i++]);
                    break;
                case 1:
                    HAL_SetFontLeading(gBuffer[i++]);
                    break;
                case 2:
                    HAL_SetFontSpacing(gBuffer[i++]);
                    break;
                case 3:
                    HAL_SetTextPos(((short)gBuffer[i] << 8) | gBuffer[i + 1],
                                   ((short)gBuffer[i + 2] << 8) | gBuffer[i + 3]);
                    i += 4;
                    break;
                case 4:
                    aLong =
                        (gBuffer[i] << 24) | (gBuffer[i + 1] << 16) | (gBuffer[i + 2] << 8) | gBuffer[i + 3];
                    i += 4;
                    HAL_SetTextTranslucency(*(float *)&aLong);
                    break;
                case 5:
                    HAL_SetTextColor(gBuffer[i], gBuffer[i + 1], gBuffer[i + 2], gBuffer[i + 3]);
                    i += 4;
                    break;
                case 6:
                    length = gBuffer[i++];
                    _DrawText(gBuffer + i, length);
                    i += length;
                    break;
            }
        }
        _ReleaseSemaphore();
    }
}

/* 0x8001a588, size 0xec */
int _HAL_SetFont(int fontID)
{
    int old; /* r31 */
    old = gFontIndex;
    gFontIndex = fontID;
    if (fontID != -1) {
        gFontDatWaveInst = FindWaveInstance(gFontIndex + 24);
        if (gFontDatWaveInst < 0)
            LockWaveInstance(gFontDatWaveInst = LoadWave(gFontIndex + 24, 0, 0, 1, 0));
        gFontTexWaveInst = FindWaveInstance(gFontIndex + 8);
        if (gFontTexWaveInst < 0)
            LockWaveInstance(gFontTexWaveInst = LoadWave(gFontIndex + 8, 0, 0, 0, gFontHeap));
        _SetupFontData(old != fontID);
    }
    if (fontID == 0)
        HAL_SetFontFixedWidth(1);
    return old;
}

/* 0x8001a674, size 0x40 */
int HAL_SetFont(int fontID)
{
    int old; /* r31 */
    _GetSemaphore();
    old = _HAL_SetFont(fontID);
    _ReleaseSemaphore();
    return old;
}

/* 0x8001a6b4, size 0x5c */
void HAL_PurgeFonts(void)
{
    int wave; /* r0 */
    int i; /* r31 */
    for (i = 0; i < 16; i++) {
        wave = FindWaveInstance(i + 24);
        if (wave >= 0)
            UnlockWaveInstance(wave);
        wave = FindWaveInstance(i + 8);
        if (wave >= 0)
            UnlockWaveInstance(wave);
    }
}

/* 0x8001a710, size 0x238 */
void _SetupFontData(int force)
{
    int i; /* r6 */
    unsigned char *data; /* r30 */
    void *handle; /* r0 */
    handle = GetWaveInstanceAddr(gFontDatWaveInst);
    data = *(unsigned char **)handle;
    if (force || gFontHeader != (void *)data) {
        gFontHeader = (void *)data;
        data += sizeof(*gFontHeader);
        gFontLetters = (void *)data;
        for (i = 0; i < gFontHeader->numChars; i++) {
        }
        data += gFontHeader->numChars * sizeof(*gFontLetters);
        if (gFontHeader->kern)
            for (i = 0; i < gFontHeader->numChars; i++) {
                gFontLetters[i].kerns = data;
                data += gFontHeader->numChars;
            }
        for (i = 0; i < 256; i++)
            gCharMap[i] = -1;
        for (i = 0; i < gFontHeader->numChars; i++)
            gCharMap[gFontLetters[i].value] = i;
    }
    if (force) {
        for (i = 0; i < gFontHeader->numTextures; i++)
            gFontTextures[i] = gTexDefPtrs[gFontTexWaveInst].texDefs + i;
        gLeading = gFontHeader->leading;
        gSpacing = gFontHeader->spacing;
    }
}

/* 0x8001a948, size 0x78 */
void HAL_SetTextColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    if (gFontColor.fore.rgba[0] != red || gFontColor.fore.rgba[1] != green ||
        gFontColor.fore.rgba[2] != blue || gFontColor.fore.rgba[3] != alpha) {
        gFontColor.fore.rgba[0] = red;
        gFontColor.fore.rgba[1] = green;
        gFontColor.fore.rgba[2] = blue;
        gFontColor.fore.rgba[3] = alpha;
        BlitClearColors();
    }
}

/* 0x8001a9c0, size 0x58 */
void HAL_SetTextDepth(float depth)
{
    gDepth = depth;
    if (depth > 0.0f) {
        gFontAttribute |= 16;
        BlitZDist(depth);
    } else
        gFontAttribute &= ~16;
}

/* 0x8001aa18, size 0x58 */
void HAL_SetMenuTextDepth(float depth)
{
    gDepth = depth;
    if (depth > 0.0f) {
        gFontAttribute |= 16;
        BlitZDist(depth);
    } else
        gFontAttribute &= ~16;
}

/* 0x8001aa70, size 0x68 */
void HAL_SetTextTranslucency(float xlu)
{
    gTrans = xlu;
    if (xlu < 0.0f || xlu >= 255.0f)
        gFontAttribute &= ~32;
    else {
        gFontAttribute |= 32;
        BlitXLU(xlu);
    }
}

/* 0x8001aad8, size 0x60 */
void HAL_SetTextPos(signed short x, signed short y)
{
    if (x != -32768)
        gFontXpos = x * 2;
    if (y != -32768)
        gFontYpos = (int)(2.0f * y + 4.0f);
}

/* 0x8001ab38, size 0x74 */
void HAL_SetFontLeading(signed short leading)
{
    if (leading < 0) {
    _SetupFontData(0);
        gLeading = gFontHeader->leading;
    } else
        gLeading = (int)(2.0f * leading);
}

/* 0x8001abac, size 0x48 */
void HAL_SetFontSpacing(signed short spacing)
{
    if (spacing < 0) {
    _SetupFontData(0);
        gSpacing = gFontHeader->spacing;
    } else
        gSpacing = spacing * 2;
}

/* 0x8001abf4, size 0x40 */
signed char HAL_SetFontFixedWidth(signed char fixedWidth)
{
    signed char retFixedWidth; /* r3 */
    _SetupFontData(0);
    retFixedWidth = gFontHeader->nonProp;
    gFontHeader->nonProp = fixedWidth;
    return retFixedWidth;
}

/* 0x8001ac34, size 0x58 */
unsigned char HAL_GetFontAscent(void)
{
    _SetupFontData(0);
    return gFontHeader->ascent / 2.0f;
}

/* 0x8001ac8c, size 0x58 */
unsigned char HAL_GetFontDescent(void)
{
    _SetupFontData(0);
    return gFontHeader->descent / 2.0f;
}

/* 0x8001ace4, size 0x44 */
signed char HAL_GetFontLeading(void)
{
    return gLeading / 2.0f;
}

/* 0x8001ad28, size 0x1c */
signed char HAL_GetFontSpacing(void)
{
    return gSpacing / 2;
}

/* Extra DC helperF41A..F46A; caller return type unproven. */
void HAL_GetFontChars(unsigned char *out)
{
    int i;
    _SetupFontData(0);
    for (i=0;i<gFontHeader->numChars;i++) out[i]=gFontLetters[i].value;
    out[i]=0;
}

/* 0x8001ad44, size 0xc8 */
unsigned int HAL_GetCharWidth(unsigned short ch)
{
    signed short c; /* r3 */
    _SetupFontData(0);
    if (gFontHeader->nonProp)
        return gFontHeader->maxWidth / 2;
    if (ch == 32 || ch > 255)
        return gFontHeader->spaceWidth / 2;
    if (ch == 10)
        return 0;
    c = gCharMap[ch];
    if (c < 0)
        return gFontHeader->spaceWidth / 2;
    return (gFontLetters[c].lrs - gFontLetters[c].uls + 1) / 2;
}

/* 0x8001ae0c, size 0x1bc */
unsigned int HAL_GetTextWidth(void *text, signed short numChars)
{
    int i; /* r6 */
    int len; /* r7 */
    int step; /* r8 */
    int ch; /* r31 */
    int chPrev; /* r9 */
    int nc; /* r10 */
    int dbl; /* r11 */
    const unsigned char *t; /* r12 */
    _SetupFontData(0);
    if (*(unsigned char *)text == 255) {
        t = (unsigned char *)text + 1;
        dbl = 1;
        step = 2;
    } else {
        t = text;
        dbl = 0;
        step = 1;
    }
    if (!t[0] && (!dbl || !t[1]))
        return 0;
    i = 0;
    len = 0;
    nc = 0;
    chPrev = -1;
    while ((numChars < 0 || nc < numChars) && (t[i] || (dbl && t[i + 1]))) {
        if (gFontHeader->nonProp)
            len = gFontHeader->maxWidth + gSpacing + len;
        else {
            if (dbl)
                ch = (t[i] << 8) | t[i + 1];
            else
                ch = t[i];
            if (ch == 32 || ch > 255) {
                len += gFontHeader->spaceWidth;
                ch = -1;
            } else if (ch == 10)
                break;
            else {
                ch = gCharMap[ch];
                if (ch < 0)
                    len += gFontHeader->spaceWidth;
                else {
                    len += gFontLetters[ch].lrs - gFontLetters[ch].uls + 1 + gSpacing;
                    if (chPrev > 0 && gFontHeader->kern)
                        len -= gFontLetters[ch].kerns[chPrev];
                }
            }
        }
        chPrev = ch;
        i += step;
        nc++;
    }
    return (len - gSpacing) / 2;
}

/* 0x8001afc8, size 0x448 */
void HAL_DrawText(void *text, signed short numChars)
{
    int size; /* r31 */
    int length; /* r30 */
    char str[256]; /* stack */
    if (gDepth >= 0.0f) {
        _DrawText(text, _TextLen(text, numChars));
        return;
    }
    if (gFinished) {
        if (numChars < 0)
            strcpy(str, text);
        else
            strncpy(str, text, numChars);
    }
    _GetSemaphore();
    if (!gBufferInited) {
        gBufferInited = 1;
        gBufLen = 0;
        _SetDefaults(&lastFontColor, &lastTrans, &lastFontXpos, &lastFontYpos, &lastFontIndex, &lastLeading,
                     &lastSpacing);
    }
    size = 0;
    if (lastFontIndex != gFontIndex)
        size = 2;
    if (lastLeading != gLeading)
        size += 2;
    if (lastSpacing != gSpacing)
        size += 2;
    if (lastFontXpos != gFontXpos || lastFontYpos != gFontYpos)
        size += 5;
    if (lastTrans != gTrans)
        size += 5;
    if (lastFontColor.fore.rgbaU32 != gFontColor.fore.rgbaU32)
        size += 5;
    length = _TextLen(text, numChars);
    size += length + 2;
    if (gBufLen + size >= 800) {
        _ReleaseSemaphore();
        return;
    }
    if (lastFontIndex != gFontIndex) {
        gBuffer[gBufLen++] = 0;
        gBuffer[gBufLen++] = lastFontIndex = gFontIndex;
    }
    if (lastLeading != gLeading) {
        gBuffer[gBufLen++] = 1;
        gBuffer[gBufLen++] = lastLeading = gLeading;
    }
    if (lastSpacing != gSpacing) {
        gBuffer[gBufLen++] = 2;
        lastSpacing = gSpacing;
        gBuffer[gBufLen++] = gSpacing / 2;
    }
    if (lastFontXpos != gFontXpos || lastFontYpos != gFontYpos) {
        lastFontXpos = gFontXpos / 2;
        lastFontYpos = gFontYpos / 2.0f;
        gBuffer[gBufLen++] = 3;
        gBuffer[gBufLen++] = (lastFontXpos >> 8) & 255;
        gBuffer[gBufLen++] = lastFontXpos;
        gBuffer[gBufLen++] = (lastFontYpos >> 8) & 255;
        gBuffer[gBufLen++] = lastFontYpos;
        lastFontXpos = gFontXpos;
        lastFontYpos = gFontYpos;
    }
    if (lastTrans != gTrans) {
        lastTrans = gTrans;
        gBuffer[gBufLen++] = 4;
        gBuffer[gBufLen++] = (*(unsigned int *)&gTrans) >> 24;
        gBuffer[gBufLen++] = (*(unsigned int *)&gTrans) >> 16;
        gBuffer[gBufLen++] = (*(unsigned int *)&gTrans) >> 8;
        gBuffer[gBufLen++] = *(unsigned int *)&gTrans;
    }
    if (lastFontColor.fore.rgbaU32 != gFontColor.fore.rgbaU32) {
        lastFontColor = gFontColor;
        gBuffer[gBufLen++] = 5;
        memcpy(gBuffer + gBufLen, &gFontColor, 4);
        gBufLen += 4;
    }
    gBuffer[gBufLen++] = 6;
    gBuffer[gBufLen++] = length;
    memcpy(gBuffer + gBufLen, text, length);
    gBufLen += length;
    _ReleaseSemaphore();
}

/* 0x8001b410, size 0x2b4 */
void _DrawText(const unsigned char *text, unsigned short length)
{
    int i; /* r29 */
    int x; /* r4 */
    int x0; /* r28 */
    int y0; /* r27 */
    int step; /* r26 */
    int ch; /* r0 */
    int chPrev; /* r5 */
    int lastPage; /* r25 */
    const unsigned char *t; /* r24 */
    int dbl; /* r23 */
    anon_kerns_value_c *l; /* r22 */
    _SetupFontData(0);
    BlitScissor(0, 0, 639, 447);
    BlitSetAttribute(gFontAttribute);
    BlitClearAttribute(~gFontAttribute);
    BlitClearColors();
    if (gFontAttribute & 32)
        gFontColor.fore.rgba[3] = (int)gTrans;
    ((void (*)(void *, int, int, int))BlitColors)(&gFontColor, 0, 0, 0);
    lastPage = 0;
    if (text[0] == 255) {
        t = text + 1;
        dbl = 1;
        step = 2;
        length -= 2;
    } else {
        t = text;
        dbl = 0;
        step = 1;
        length -= 1;
    }
    x0 = gFontXpos;
    y0 = gFontYpos;
    i = 0;
    chPrev = -1;
    for (; i < length;) {
        if (dbl)
            ch = (t[i] << 8) | t[i + 1];
        else
            ch = t[i];
        if (ch == 10) {
            ch = -1;
            y0 += gFontHeader->ascent + gFontHeader->descent + gLeading;
            x0 = gFontXpos;
        } else {
            if (ch == 32 || ch > 255) {
            space:
                x0 += gFontHeader->nonProp ? gFontHeader->maxWidth + gSpacing : gFontHeader->spaceWidth;
                ch = -1;
            } else {
                ch = gCharMap[ch];
                if (ch < 0)
                    goto space;
                if (chPrev > 0 && gFontHeader->kern && !gFontHeader->nonProp)
                    x0 -= gFontLetters[ch].kerns[chPrev];
                l = gFontLetters + ch;
                if (l->texIdx != lastPage)
                    lastPage = l->texIdx;
                x = gFontHeader->nonProp ? (gFontHeader->maxWidth - (l->lrs - l->uls + 1)) / 2 : 0;
                x = x0 + x;
                BlitDraw(gFontTextures[lastPage], x, y0, x + l->lrs - l->uls, y0 + l->lrt - l->ult + 1,
                         l->uls + 1, l->ult + 1);
                x0 += (gFontHeader->nonProp ? gFontHeader->maxWidth : l->lrs - l->uls + 1) + gSpacing;
            }
        }
        i += step;
        chPrev = ch;
    }
}

/* 0x8001b6c4, size 0x40 */
void _SetDefaults(anon_fore_4 *fontColor, float *trans, signed short *fontXpos, signed short *fontYpos,
                         signed char *fontIndex, signed char *leading, signed char *spacing)
{
    fontColor->fore.rgba[0] = fontColor->fore.rgba[1] = fontColor->fore.rgba[2] = 230;
    fontColor->fore.rgba[3] = 255;
    *trans = -1.0f;
    *fontXpos = 0;
    *fontYpos = 0;
    *fontIndex = -1;
    *leading = -1;
    *spacing = -1;
}

/* 0x8001b704, size 0x88 */
unsigned int _TextLen(const unsigned char *text, signed short numChars)
{
    unsigned int len; /* r7 */
    int nc; /* r8 */
    nc = 0;
    if (text[0] == 255) {
        len = 1;
        do {
            len += 2;
            nc++;
        } while ((text[len - 1] || text[len - 2]) && (numChars < 0 || nc <= numChars));
    } else {
        len = 0;
        do {
            len++;
            nc++;
        } while (*text++ && (numChars < 0 || nc <= numChars));
    }
    return len;
}

/* 0x8001b78c, size 0x18 */
static void _InitSemaphore(void)
{
    static signed char sInited = 0;
    if (!sInited)
        sInited = 1;
}

/* 0x8001b7a4, size 0x4 */
static void _GetSemaphore(void) { gOldMask = SetInterruptMask(15); }

/* 0x8001b7a8, size 0x4 */
static void _ReleaseSemaphore(void) { SetInterruptMask(gOldMask); }
