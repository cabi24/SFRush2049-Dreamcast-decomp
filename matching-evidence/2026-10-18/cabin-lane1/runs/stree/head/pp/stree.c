/* stree.c - private Dreamcast spatial tree candidate; MAT3 semantic source read-only. */
#line 1 "stree_types.h"
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
#line 2 "stree_types.h"
#ifndef STREE_TYPES_H
#define STREE_TYPES_H
typedef tCarData tStreeGameCar;
/* Spatial layouts: InitSTree145C..14E2 proves header16/path132/tree20/surf24/vertex8/save32.
   streetyp fields: downleaf162E..168C; surftyp fields: PointInSurf18D2 onward.
   sverttyp packed position/overflow: PointInSurf and MoveCollisionObject378E onward.
   savetyp fields: ResetCollisionObject3C02..3C30, stride32 at3C3C.
   pathtyp scalar fields: fPathInterp22D4..2652 and fHalfPipeInterp2652..286C.
   collision record offsets+0/+4/+28/+2A/+2C: CarApply3CC2..3CD2 and AgainstSurface tail. */
typedef struct streetyp {
    signed short parent; /* 0x0 */
    unsigned char unused; /* 0x2 */
    unsigned char flags; /* 0x3 */
    signed short minh; /* 0x4 */
    signed short maxh; /* 0x6 */
    signed short minv; /* 0x8 */
    signed short maxv; /* 0xa */
    unsigned short child[4]; /* 0xc */
} streetyp;



typedef struct stheadertyp {
    unsigned short pnum; /* 0x0 */
    unsigned short tnum; /* 0x2 */
    unsigned short snum; /* 0x4 */
    unsigned short vnum; /* 0x6 */
    unsigned short savenum; /* 0x8 */
    unsigned short vlnum; /* 0xa */
    unsigned int slnum; /* 0xc */
} stheadertyp;

typedef struct pathtyp {
    float pos[3]; /* 0x0 */
    float uvs[3][3]; /* 0xc */
    float ve[3]; /* 0x30 */
    float vl[3]; /* 0x3c */
    float vr[3]; /* 0x48 */
    float width; /* 0x54 */
    float w; /* 0x58 */
    float m0; /* 0x5c */
    float m1; /* 0x60 */
    float wl; /* 0x64 */
    float ml; /* 0x68 */
    float wr; /* 0x6c */
    float mr; /* 0x70 */
    float m0l; /* 0x74 */
    float m0r; /* 0x78 */
    float m1l; /* 0x7c */
    float m1r; /* 0x80 */
} pathtyp;

typedef struct sverttyp {
    signed short pos[3]; /* 0x0 */
    unsigned short overflow; /* 0x6 */
} sverttyp;

typedef struct savetyp {
    unsigned short object; /* 0x0 */
    unsigned short snum; /* 0x2 */
    signed short uvs[3][3]; /* 0x4 */
    unsigned short vnum; /* 0x16 */
    signed short pos[3]; /* 0x18 */
    unsigned short overflow; /* 0x1e */
} savetyp;

typedef struct anon_surf_uvs_30 {
    surftyp *surf; /* 0x0 */
    float uvs[3][3]; /* 0x4 */
    signed short carEdge; /* 0x28 */
    signed short index; /* 0x2a */
    float dist; /* 0x2c */
} anon_surf_uvs_30;
/* Private model overlay. All omitted regions remain explicit gaps.
   Fields inherited in canonical layouts keep their proven offsets. */




typedef struct tStreeLink { char pad0[8]; unsigned char owner; char pad1; } tStreeLink; /* target54674..680 stride10,owner8 */
extern tStreeGameCar gGameCar[];
extern tStreeLink gLink[];
extern float unitmatrix[4][3]; /* target51888=base+36,52D38=base */
extern float _builtin_sqrtf(float);
extern float _builtin_fabsf(float);
#define sqrtf _builtin_sqrtf
#define fabs _builtin_fabsf
void BodyVector(float *,float *,float (*)[3]);
void WorldVector(float *,float *,float (*)[3]);
void CopyUV(float (*)[3],float (*)[3]);
void MulUVS(float (*)[3],float (*)[3],float (*)[3]);
float NormalVector(float *);
void froll(float,float,float (*)[3]);
void fpitch(float,float,float (*)[3]);
void AbortCar(signed short,int);
void AddBattlePoint(signed short,signed short,int);
int PlayGameSound(int,int,int,unsigned char);
void *memcpy(void *,const void *,unsigned int);
typedef char checkStreet[(sizeof(streetyp)==20)?1:-1];
typedef char checkSurf[(sizeof(surftyp)==24)?1:-1];
typedef char checkRoadHeader[(sizeof(stheadertyp)==16)?1:-1];
typedef char checkPath[(sizeof(pathtyp)==132)?1:-1];
typedef char checkVert[(sizeof(sverttyp)==8)?1:-1];
typedef char checkSave[(sizeof(savetyp)==32)?1:-1];
typedef char checkCollision[(sizeof(anon_surf_uvs_30)==48)?1:-1];
typedef char checkGameCar[(sizeof(tStreeGameCar)==0x3CC)?1:-1];
typedef char checkLink[(sizeof(tStreeLink)==10)?1:-1];
#endif
#line 3 "stree.c"
void *gRoadDat;
streetyp *stree;
surftyp *surf;
sverttyp *svert;
pathtyp *path;
unsigned char *slist;
unsigned char *vlist;
savetyp *save;
unsigned short savenum;
stheadertyp *gRoadHead;
void InitSTree(void);
void mats2fcopy(signed short (*sp)[3], float (*fp)[3]);
void matf2scopy(float (*fp)[3], signed short (*sp)[3]);
void FVecInterp(float *v1, float *v2, float t, float *vr);
streetyp *downleaf(streetyp *tp, signed short posh, signed short posv, signed short *quad);
streetyp *updownleaf(streetyp *tp, signed short posh, signed short posv, signed short *quad);
void DecodeSlist(unsigned char *slp, int snum, unsigned short *slnum, unsigned int sli);
unsigned short DecodeVlist(unsigned char *vlp, int vnum, unsigned short *vlnum);
void surfpos(float *pos, float (*uvs)[3], float *pt, float *rpt);
int VectorToSurfEdge(float *tpos, float *cpos, float *epos, float radiussq);
signed short PointInSurf(surftyp *sp, float *ipos, float *ipos2, float *spos, float *dist, unsigned short *path, float (*uvs)[3], float maxSurfHop);
signed short CylinderInSurf(surftyp *sp, float *ipos, float *ipos2, float *opos, float *epos, float (*uvs)[3], float radiussq, int laststate, unsigned short *path);
void fPathInterp(surftyp *sp, unsigned short pathIndex, float *ipos, float *opos, float (*uvs)[3]);
void fHalfPipeInterp(surftyp *sp, unsigned short pathIndex, float *ipos, float *opos, float (*uvs)[3], float dist);
static float floorpos(float f);
void TireSurf(tModelData *m, float *ipos, float *opos, int *roadcode, float (*uvs)[3], int whl);
surftyp *RoadPosition(float *ipos, float *opos, float (*uvs)[3]);
surftyp *CameraCollision(float *ipos, float *lookat);
static signed short shalf(signed short v);
surftyp *ShotToWorldCollision(float *old, float *new, float (*uvs)[3], float radius, int interp, int ignore);
void MoveCollisionObject(int num, float *trans);
void RotateCollisionObject(int num, float *ipos, float (*orig)[3], float (*uvs)[3]);
void ResetCollisionObject(int num);
void RemoveCollisionObject(int num);
void CarApplyCollision(tModelData *m, anon_surf_uvs_30 *c);
void CollideCarBodyAgainstSurface(tModelData *m, surftyp *sp, anon_surf_uvs_30 *buf);
void CollideCarBody(tModelData *m);
#pragma inline(floorpos,shalf)
/* 0x80043904 */
void InitSTree(void)
{
 stheadertyp *hp;
 gRoadHead = hp = (stheadertyp *)gRoadDat;
 path = (pathtyp *)(hp + 1);
 stree = (streetyp *)(path + hp->pnum);
 surf = (surftyp *)(stree + hp->tnum);
 svert = (sverttyp *)(surf + hp->snum);
 save = (savetyp *)(svert + hp->vnum);
 savenum = hp->savenum;
 vlist = (unsigned char *)(save + hp->savenum);
 slist = vlist + hp->vlnum;
}

/* Retain recovered local helpers while their callers remain unmatched. */

/* 0x80043BD4 */
void mats2fcopy(signed short (*sp)[3], float (*fp)[3])
{
    fp[0][0] = sp[0][0] * (1.0f / 16384.0f);
    fp[0][1] = sp[0][1] * (1.0f / 16384.0f);
    fp[0][2] = sp[0][2] * (1.0f / 16384.0f);
    fp[1][0] = sp[1][0] * (1.0f / 16384.0f);
    fp[1][1] = sp[1][1] * (1.0f / 16384.0f);
    fp[1][2] = sp[1][2] * (1.0f / 16384.0f);
    fp[2][0] = sp[2][0] * (1.0f / 16384.0f);
    fp[2][1] = sp[2][1] * (1.0f / 16384.0f);
    fp[2][2] = sp[2][2] * (1.0f / 16384.0f);
}

/* 0x80043D0C */
void matf2scopy(float (*fp)[3], signed short (*sp)[3])
{
    sp[0][0] = fp[0][0] * 16384.0f;
    sp[0][1] = fp[0][1] * 16384.0f;
    sp[0][2] = fp[0][2] * 16384.0f;
    sp[1][0] = fp[1][0] * 16384.0f;
    sp[1][1] = fp[1][1] * 16384.0f;
    sp[1][2] = fp[1][2] * 16384.0f;
    sp[2][0] = fp[2][0] * 16384.0f;
    sp[2][1] = fp[2][1] * 16384.0f;
    sp[2][2] = fp[2][2] * 16384.0f;
}


/* 0x80043DF4 */
void FVecInterp(float *v1, float *v2, float t, float *vr)
{
    float tp;
    signed short i;
    tp = 1.0f - t;
    for (i=0;i<3;i++) *vr++ = *v1++ * tp + *v2++ * t;
}

/* 0x80043E3C */
streetyp *downleaf(streetyp *tp, signed short posh, signed short posv, signed short *quad)
{
    int midh, midv;
    signed short q;
again:
    midh = tp->minh + tp->maxh;
    if (midh < 0) midh++;
    midv = tp->minv + tp->maxv;
    if (midv < 0) midv++;
    midh >>= 1;
    midv >>= 1;
    q = posh >= midh;
    if (posv < midv) q += 2;
    if (tp->flags & (1 << q)) {
        tp = &stree[tp->child[q]];
        if (tp == stree) return 0;
        goto again;
    } else {
        *quad = q;
        return tp;
    }
}

/* 0x80043EE0 */
streetyp *updownleaf(streetyp *tp, signed short posh, signed short posv, signed short *quad)
{
    signed short outside;
    if (!tp) tp = stree;
again:
    if ((outside = (posh >= tp->maxh || posh < tp->minh || posv >= tp->maxv || posv < tp->minv)) != 0) {
        if (tp->parent == -1) return 0;
        tp = &stree[tp->parent];
        goto again;
    } else return downleaf(tp, posh, posv, quad);
}

/* 0x80043F78 */
void DecodeSlist(unsigned char *slp, int snum, unsigned short *slnum, unsigned int sli)
{
    unsigned short *slnum2 = slnum + snum - 1;
    int cnt;
    unsigned int next;
    while (snum > 0) {
        next = (slp[0] << 8) + slp[1];
        slp += 2;
        cnt = next >> 13;
        snum -= cnt + 1;
        next &= 0x1fff;
        do {
            if (next == sli) *slnum++ = next++;
            else *slnum2-- = next++;
        } while (--cnt >= 0);
   }
}

/* 0x80043FE4 */
unsigned short DecodeVlist(unsigned char *vlp, int vnum, unsigned short *vlnum)
{
    int cnt;
    unsigned int next;
    while (vnum > 0) {
        next = (vlp[0] << 8) + vlp[1];
        vlp += 2;
        if (vnum > 1 && vlp[0] >= 0xe0) {
            cnt = *vlp++ & 0x1f;
        } else cnt = 0;
        vnum -= cnt + 1;
        do {
            *vlnum++ = next++;
        } while (--cnt >= 0);
   }
    return (vlp[0] << 8) + vlp[1];
}

/* Retain recovered geometric helpers while their callers remain unmatched. */

/* 0x8004405C */
void surfpos(float *pos, float (*uvs)[3], float *pt, float *rpt)
{
    float temp[3];
    temp[0] = pt[0] - pos[0];
    temp[1] = pt[1] - pos[1];
    temp[2] = pt[2] - pos[2];
    BodyVector(temp, rpt, uvs);
}

/* 0x800440BC */
int VectorToSurfEdge(float *tpos, float *cpos, float *epos, float radiussq)
{
    float t;
    t = (cpos[0] * tpos[0] + cpos[2] * tpos[2]) / (tpos[0] * tpos[0] + tpos[2] * tpos[2]);
    if (t < 0.0f) {
        epos[0] = unitmatrix[3][0] - cpos[0];
        epos[1] = 0.0f;
        epos[2] = unitmatrix[3][2] - cpos[2];
    } else if (t > 1.0f) {
        epos[0] = tpos[0] - cpos[0];
        epos[1] = 0.0f;
        epos[2] = tpos[2] - cpos[2];
    } else {
        epos[0] = tpos[0] * t - cpos[0];
        epos[1] = 0.0f;
        epos[2] = tpos[2] * t - cpos[2];
    }
    epos[1] = 0.0f;
    return !(epos[0] * epos[0] + epos[2] * epos[2] >= radiussq);
}


/* 0x80044198 */
signed short PointInSurf(surftyp *sp, float *ipos, float *ipos2, float *spos, float *dist, unsigned short *path, float (*uvs)[3], float maxSurfHop)
{
    int i;
    float tpos[3], vpos[3], epos[3], cpos[3];
    unsigned int vnum;
    unsigned short vlnum[16];
    sverttyp *vp;
    float ny;
    int state = 1;
    vnum = sp->snd_flags & 15;
    *path = DecodeVlist(vlist + sp->vli, vnum, vlnum);
    mats2fcopy(sp->uvs, uvs);
    vp = &svert[vlnum[0]];
    vpos[0] = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
    vpos[1] = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
    vpos[2] = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
    tpos[0] = ipos[0] - vpos[0];
    tpos[1] = ipos[1] - vpos[1];
    tpos[2] = ipos[2] - vpos[2];
    BodyVector(tpos, spos, uvs);
    if (spos[1] <= maxSurfHop || spos[1] > *dist) return 0;
    vp = &svert[vlnum[vnum - 1]];
    vpos[0] = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
    vpos[1] = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
    vpos[2] = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
    ny = vpos[0] * (vpos[2] - spos[2]) - vpos[2] * (vpos[0] - spos[0]);
    if (ny < 0.0f) {
        if (!VectorToSurfEdge(vpos, spos, epos, 0.01f)) return 0;
        state = -1;
        goto done;
    }
    vp = &svert[vlnum[1]];
    vpos[0] = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
    vpos[1] = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
    vpos[2] = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
    ny = spos[2] * vpos[0] - spos[0] * vpos[2];
    if (ny < 0.0f) {
        if (!VectorToSurfEdge(vpos, spos, epos, 0.01f)) return 0;
        state = -1;
        goto done;
    }
    for (i = 2; i < vnum; i++) {
        float oldx = vpos[0], oldz = vpos[2];
        vp = &svert[vlnum[i]];
        vpos[0] = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
    vpos[1] = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
    vpos[2] = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
        tpos[0] = vpos[0] - oldx;
        tpos[1] = 0.0f;
        tpos[2] = vpos[2] - oldz;
        cpos[0] = spos[0] - oldx;
        cpos[1] = 0.0f;
        cpos[2] = spos[2] - oldz;
        ny = cpos[2] * tpos[0] - cpos[0] * tpos[2];
        if (ny < 0.0f) {
            if (!VectorToSurfEdge(tpos, cpos, epos, 0.01f)) return 0;
            state = -1;
            break;
        }
    }
done:
    *dist = spos[1];
    if (ipos2 && ((sp->flags & 15) == 5 || (sp->flags & 15) == 6)) {
        vp = &svert[vlnum[0]];
        vpos[0] = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
    vpos[1] = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
    vpos[2] = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
        tpos[0] = ipos2[0] - vpos[0];
        tpos[1] = ipos2[1] - vpos[1];
        tpos[2] = ipos2[2] - vpos[2];
        BodyVector(tpos, vpos, uvs);
        if (vpos[1] <= *dist || vpos[1] < -0.050000011920928955f) return 0;
    }
    return state;
}


/* 0x80044738 */
signed short CylinderInSurf(surftyp *sp, float *ipos, float *ipos2, float *opos, float *epos, float (*uvs)[3], float radiussq, int laststate, unsigned short *path)
{
    unsigned int vnum;
    unsigned short vlnum[16];
    sverttyp *vp;
    float spos[3], spos2[3], tpos[3], vpos[3];
    float t, ny;
    int i;
    int state = 1;
    float cpos[3];
    float x, y, z;
    mats2fcopy(sp->uvs, uvs);
    vnum = sp->snd_flags & 15;
    *path = DecodeVlist(vlist + sp->vli, vnum, vlnum);
    vp = &svert[vlnum[0]];
    x = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
    y = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
    z = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
    tpos[0] = ipos[0] - x;
    tpos[1] = ipos[1] - y;
    tpos[2] = ipos[2] - z;
    BodyVector(tpos, spos, uvs);
    if (spos[1] < 0.0f) return 0;
    tpos[0] = ipos2[0] - x;
    tpos[1] = ipos2[1] - y;
    tpos[2] = ipos2[2] - z;
    BodyVector(tpos, spos2, uvs);
    if (spos2[1] > 0.0f) return 0;
    tpos[0] = spos2[0] - spos[0];
    tpos[1] = spos2[1] - spos[1];
    tpos[2] = spos2[2] - spos[2];
    if (tpos[1] > 0.0f) return 0;
    if (tpos[1] < 0.0f) {
        t = spos[1] / tpos[1];
        spos[0] -= tpos[0] * t;
        spos[1] -= tpos[1] * t;
        spos[2] -= tpos[2] * t;
    }
    vp = &svert[vlnum[vnum - 1]];
    vpos[0] = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
        vpos[1] = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
        vpos[2] = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
    ny = vpos[0] * (vpos[2] - spos[2]) - vpos[2] * (vpos[0] - spos[0]);
    if (ny < 0.0f) {
        if (laststate > 0) return 0;
        if (!VectorToSurfEdge(vpos, spos, epos, radiussq)) return 0;
        state = -1;
        goto done;
    }
    vp = &svert[vlnum[1]];
    vpos[0] = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
        vpos[1] = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
        vpos[2] = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
    ny = spos[2] * vpos[0] - spos[0] * vpos[2];
    if (ny < 0.0f) {
        if (laststate > 0) return 0;
        if (!VectorToSurfEdge(vpos, spos, epos, radiussq)) return 0;
        state = -1;
        goto done;
    }
    for (i = 2; i < vnum; i++) {
        float oldx = vpos[0], oldz = vpos[2];
        vp = &svert[vlnum[i]];
        vpos[0] = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
        vpos[1] = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
        vpos[2] = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
        tpos[0] = vpos[0] - oldx;
        tpos[1] = 0.0f;
        tpos[2] = vpos[2] - oldz;
        cpos[0] = spos[0] - oldx;
        cpos[1] = 0.0f;
        cpos[2] = spos[2] - oldz;
        ny = cpos[2] * tpos[0] - cpos[0] * tpos[2];
        if (ny < 0.0f) {
            if (laststate > 0) return 0;
            if (!VectorToSurfEdge(tpos, cpos, epos, radiussq)) return 0;
            state = -1;
            break;
        }
    }
done:
    WorldVector(spos, opos, uvs);
    opos[0] += x;
    opos[1] += y;
    opos[2] += z;
    return state;
}


/* The original call sites reserve r3 for the surface argument. The recovered
 * DWARF declaration omits this unused argument. */
/* 0x80044CD0 */
void fPathInterp(surftyp *sp, unsigned short pathIndex, float *ipos, float *opos, float (*uvs)[3])
{
    pathtyp *p0p;
    pathtyp *p1p;
    float xi, yi;
    float vz0[3], vz1[3], vx[3], vy[3], vz[3];
    float pos0[3], pos1[3], pos[3];
    float m0, m1, length, i2, offset;
    float fgpos[3];
    int i;
    p1p = &path[pathIndex + 1 < gRoadHead->pnum ? pathIndex + 1 : 0];
    p0p = &path[pathIndex];
    surfpos(p0p->pos, p0p->uvs, ipos, fgpos);
    length = fgpos[0] * (p0p->m1 - p0p->m0) + p0p->w;
    xi = (fgpos[2] - fgpos[0] * p0p->m0) / length;
    if (fgpos[0] > 0.0f)
        yi = 0.5f * (fgpos[0] / (fgpos[2] * p0p->mr + p0p->wr)) + 0.5f;
    else
        yi = -0.5f * (fgpos[0] / (fgpos[2] * p0p->ml + p0p->wl)) + 0.5f;
    FVecInterp(p0p->vl, p0p->vr, yi, vz0);
    FVecInterp(p1p->vl, p1p->vr, yi, vz1);
    FVecInterp(vz0, vz1, xi, vz);
    FVecInterp(p0p->ve, p1p->ve, xi, vx);
    vy[0] = vz[1] * vx[2] - vz[2] * vx[1];
    vy[1] = vz[2] * vx[0] - vz[0] * vx[2];
    vy[2] = vz[0] * vx[1] - vz[1] * vx[0];
    for (i=0; i<3; i++) {
        uvs[0][i] = vx[i]; uvs[1][i] = vy[i]; uvs[2][i] = vz[i];
    }
    for (i=0; i<3; i++) pos0[i] = p0p->ve[i] * ((yi - 0.5f) * p0p->width) + p0p->pos[i];
    for (i=0; i<3; i++) pos1[i] = p1p->ve[i] * ((yi - 0.5f) * p1p->width) + p1p->pos[i];
    FVecInterp(pos0, pos1, xi, pos);
    m0 = yi * (p0p->m0r - p0p->m0l) + p0p->m0l;
    m1 = yi * (p1p->m0r - p1p->m0l) + p1p->m0l;
    i2 = xi * xi;
    m1 = -m1;
    offset = (xi * i2 * (m0 + m1) - i2 * (m1 + (m0 + m0)) + xi * m0);
    offset *= length;
    pos[0] -= offset * uvs[1][0];
    pos[1] -= offset * uvs[1][1];
    pos[2] -= offset * uvs[1][2];
    surfpos(pos, uvs, ipos, opos);
}

/* 0x80044FBC */
void fHalfPipeInterp(surftyp *sp, unsigned short pathIndex, float *ipos, float *opos, float (*uvs)[3], float dist)
{
    pathtyp *p0p;
    pathtyp *p1p;
    float r, r2, w, x, y, z, l;
    float fpos[3];
    p0p = &path[pathIndex];
    p1p = pathIndex + 1 < gRoadHead->pnum ? p0p + 1 : path;
    CopyUV(p0p->uvs, uvs);
    surfpos(p0p->pos, p0p->uvs, ipos, fpos);
    z = fpos[2];
    l = p0p->m1;
    if (z < 0.0f) z = 0.0f;
    else if (z > l) z = l;
    l = z / l;
    w = l * (p1p->w - p0p->w) + p0p->w;
    r = l * (p1p->width - p0p->width) + p0p->width;
    r -= w;
    r -= 2.5f;
    x = fpos[0];
    x += p0p->m0;
    y = fpos[1];
    if (uvs[1][1] < 0.0f) {
        froll(0.0f, -1.0f, uvs);
        x = -x;
        y = 5.0f + (-y + r);
    } else y -= r;
    if (x < -w) x += w;
    else if (x > w) x -= w;
    else {
        if (y > 0.0f) {
            opos[1] = r - y;
            froll(0.0f, -1.0f, uvs);
        } else opos[1] = r + y;
        return;
    }
    w = sqrtf(x * x + y * y);
    if (w < 0.01f) {
        opos[1] += r;
        return;
    }
    if ((opos[1] = r - w) < dist) {
        r2 = p0p->w + (p1p->width - p1p->w - p0p->width);
        r = p1p->w - p0p->w;
        if (fabs(r2) > 1.0) {
            l = sqrtf(r2 * r2 + p0p->m1 * p0p->m1);
            z = l;
            fpitch(-r2 / l, p0p->m1 / l, uvs);
        }
        froll(-x / w, -y / w, uvs);
        if (fabs(r) > 1.0) {
            l = r * fabs(x) / w;
            {
                float length = sqrtf(l * l + p0p->m1 * p0p->m1);
                fpitch(-l / length, p0p->m1 / length, uvs);
            }
        }
        if (fabs(r2) > 1.0) fpitch(r2 / z, p0p->m1 / z, uvs);
    }
}

/* floor helper written as an inline function (no DWARF entry): it reproduces the original truncate-and-adjust code at each call */
static float floorpos(float f)
{
    if ((f < 0.0f && (int)f != f) != 0) f -= 1.0f;
    return f;
}

/* 0x80045520 */
void TireSurf(tModelData *m, float *ipos, float *opos, int *roadcode, float (*uvs)[3], int whl)
{
    float tuvs[3][3];
    unsigned int i;
    unsigned short path, savepath;
    signed short quad;
    unsigned int sli;
    float dist, maxdist = 250.0f;
    float spos[3];
    unsigned short snum, slnum[35];
    unsigned char *slp;
    streetyp *ltp;
    surftyp *sp, *savesp = 0;
    float x, z;
    z = floorpos(ipos[2]);
    x = floorpos(ipos[0]);
    ltp = updownleaf(m->lasttp[whl], x, z, &quad);
    if (!ltp) goto failed;
    m->lasttp[whl] = ltp;
    sli = ltp->child[quad];
    if (ltp->flags & (16 << quad)) sli |= 0x10000;
    if (!sli) goto failed;
    slp = slist + sli;
    snum = *slp++;
    DecodeSlist(slp, snum, slnum, m->tiresli[whl]);
    for (i=0; i<snum; i++) {
        sp = &surf[slnum[i]];
        if ((sp->flags & 15) == 5 || (sp->flags & 15) == 6 || (sp->flags & 15) == 15) continue;
        dist = maxdist;
        if (PointInSurf(sp, ipos, 0, spos, &dist, &path, tuvs, -5.0f) && dist > -5.0f && dist < maxdist) {
            savepath = path;
            maxdist = dist;
            savesp = sp;
            opos[0] = spos[0]; opos[1] = spos[1]; opos[2] = spos[2];
            CopyUV(tuvs, uvs);
            m->tiresli[whl] = slnum[i];
        }
    }
    if (maxdist == 250.0f) goto failed;
    if (opos[1] < 1.0f && (savesp->snd_flags & 0x30)) m->movingSurface = savesp;
    if (savesp->flags & 0x2000) {
        fHalfPipeInterp(savesp, savepath, ipos, opos, uvs, 250.0f);
        if (opos[1] < -5.0f) goto failed;
    } else if (savesp->flags & 0x1000) fPathInterp(savesp, savepath, ipos, opos, uvs);
    *roadcode = savesp->flags & 15;
    m->sviscode[whl] = (savesp->flags >> 4) & 15;
    m->roadboost[whl] = (savesp->flags >> 8) & 15;
    if (savesp->flags & 0x8000) m->roadboost[whl] = -m->roadboost[whl];
    m->sound_flags[whl] = savesp->snd_flags;
    if (savesp->snd_flags & 0x30) m->sound_flags[whl] &= 0x7ff;
    if ((*roadcode == 3 || *roadcode == 4 || *roadcode == 7) && opos[1] < 0.25f &&
        !gGameCar[m->slot].dead && !gGameCar[m->slot].crash_state && m->resurrect.moving_state == -1) {
        if (*roadcode == 4) {
            AddBattlePoint(m->slot, m->slot, -1);
            m->crashflag = 1;
        } else if (*roadcode == 7) {
            m->resurrect.waterAbort = 1;
            AbortCar(m->slot, 1);
        } else {
            m->resurrect.waterAbort = 1;
            AbortCar(m->slot, 1);
            PlayGameSound(0x56, 0, 1, 2);
        }
    }
    return;
failed:
    *roadcode = 0;
    m->sviscode[whl] = 0;
    m->roadboost[whl] = 0;
    m->sound_flags[whl] = 0;
    opos[0] = ipos[0];
    opos[1] = ipos[1] - -200.0f;
    opos[2] = ipos[2];
    CopyUV(unitmatrix, uvs);
}

/* 0x80045990 */
surftyp *RoadPosition(float *ipos, float *opos, float (*uvs)[3])
{
    float tuvs[3][3];
    unsigned int i;
    unsigned short path, savepath;
    signed short quad;
    unsigned int sli;
    float dist, maxdist, savedist;
    float spos[3];
    unsigned short snum, slnum[35];
    unsigned char *slp;
    streetyp *ltp;
    surftyp *sp, *savesp = 0;
    int tries = 0;
    float x, z;
retry:
    z = floorpos(ipos[2]);
    x = floorpos(ipos[0]);
    ltp = updownleaf(stree, x, z, &quad);
    if (!ltp) goto failed;
    sli = ltp->child[quad];
    if (ltp->flags & (16 << quad)) sli |= 0x10000;
    if (!sli) goto failed;
    slp = slist + sli;
    snum = *slp++;
    DecodeSlist(slp, snum, slnum, -1);
    maxdist = savedist = 250.0f;
    for (i = 0; i < snum; i++) {
        sp = &surf[slnum[i]];
        if ((sp->flags & 15) == 5 || (sp->flags & 15) == 6 || (sp->flags & 15) == 15) continue;
        dist = maxdist;
        if (PointInSurf(sp, ipos, 0, spos, &dist, &path, tuvs, -20.0f) && fabs(dist) < maxdist) {
            savepath = path;
            savedist = dist;
            maxdist = fabs(dist);
            savesp = sp;
            opos[0] = spos[0]; opos[1] = spos[1]; opos[2] = spos[2];
            CopyUV(tuvs, uvs);
        }
    }
    if (maxdist == 250.0f) goto failed;
    if (savesp->flags & 0x2000) {
        fHalfPipeInterp(savesp, savepath, ipos, opos, uvs, 250.0f);
        if (opos[1] < -5.0f) goto failed;
        spos[0] = uvs[1][0] * -opos[1];
        spos[1] = uvs[1][1] * -opos[1];
        spos[2] = uvs[1][2] * -opos[1];
    } else {
        if (savesp->flags & 0x1000) {
            fPathInterp(savesp, savepath, ipos, opos, uvs);
            NormalVector(uvs[0]); NormalVector(uvs[1]); NormalVector(uvs[2]);
        }
        spos[0] = uvs[1][0] * -savedist;
        spos[1] = uvs[1][1] * -savedist;
        spos[2] = uvs[1][2] * -savedist;
    }
    opos[0] = ipos[0] + spos[0];
    opos[1] = ipos[1] + spos[1];
    opos[2] = ipos[2] + spos[2];
    return savesp;
failed:
    if (tries == 0) { ipos[0] += 1.0f; tries = 1; goto retry; }
    else if (tries == 1) { ipos[0] -= 1.0f; ipos[2] += 1.0f; tries = 2; goto retry; }
    else if (tries == 2) { ipos[2] -= 2.0f; tries = 3; goto retry; }
    return 0;
}

/* 0x80045D5C */
surftyp *CameraCollision(float *ipos, float *lookat)
{
    surftyp *sp;
    float surfuvs[3][3];
    float temp[3];
    sp = ShotToWorldCollision(lookat, ipos, surfuvs, 0.5f, 1, 6);
    if (sp) {
        temp[0] = lookat[0] - ipos[0];
        temp[1] = lookat[1] - ipos[1];
        temp[2] = lookat[2] - ipos[2];
        NormalVector(temp);
        ipos[0] += 0.25f * temp[0];
        ipos[1] += 0.25f * temp[1];
        ipos[2] += 0.25f * temp[2];
   }
    return 0;
}


/* inline helper: routing this one shift through an inlined return value moves the block's volatile registers onto the target's */
static signed short shalf(signed short v) { return v >> 1; }
surftyp *ShotToWorldCollision(float *old, float *new, float (*uvs)[3], float radius, int interp, int ignore)
{
#define SHOT_FLOOR(v) (((v < 0.0f && (int)v != v) != 0) ? v - 1.0f : v)
    float tuvs[3][3];
    unsigned int i;
    signed short quad0, quad1;
    int sli;
    float radiussq, t;
    float ipos[3], opos[3], epos[3];
    unsigned short snum, path, spath, slnum[35];
    signed short midh, midv;
    int state;
    unsigned char *slp;
    streetyp *ltp0, *ltp1;
    surftyp *sp, *savesp = 0;
    float x, z;
    ipos[0] = old[0]; ipos[1] = old[1]; ipos[2] = old[2];
    z = floorpos(ipos[2]);
    x = floorpos(ipos[0]);
    ltp0 = updownleaf(stree, x, z, &quad0);
    if (!ltp0) return 0;
    z = floorpos(new[2]);
    x = floorpos(new[0]);
    ltp1 = updownleaf(stree, x, z, &quad1);
    radiussq = radius * radius;
    do {
        /* small arrays the compiler scalarises into registers; like propagated scalars they are left out of the DWARF, which is why the original lists no locals for them */
        signed short b[4];
        float d[3];
        midh = ltp0->minh + ltp0->maxh;
        if (midh < 0) midh++;
        midv = ltp0->minv + ltp0->maxv;
        if (midv < 0) midv++;
        midh = midh >> 1; /* `x = x >> 1`, not `x >>= 1`: for a short the compound form is pushed down into each use */
        b[1] = midh;
        midv = shalf(midv);
        if (quad0 & 1) b[0] = midh; else b[0] = ltp0->minh;
        if (quad0 & 1) b[1] = ltp0->maxh;
        /* dead narrowing store (state is reassigned in the loop): the cast folds into the existing rlwinm., and an assigned temporary is coloured before shalf's return value, giving r3 to the test and r4 to midv */
        state = (unsigned char)(quad0 & 2);
        if (state) b[2] = ltp0->minv; else b[2] = midv;
        if (state) b[3] = midv; else b[3] = ltp0->maxv;
        sli = ltp0->child[quad0];
        if (ltp0->flags & (16 << quad0)) sli |= 0x10000;
        if (sli) {
            slp = slist + sli;
            snum = *slp++;
            DecodeSlist(slp, snum, slnum, -1);
            for (i = 0; i < snum; i++) {
                sp = &surf[slnum[i]];
                if ((sp->flags & 15) == 15 || (sp->flags & 15) == ignore) continue;
                state = CylinderInSurf(sp, ipos, new, opos, epos, tuvs, radiussq, 0, &path);
                if (state) {
                    if (opos[0] < b[0] || opos[0] >= b[1] || opos[2] < b[2] || opos[2] >= b[3]) continue;
                    new[0] = opos[0]; new[1] = opos[1]; new[2] = opos[2];
                    CopyUV(tuvs, uvs);
                    savesp = sp;
                    spath = path;
                }
            }
        }
        if (savesp) {
            if (interp) {
                if (savesp->flags & 0x2000) fHalfPipeInterp(savesp, spath, new, epos, uvs, 250.0f);
                else if (savesp->flags & 0x1000) {
                    fPathInterp(savesp, spath, new, epos, uvs);
                    NormalVector(uvs[0]); NormalVector(uvs[1]); NormalVector(uvs[2]);
                }
            }
            return savesp;
        }
        if (ltp0 == ltp1 && quad0 == quad1) return 0;
        d[0] = new[0] - ipos[0];
        d[1] = new[1] - ipos[1];
        d[2] = new[2] - ipos[2];
        if (d[0] < 0.0f) {
            opos[0] = b[0];
            t = (opos[0] - ipos[0]) / d[0];
            opos[2] = ipos[2] + t * d[2];
            opos[1] = ipos[1] + t * d[1];
            if (b[2] == opos[2] && d[2] < 0.0f) {
                ltp0 = updownleaf(stree, b[0] - 1, b[2] - 1, &quad0);
                goto next;
            } else if (b[3] == opos[2] && d[2] < 0.0f) {
                ltp0 = updownleaf(stree, b[0] - 1, b[3] - 1, &quad0);
                goto next;
            } else if (b[2] <= opos[2] && opos[2] <= b[3]) {
                ltp0 = updownleaf(stree, b[0] - 1, SHOT_FLOOR(opos[2]), &quad0);
                goto next;
            }
        } else if (d[0] > 0.0f) {
            opos[0] = b[1];
            t = (opos[0] - ipos[0]) / d[0];
            opos[2] = ipos[2] + t * d[2];
            opos[1] = ipos[1] + t * d[1];
            if (b[2] == opos[2] && d[2] < 0.0f) {
                ltp0 = updownleaf(stree, b[1], b[2] - 1, &quad0);
                goto next;
            } else if (b[3] == opos[2] && d[2] < 0.0f) {
                ltp0 = updownleaf(stree, b[1], b[3] - 1, &quad0);
                goto next;
            } else if (b[2] <= opos[2] && opos[2] <= b[3]) {
                ltp0 = updownleaf(stree, b[1], SHOT_FLOOR(opos[2]), &quad0);
                goto next;
            }
        }
        if (d[2] < 0.0f) {
            opos[2] = b[2];
            t = (opos[2] - ipos[2]) / d[2];
            opos[1] = ipos[1] + t * d[1];
            opos[0] = ipos[0] + t * d[0];
            if (b[0] == opos[0] && d[0] < 0.0f) {
                ltp0 = updownleaf(stree, b[0] - 1, b[2] - 1, &quad0);
                goto next;
            } else if (b[1] == opos[0] && d[0] < 0.0f) {
                ltp0 = updownleaf(stree, b[1] - 1, b[2] - 1, &quad0);
                goto next;
            } else if (b[0] <= opos[0] && opos[0] <= b[1]) {
                ltp0 = updownleaf(stree, SHOT_FLOOR(opos[0]), b[2] - 1, &quad0);
                goto next;
            }
        } else if (d[2] > 0.0f) {
            opos[2] = b[3];
            t = (opos[2] - ipos[2]) / d[2];
            opos[1] = ipos[1] + t * d[1];
            opos[0] = ipos[0] + t * d[0];
            if (b[0] == opos[0] && d[0] < 0.0f) {
                ltp0 = updownleaf(stree, b[0] - 1, b[3], &quad0);
                goto next;
            } else if (b[1] == opos[0] && d[0] < 0.0f) {
                ltp0 = updownleaf(stree, b[1] - 1, b[3], &quad0);
                goto next;
            } else if (b[0] <= opos[0] && opos[0] <= b[1]) {
                ltp0 = updownleaf(stree, SHOT_FLOOR(opos[0]), b[3], &quad0);
            }
        }
next:
        ipos[0] = opos[0]; ipos[1] = opos[1]; ipos[2] = opos[2];
    } while (ltp0);
    return 0;
#undef SHOT_FLOOR
}

/* 0x800469EC */
void MoveCollisionObject(int num, float *trans)
{
    unsigned int i;
    sverttyp *vp;
    savetyp *s;
    for (i = 0, s = save; i < savenum; s++, i++) {
        if (s->object == num) {
            float x, y, z;
            int ix, iy, iz;
            vp = &svert[s->vnum];
            x = ((vp->pos[0] << 5) + ((vp->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
            y = ((vp->pos[1] << 5) + ((vp->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
            z = ((vp->pos[2] << 5) + (vp->overflow & 31)) * (1.0f / 32.0f);
            x += trans[0];
            y += trans[1];
            z += trans[2];
            {
                float scaled = x * 32.0f;
                ix = scaled < 0.0f ? scaled - 0.5f : scaled + 0.5f;
            }
            {
                float scaled = y * 32.0f;
                iy = scaled < 0.0f ? scaled - 0.5f : scaled + 0.5f;
            }
            {
                float scaled = z * 32.0f;
                iz = scaled < 0.0f ? scaled - 0.5f : scaled + 0.5f;
            }
            vp->pos[0] = ix >> 5;
            vp->pos[1] = iy >> 5;
            vp->pos[2] = iz >> 5;
            vp->overflow = ((ix & 31) << 10) | ((iy & 31) << 5) | (iz & 31);
        }
    }
}

/* 0x80046B98 */
void RotateCollisionObject(int num, float *ipos, float (*orig)[3], float (*uvs)[3])
{
    unsigned int i;
    float p0[3];
    float vec[3];
    float surfuvs[3][3];
    float newuvs[3][3];
    sverttyp *vp;
    savetyp *s;
    for (i = 0, s = save; i < savenum; s++, i++) {
        if (s->object == num) {
            int ix, iy, iz;
            mats2fcopy(s->uvs, surfuvs);
            MulUVS(orig, surfuvs, newuvs);
            MulUVS(uvs, newuvs, surfuvs);
            matf2scopy(surfuvs, surf[s->snum].uvs);
            p0[0] = ((s->pos[0] << 5) + ((s->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
            p0[1] = ((s->pos[1] << 5) + ((s->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
            p0[2] = ((s->pos[2] << 5) + (s->overflow & 31)) * (1.0f / 32.0f);
            vec[0] = p0[0] - ipos[0];
            vec[1] = p0[1] - ipos[1];
            vec[2] = p0[2] - ipos[2];
            WorldVector(vec, p0, orig);
            WorldVector(p0, vec, uvs);
            p0[0] = vec[0] + ipos[0];
            p0[1] = vec[1] + ipos[1];
            p0[2] = vec[2] + ipos[2];
            ix = p0[0] * 32.0f;
            iy = p0[1] * 32.0f;
            iz = p0[2] * 32.0f;
            vp = &svert[s->vnum];
            vp->overflow = ((ix & 31) << 10) + ((iy & 31) << 5) + (iz & 31);
            vp->pos[0] = ix >> 5;
            vp->pos[1] = iy >> 5;
            vp->pos[2] = iz >> 5;
        }
    }
}

/* 0x80046DBC */
void ResetCollisionObject(int num)
{
    savetyp *s;
    int i;
    for (s = save, i = 0; i < savenum; i++, s++) {
        if (s->object == num) {
            memcpy(surf[s->snum].uvs, s->uvs, 18);
            memcpy(&svert[s->vnum], s->pos, 8);
       }
   }
}

/* 0x80046E5C */
void RemoveCollisionObject(int num)
{
    savetyp *s;
    int i;
    for (s = save, i = 0; i < savenum; i++, s++) {
        if (s->object == num) surf[s->snum].flags = 15;
   }
}

static signed char tireIndex[8] = {0,1,3,2,0,1,3,2};
/* 0x80046EA0 */
void CarApplyCollision(tModelData *m, anon_surf_uvs_30 *c)
{
    float tmp[3];
    float coord[3];
    float massEffect, dz;
    int i, whl;
    surftyp *sp;
    whl = tireIndex[c->carEdge];
    dz = c->dist;
    sp = c->surf;
    tmp[0] = c->uvs[1][0] * dz;
    tmp[1] = c->uvs[1][1] * dz;
    tmp[2] = c->uvs[1][2] * dz;
    m->rwr[0] = tmp[0] + m->rwr[0];
    m->rwr[1] = tmp[1] + m->rwr[1];
    m->rwr[2] = tmp[2] + m->rwr[2];
    m->reckon.rwr[0] = tmp[0] + m->reckon.rwr[0];
    m->reckon.rwr[1] = tmp[1] + m->reckon.rwr[1];
    m->reckon.rwr[2] = tmp[2] + m->reckon.rwr[2];
    for (i = 0; i < 4; i++) {
        m->bodyrwr[i][0] = tmp[0] + m->bodyrwr[i][0];
        m->bodyrwr[i][1] = tmp[1] + m->bodyrwr[i][1];
        m->bodyrwr[i][2] = tmp[2] + m->bodyrwr[i][2];
        m->tireRwr[i][0] = tmp[0] + m->tireRwr[i][0];
        m->tireRwr[i][1] = tmp[1] + m->tireRwr[i][1];
        m->tireRwr[i][2] = tmp[2] + m->tireRwr[i][2];
    }
    tmp[0] = m->bodyR[whl][1] * m->w[2] - m->bodyR[whl][2] * m->w[1];
    tmp[1] = m->bodyR[whl][2] * m->w[0] - m->bodyR[whl][0] * m->w[2];
    tmp[2] = m->bodyR[whl][0] * m->w[1] - m->bodyR[whl][1] * m->w[0];
    tmp[0] += m->v[0];
    tmp[1] += m->v[1];
    tmp[2] += m->v[2];
    WorldVector(tmp, coord, m->uvs);
    BodyVector(coord, tmp, c->uvs);
    m->appearance |= 0x100000;
    massEffect = 20000.0f / m->mass;
    i = sp->flags & 15;
    if (i == 5 || (sp->flags & 0x2000) || i == 6)
        dz = 0.0009f * (massEffect * m->idt);
    else
        dz = 0.015f * (massEffect * m->idt);
    coord[0] = -tmp[0] * dz;
    coord[2] = -tmp[2] * dz;
    if (m->drone_type == 1)
        dz = 0.05f + 0.1f * fabs(m->uvs[2][0] * c->uvs[1][0] + m->uvs[2][1] * c->uvs[1][1] + m->uvs[2][2] * c->uvs[1][2]);
    else
        dz = 0.05f + 0.2f * fabs(m->uvs[2][0] * c->uvs[1][0] + m->uvs[2][1] * c->uvs[1][1] + m->uvs[2][2] * c->uvs[1][2]);
    coord[1] = dz * (m->idt * (-tmp[1] * massEffect));
    WorldVector(coord, tmp, c->uvs);
    BodyVector(tmp, coord, m->uvs);
    if ((i == 5 || i == 6) && m->roadcode[whl] == 8 && fabs(coord[2]) < 4000.0 && fabs(coord[1]) < 4000.0) {
        m->centerForce[2] += coord[2];
        m->bodyForce[whl][0] += coord[0];
    } else {
        m->bodyForce[whl][0] = coord[0] + m->bodyForce[whl][0];
        m->bodyForce[whl][1] = coord[1] + m->bodyForce[whl][1];
        m->bodyForce[whl][2] = coord[2] + m->bodyForce[whl][2];
    }
}

static signed char BodyVerts[16] = {0,1,2,3,0,1,2,3,1,2,3,0,4,5,7,6};
/* 0x800472A0 */
void CollideCarBodyAgainstSurface(tModelData *m, surftyp *sp, anon_surf_uvs_30 *buf)
{
    unsigned int numVerts;
    unsigned short vertIndices[16];
    sverttyp *vert;
    float coord[3];
    float tmp[3];
    float bodyPoints[8][3];
    float surfPoints[16][2];
    float surfTrans[3][3];
    int isSurfReady = 0;
    int i, j, k, l;
    float crossY, y1, y2, dy, dx, dz, sx, sz, cand;
    int crossEdge, index;
    float a, t, x, z, sdx, sdz;
    numVerts = sp->snd_flags & 15;
    DecodeVlist(vlist + sp->vli, numVerts, vertIndices);
    vert = &svert[vertIndices[0]];
    coord[0] = ((vert->pos[0] << 5) + ((vert->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
    coord[1] = ((vert->pos[1] << 5) + ((vert->overflow & 0x3e0) >> 5)) * (1.0f / 32.0f);
    coord[2] = ((vert->pos[2] << 5) + (vert->overflow & 31)) * (1.0f / 32.0f);
    mats2fcopy(sp->uvs, surfTrans);
    for (i = 0; i < 4; i++) {
        tmp[0] = m->bodyrwr[tireIndex[i]][0] - coord[0];
        tmp[1] = m->bodyrwr[tireIndex[i]][1] - coord[1];
        tmp[2] = m->bodyrwr[tireIndex[i]][2] - coord[2];
        BodyVector(tmp, bodyPoints[i], surfTrans);
    }
    if ((sp->flags & 15) == 5 || (sp->flags & 15) == 6) dz = -3.0f;
    else if (surfTrans[1][0] * m->uvs[1][0] + surfTrans[1][1] * m->uvs[1][1] + surfTrans[1][2] * m->uvs[1][2] < -0.707f) dz = -4.5f;
    else dz = -0.5f;
    BodyVector(m->uvs[1], tmp, surfTrans);
    for (i = 0; i < 4; i++) {
        bodyPoints[i + 4][0] = bodyPoints[i][0] + tmp[0] * dz;
        bodyPoints[i + 4][1] = bodyPoints[i][1] + tmp[1] * dz;
        bodyPoints[i + 4][2] = bodyPoints[i][2] + tmp[2] * dz;
    }
    crossY = 0.0f;
    crossEdge = -1;
    for (l = 0; l < 8; l++) {
        i = BodyVerts[l];
        j = BodyVerts[l + 8];
        y1 = bodyPoints[i][1];
        y2 = bodyPoints[j][1];
        if (!(y1 * y2 < 0.0f)) continue;
        if (!isSurfReady) {
            for (k = numVerts - 1; k > 0; k--) {
                vert = &svert[vertIndices[k]];
                surfPoints[k][0] = ((vert->pos[0] << 5) + ((vert->overflow & 0x7c00) >> 10)) * (1.0f / 32.0f);
                surfPoints[k][1] = ((vert->pos[2] << 5) + (vert->overflow & 31)) * (1.0f / 32.0f);
            }
            isSurfReady = 1;
        }
        dy = y2 - y1;
        dx = bodyPoints[j][0] - bodyPoints[i][0];
        dz = bodyPoints[j][2] - bodyPoints[i][2];
        if (!dy) continue;
        a = fabs(y1 / dy);
        x = bodyPoints[i][0] + dx * a;
        z = bodyPoints[i][2] + dz * a;
        if (dy > 0.0f) {
            cand = a;
            dx = -dx;
            dz = -dz;
        } else cand = 1.0f - a;
        sx = surfPoints[1][0];
        sz = 0.0f;
        if (-sx * z > 0.0f) continue;
        if (dz) {
            t = -z / dz;
            if (t > 0.0f && t < cand) cand = t;
        }
        for (k = 2; k < numVerts; k++) {
            sdx = surfPoints[k][0] - sx;
            sdz = surfPoints[k][1] - sz;
            a = (x - sx) * sdz - (z - sz) * sdx;
            if (a > 0.0f) goto nextEdge;
            t = sdx * dz - sdz * dx;
            if (t) {
                t = a / t;
                if (t > 0.0f && t < cand) cand = t;
            }
            sx = surfPoints[k][0];
            sz = surfPoints[k][1];
        }
        a = sz * x - sx * z;
        if (a < 0.0f) continue;
        t = sx * dz - sz * dx;
        if (t) {
            t = a / t;
            if (t > 0.0f && t < cand) cand = t;
        }
        if ((sp->flags & 15) == 4) {
            if (!gGameCar[m->slot].dead && !gGameCar[m->slot].crash_state && m->resurrect.moving_state == -1) {
                AddBattlePoint(m->slot, m->slot, -1);
                m->crashflag = 1;
            }
        } else if ((sp->flags & 15) == 7) {
            if (!gGameCar[m->slot].dead && !gGameCar[m->slot].crash_state && m->resurrect.moving_state == -1) {
                m->resurrect.waterAbort = 1;
                AbortCar(m->slot, 1);
            }
        } else if ((sp->flags & 15) == 3) {
            if (!gGameCar[m->slot].dead && !gGameCar[m->slot].crash_state && m->resurrect.moving_state == -1) {
                m->resurrect.waterAbort = 1;
                AbortCar(m->slot, 1);
                if (gLink[m->slot].owner == 6) PlayGameSound(0x56, 0, 1, 2);
            }
            return;
        }
        cand *= dy;
        if (cand < 0.0f) cand = -cand;
        if (cand > crossY) {
            if (-y1 / y2 < 1.0f) k = i;
            else k = j;
            coord[0] = m->bodyrwr[tireIndex[k]][0] - m->bodyrwr2[tireIndex[k]][0];
            coord[1] = m->bodyrwr[tireIndex[k]][1] - m->bodyrwr2[tireIndex[k]][1];
            coord[2] = m->bodyrwr[tireIndex[k]][2] - m->bodyrwr2[tireIndex[k]][2];
            BodyVector(coord, tmp, surfTrans);
            if (tmp[1] >= 0.0f) continue;
            crossY = cand;
            index = l;
            crossEdge = k;
        }
nextEdge:;
    }
    if (crossEdge < 0) return;
    {
        surftyp *old = buf->surf;
        if (old) {
            if (buf->dist < crossY) {
                CarApplyCollision(m, buf);
                buf->surf = 0;
                CollideCarBodyAgainstSurface(m, sp, buf);
            } else {
                buf->surf = sp;
                CopyUV(surfTrans, buf->uvs);
                buf->carEdge = crossEdge;
                buf->index = index;
                buf->dist = crossY;
                CarApplyCollision(m, buf);
                buf->surf = 0;
                CollideCarBodyAgainstSurface(m, old, buf);
            }
        } else {
            buf->surf = sp;
            CopyUV(surfTrans, buf->uvs);
            buf->carEdge = crossEdge;
            buf->index = index;
            buf->dist = crossY;
        }
    }
}


/* 0x80047B88 */
void CollideCarBody(tModelData *m)
{
    streetyp *trees[4];
    signed short quads[4];
    streetyp *candidate;
    unsigned short slnum[140];
    int slcnt, numTrees = 0;
    int i, j, unique;
    signed short quadCand;
    anon_surf_uvs_30 collBuffer = {0};
    int sli;
    unsigned char *slp;
    unsigned short snum;
    for (i = 0; i < 4; i++) {
        float z = floorpos(m->bodyrwr[i][2]);
        float x = floorpos(m->bodyrwr[i][0]);
        candidate = updownleaf(m->lasttp[i], x, z, &quadCand);
        if (candidate) {
            unique = 1;
            for (j = numTrees - 1; j > -1; j--) {
                if (candidate == trees[j] && quadCand == quads[j]) {
                    unique = 0;
                    break;
                }
            }
            if (unique) {
                quads[numTrees] = quadCand;
                trees[numTrees++] = candidate;
            }
        }
    }
    slcnt = 0;
    for (i = 0; i < numTrees; i++) {
        sli = trees[i]->child[quads[i]];
        if (trees[i]->flags & (16 << quads[i])) sli |= 0x10000;
        if (sli) {
            slp = slist + sli;
            snum = *slp++;
            DecodeSlist(slp, snum, &slnum[slcnt], 0);
            for (j = slcnt; j < slcnt + snum; j++) {
                surftyp *sp = &surf[slnum[j]];
                if (collBuffer.surf == sp || (sp->flags & 0x4000) || (sp->flags & 0x2000) || (sp->flags & 15) == 15) continue;
                CollideCarBodyAgainstSurface(m, sp, &collBuffer);
                sp->flags |= 0x4000;
            }
            slcnt += snum;
        }
    }
    if (collBuffer.surf) CarApplyCollision(m, &collBuffer);
    for (i = 0; i < slcnt; i++) {
        surftyp *sp = &surf[slnum[i]];
        float dist;
        float opos[3], svel[3];
        unsigned short path;
        sp->flags &= ~0x4000;
        if (sp->flags & 0x2000) {
            for (j = 0; j < 4; j++) {
                dist = 250.0f;
                if (PointInSurf(sp, m->bodyrwr[tireIndex[j]], m->bodyrwr2[tireIndex[j]], opos, &dist, &path, collBuffer.uvs, -5.0f)) {
                    fHalfPipeInterp(sp, path, m->bodyrwr[j], opos, collBuffer.uvs, 0.0f);
                    dist = opos[1];
                    if (dist < -5.0f || dist >= 0.0f) continue;
                    opos[0] = m->bodyrwr[tireIndex[j]][0] - m->bodyrwr2[tireIndex[j]][0];
                    opos[1] = m->bodyrwr[tireIndex[j]][1] - m->bodyrwr2[tireIndex[j]][1];
                    opos[2] = m->bodyrwr[tireIndex[j]][2] - m->bodyrwr2[tireIndex[j]][2];
                    BodyVector(opos, svel, collBuffer.uvs);
                    if (svel[1] >= 0.0f) continue;
                    collBuffer.surf = sp;
                    collBuffer.carEdge = j;
                    collBuffer.index = j;
                    collBuffer.dist = -dist;
                    CarApplyCollision(m, &collBuffer);
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        m->bodyrwr2[i][0] = m->bodyrwr[i][0];
        m->bodyrwr2[i][1] = m->bodyrwr[i][1];
        m->bodyrwr2[i][2] = m->bodyrwr[i][2];
    }
}
