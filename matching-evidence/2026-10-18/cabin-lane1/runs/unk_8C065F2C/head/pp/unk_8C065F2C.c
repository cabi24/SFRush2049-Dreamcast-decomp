/* Target-backed world visibility/fog continuation; anonymous entry names retained. */
#line 1 "world_visibility_private.h"
#ifndef DC_WORLD_VISIBILITY_PRIVATE_H
#define DC_WORLD_VISIBILITY_PRIVATE_H
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
#line 4 "world_visibility_private.h"
typedef struct tModelPart {
 char name[16]; tUVMatrix mat; unsigned int dlflags;
 signed short next,child; char unknown48[8]; float min[3],max[3];
} tModelPart; /* sizeof68; matrix10;translation34;next44;min50/max5C */
typedef struct anon_x_z_10 {float x,z,dist,max;} anon_x_z_10;
typedef struct tWorldVisibilityObject {
 unsigned int flags; char unknown04[4]; tUVMatrix *mat; char unknown0c[0x58-0x0c];
} tWorldVisibilityObject; /* qualified fields0/8;stride58 at661AA/1BC */
typedef char check_worldpart[(sizeof(tModelPart)==0x68)?1:-1];
typedef char check_worldobject[(sizeof(tWorldVisibilityObject)==0x58)?1:-1];
typedef char check_window[(sizeof(tWindowInfo)==0x48)?1:-1];
extern tModelPart *WorldBase;
extern volatile signed short Worlddlidx;
extern tWorldVisibilityObject ObjInstanceList[];
extern tWindowInfo ZoidWinInfo[];
extern signed char gTrackNum,gFogIntensity,gInvisibleTrack;
extern signed short gNumPlayers;
extern int gPauseState;
extern float gViewableDistance;
#define sqrtf _builtin_sqrtf
float _builtin_sqrtf(float),Distance2D(float,float);
void WYawUV(float,tUVMatrix *);
void func_8C065F2C(tModelPart *,float *,int *,float *,int *,int *);
void func_8C06602E(int,float *,int),func_8C066278(int,float *);
#endif
#line 3 "unk_8C065F2C.c"

unsigned int track1Parts[117][4] = {
    0xd92145ff, 0xbff, 0x3fb8000, 0x14f018, 0x1ff, 0x803, 0x3d80000, 0x140018, 0x4907d7ff, 0x803,
    0x3d88000, 0x140018, 0x4024d7ff, 1, 0x1600000, 0x40018, 0x5903d7ff, 1, 0x400000, 0x70000,
    0x5800c7ff, 1, 0xc00000, 0x70000, 0x5903d7fe, 1, 0, 0x70000, 0x59bdfffe, 1, 0xc000000, 0x40000,
    0x5bfffffe, 1, 0x1c000000, 0x60000, 0x5ffffffe, 0, 0x1c000000, 0x40000, 0x5ffffffe, 0,
    0x1c000000, 0x40000, 0x5fffffae, 1, 0x1c000000, 0x40000, 0x5fffffa4, 1, 0x1c000000, 0x40000,
    0x5ffffffc, 1, 0x1c000000, 0x40000, 0x5ffffffe, 8, 0x1c000000, 0x58000, 0x5fffd7a0, 0, 0,
    0x40000, 0, 0, 0, 0, 0, 0, 0, 0, 0xdffdffa4, 0, 0x1c000000, 0x44000, 0x53fffffe, 0, 0x1c000000,
    0x40000, 0x5fffff84, 0, 0x1c000000, 0, 0xdfffff84, 2, 0x3c000000, 0x44000, 0xdfffff86, 0,
    0x1c000000, 0x4000, 0xdeffff00, 0, 0x1c000000, 0, 0, 0, 0, 0, 0xdffffe02, 0xc, 0x1c000000,
    0x4000, 0xdff7fa00, 0x3f, 0x1c000000, 0x4000, 0xdff79003, 0x2ff, 0x24000000, 0xc000,
    0xdff79003, 0x2f, 0x4000000, 0x4000, 0, 0, 0, 0, 0xde679003, 0x1cff, 0xe0000000, 0xc000,
    0xdff79003, 0x1dff, 0x61000000, 0x1e000, 0xfef71007, 0x1dff, 0xe2b00000, 0xe000, 0xfe270007, 0x80133c77,
    0xe3fb8000, 0x14f018, 0xfee60003, 0x1bff, 0x21200000, 0x1e000, 0xdee20003, 0x3fd, 0x1000000,
    0x2c000, 0xda220003, 0x3ff, 0x3800000, 0x1e000, 0xdee0000f, 0x1fff, 0x3fb8000, 0x14f018,
    0xd8000007, 0x134fff, 0x3000000, 0xe000, 0xda000003, 0x8403efff, 0x1000004, 0x4000, 0x48000003,
    0xc43bfff9, 0x3400005, 0, 0x48000003, 0xc41bffe1, 0x83000004, 0, 0xdc000007, 0x8417ffff,
    0x3c00004, 0x4000, 0xe8000007, 0x17ffa7, 0xe3f38000, 0x101018, 0x48000007, 0x841fffa7,
    0xe340010c, 0, 0x40000002, 0xc41fff83, 0x83000016, 0, 0x40000002, 0xc43bff81, 0x83000004, 0,
    0x40000002, 0x841bff81, 0x830001b4, 0, 0x40000002, 0x3bfb81, 0, 0, 0x40000002, 0x1bff83,
    0x83400000, 0x18, 0, 0, 0, 0, 0, 0xfffff300, 0x1000004, 0, 0, 0xfffff200, 0x1000004, 0, 0,
    0xffff0200, 0, 0, 0, 0xffff6300, 5, 0, 0, 0xfffd0200, 2, 0, 0, 0xffff0200, 0x3ab, 0x20, 0,
    0xfffc0000, 0x17ff, 0x80020, 0, 0xfff00000, 0x17ff, 0x1e0, 0, 0xffb00000, 0x17ff, 0x80000, 0,
    0, 0, 0, 0, 0xffb00000, 0x7ff, 0x80000, 0, 0xfe880000, 0x7ff, 0x80000, 0, 0xfed80000, 0x37ff,
    0, 0, 0x405c0000, 0xaf, 0, 0, 0xfe800000, 0x37ff, 0x80020, 0, 0xfe800000, 0x1fff, 0x80180, 0,
    0xfe800000, 0x37ff, 0x80020, 0, 0xfe800000, 0x1fff, 0x803e3, 0, 0xee800000, 0x37ff, 0x80000, 0,
    0xee800000, 0x3fff, 0x80183, 0, 0xee800000, 0x3fff, 0x80000, 0, 0xea800000, 0x7ff, 0x80007, 0,
    0xee800000, 0x3fff, 0x80000, 0, 0xee800000, 0x7ffe, 0x7e0, 0, 0xea800000, 0xfffe, 0, 0,
    0x22800000, 0x12ffec, 0, 0, 1, 0x32fc00, 8, 0x60000002, 1, 0x3bafc00, 0xf98, 0x60000002, 1,
    0x3bff800, 0x18, 0x20000002, 1, 0x3fff000, 0x18, 0x60000003, 1, 0x3fff220, 0x18, 0, 0, 0, 0,
    0x60000003, 3, 0x3fff368, 0x18, 0x60000007, 3, 0x3fff000, 0x2018, 3, 3, 0x3fbc000, 0x2018, 0,
    0, 0, 0, 0x60000007, 0x1b, 0x3ffc000, 0x14e018, 0x60000007, 3, 0x3ff8000, 0x143018, 0x60000007,
    0x2f, 0x3ff8000, 0x14fc18, 0x5fffffa4, 4, 0x1c000000, 0x40000, 0x3e7d680, 0, 0x1c000000, 0,
    0x5f1fffae, 0x19, 0x1c000000, 0x40000, 0xe800000f, 0x26003, 0xe3ff8000, 0x141018, 0xc0000000,
    0x26000, 0xe0000000, 0, 0xc0000000, 0x13f700, 0xe0000000, 0, 0, 0x60000000, 0x7fe, 7, 0, 0,
    0x168, 0xf, 0x20000000, 0, 0x300000, 0xf, 0x60000007, 3, 0x3ffd400, 0x14201e, 0x6000000f, 0x33,
    0x3ff8000, 0x10201c, 0, 0xffb00000, 0x17ff, 0x807e0, 0, 0xee800000, 0x37ff, 0x803e0, 0,
    0xee800000, 0x1ffe, 0x1803e0, 0, 0, 0x10, 0x7e0, 0, 0, 0x10, 0xfc0, 0x60000002, 1, 0x3bff000,
    0x3f18, 0, 0, 0x2200000, 0x3e00, 3, 3, 0x3fb8000, 0x103c18, 0x88000003, 0x3e, 0x3b88000,
    0x14f018, 0xde640007, 0xfff, 0x3800000, 0x1e000, 0xe8000007, 0x3ff, 0x2800000, 0x1e000,
    0x60000150, 4, 0, 0x3c000, 0x1000550, 0, 0, 0x78000, 0x5903c7ff, 1, 0x400000, 0x70000, 0,
    0xee800000, 0x37ff, 0x801a1, 0xfe000007, 0xbf, 0x3bf8000, 0x14d018
};
unsigned int track2Parts[117][4] = {
    0x1f87f, 0, 0, 0x17e500, 0x3f87f, 0, 0, 0x17fe00, 0xbf8ff, 0, 0, 0x17f400, 0x3f87f, 0, 0,
    0x17c400, 0x7787f, 0, 0, 0x17c400, 0xbf8ff, 0, 0, 0x17d400, 0x2ff8ff, 0, 0, 0x17c600, 0x7f8, 0,
    0, 0x110000, 0x780, 0, 0, 0, 0x780, 0, 0, 0, 0x780, 0, 0, 0, 0x926f787f, 0, 0, 0x178400,
    0x92ffff7b, 0, 0, 0x178400, 0x92fff87f, 0, 0, 0x178400, 0x17ff842, 0, 0, 0x178000, 0x13fff040,
    0, 0, 0x68000, 0x17fff87e, 0, 0, 0x178400, 0x1fff86a, 0, 0, 0x178000, 0x17fff800, 0, 0,
    0x68400, 0x17fff860, 0, 0, 0x78400, 0x1fffe000, 0, 0, 0x68000, 0x9ffff840, 0, 0, 0x78400,
    0xfffff840, 0x14, 0, 0x78400, 0xffffb000, 0, 0, 0x68000, 0xffffb000, 0x16, 0, 0x68000,
    0xffff8000, 0x10, 0, 0x68000, 0xfffd8000, 0x16, 0, 0x68000, 0xfff08000, 0x336, 0, 0,
    0xfff00000, 0x236, 0, 0, 0xfff08000, 0x23b7, 0, 0, 0xfff08000, 0x40063f7, 0, 0, 0xff800000,
    0x9402ebb7, 0, 0, 0xf0000000, 0x3b7, 0, 0, 0xfe000000, 0x406efb7, 0, 0, 0xfe000000, 0x9407fbb7,
    0, 0, 0xf8000000, 0x9401faff, 0, 0, 0xfe000000, 0x9407fbb7, 0, 0, 0xf0000000, 0x9407fff7, 0, 0,
    0xf0000000, 0x7faff, 0, 0, 0xf0000000, 0x3b7, 0, 0, 0xf0000000, 0x1407ffb7, 0, 0, 0xf0000000,
    0x1407ffb7, 0, 0, 0xd0000000, 0x7ff76, 0, 0, 0xf0000000, 0x940ffff7, 0, 0, 0xf4000000, 0x7fa76,
    0, 0, 0xf0000000, 0x940ffaf7, 0, 0, 0xf0000000, 0x96fffaff, 0, 0, 0xf0000000, 0x9effff7e, 0, 0,
    0x80000000, 0x7fe7e, 0, 0, 0xd0000000, 0xdffffa76, 0xa8, 0, 0xd0000000, 0x9ffffe36, 0, 0, 0,
    0x3e8000, 0, 0, 0x90000000, 0xffffc204, 0x404ef, 0, 0x90000000, 0xfffffa04, 0x404ef, 0,
    0x90000000, 0xffffc204, 0x404ef, 0, 0x80000000, 0xffffc014, 0x404ef, 0, 0x80000000, 0xfffe0004,
    0x404ff, 0, 0x90000000, 0xffffc004, 0x404ff, 0, 0, 0xfffe0000, 0x405df, 0, 0x80000000,
    0xfffe8004, 0x404ff, 0, 0, 0xfffe0000, 0x404ff, 0, 0, 0xfff80000, 0x405ff, 0, 0, 0xfffa0000,
    0x40fff, 0, 0, 0xfff80000, 0xfff, 0, 0, 0xfffa0000, 0x404ff, 0, 0, 0xfff80000, 0x405ff, 0, 0,
    0xfff80000, 0x42fff, 0, 0, 0xebe80000, 0xffd, 0, 0, 0xdf800000, 0x47ffe, 0, 0, 0xd4000000,
    0x446ffc, 0, 0, 0x84000000, 0x46fffc, 2, 0, 0x80000000, 0xc7fffc, 2, 0, 0x94000000, 0x467ffc,
    0, 0, 0x84000000, 0xf6fffc, 2, 0, 0x80000000, 0x467ff4, 0, 0, 0x80000000, 0xf7fff4, 2, 0, 0,
    0x1487ffd4, 2, 0, 0, 0xf7ffd4, 2, 0, 0, 0x1587f8d4, 2, 0, 0, 0x3587fbd4, 2, 0, 0, 0x7d8ff8c0,
    2, 0, 0, 0x7f8ff8c0, 6, 0, 0, 0x3f8ff8c0, 6, 0, 0, 0x3f8ff840, 6, 0, 0, 0x79700a60, 0, 0,
    0x80000000, 0x11700bfc, 0, 0, 0x80000000, 0x701ffe, 0, 0, 0, 0xff8ff8c0, 6, 0, 0, 0xffffda00,
    6, 0, 0, 0xffbfd800, 7, 0, 0, 0xff9f5000, 0xf, 0, 0, 0xff990000, 0xd, 0, 0, 0xffbf4000, 0x1d,
    0, 0, 0xff9f4000, 0xd, 0, 0, 0xff800000, 9, 0, 0, 0xff8e4000, 0x103d, 0, 0, 0xff8e4000, 0x187d,
    0, 0, 0x7f8ff8c0, 6, 0, 0, 0xffbf4000, 0x1d, 0, 0, 0xf4000000, 0xf879, 2, 0, 0xa0000000,
    0x9f9f9, 0, 0, 0x80000000, 0xd8f9, 0, 0, 0, 0x9fbf9, 0, 0, 0, 0x3e0, 0, 0, 0, 0x87e0, 0x8819,
    0, 0, 0x1687c0, 0xa87f, 0, 0, 0x17c700, 2, 0, 0x80000000, 0xff819, 2, 0, 0x80000000, 0xff819,
    0xb, 0, 0, 0xff819, 0x287f, 0, 0, 0xff819, 0x787f, 0, 0, 0x17ff10, 0x128ff, 0, 0, 0x17fe10,
    0xa87f, 0, 0, 0x17ee00, 0xa87f, 0, 0, 0x17e400, 0xa, 0, 0x80000000, 0x9f819, 0x378ff, 0, 0,
    0x17c400
};
unsigned int track3Parts[117][4] = {
    7, 0x4000, 0, 0x20100, 0x89ff, 0x4000, 0x10000, 0xa09fe, 0x1ff, 0, 0x10000, 0xa09fe, 0xff, 0,
    0x10000, 0x209fe, 0x10bfe, 0x4000, 0x10000, 0x209e4, 0x3efbe, 0x4000, 0x10000, 0xa09fe, 0x1fa,
    0, 0, 0x800, 0x3fffe, 0x4000, 0, 0x809c0, 0x7fff2, 0x4000, 0, 0xa09c0, 0x24ffffb0, 0x4000, 0,
    0x80840, 0x7ffa0, 0, 0, 0x80840, 0x3fffb0, 0x4400, 0, 0x840, 0x8fff00, 0x4400, 0, 0, 0x1fff20,
    0, 0, 0x840, 0xa4ffffa0, 0, 0, 0x840, 0xa5ffffa0, 0, 0, 0xa40, 0xa5ffffa0, 0x4000, 0, 0xa40,
    0xa7ffff80, 0x4000, 0, 0x200, 0xa7ffff00, 0, 0, 0x200, 0xaffffe00, 0x4400, 0, 0x200,
    0xafffee00, 0x4400, 0, 0x200, 0xbfffe200, 0x4400, 0, 0x200, 0xbfffe200, 0x401, 0, 0x200,
    0xfffb6000, 0x401, 0, 0x200, 0xffffe000, 0x4401, 0, 0x200, 0xfffe0000, 0x401, 0, 0x200,
    0xffff2000, 0x401, 0, 0x200, 0xfffe0000, 0x41d, 0, 0x200, 0x7dfe0000, 0x7d, 0, 0x200,
    0x7c800000, 0x3d, 0, 0, 0x7c000000, 0x47f, 0, 0x2000, 0xfffc0000, 0x45f, 0, 0x200, 0xffa00000,
    0xfff, 0, 0x2200, 0xe0000000, 0x89fff, 0, 0x102000, 0xfc000000, 0x4ff, 0, 0x2000, 0xfc000000,
    0x147f, 0, 0x2000, 0xf4000000, 0x3fff, 0, 0x100000, 0x70000000, 0x37ffd, 0, 0x100000,
    0x40000000, 0x3fff, 0, 0x100000, 0x20000000, 0xbfff7, 0, 0x142000, 0, 0xbfff4, 0, 0x142000, 0,
    0xbfff4, 0, 0x142000, 0, 0xbfff4, 0, 0x142000, 0, 0x12bfff4, 0, 0x142000, 0, 0x1fffff4, 0,
    0x142000, 0, 0xbffff4, 0, 0x142000, 0, 0, 0, 0, 0, 0x87fffff4, 0x80, 0x142000, 0, 0x8ffffff4,
    3, 0x140000, 0, 0xbffffff4, 0xc0383, 0x140000, 0, 0xdffffff4, 0xc0003, 0x140000, 0, 0xfffffff4,
    0xc0003, 0x140000, 0, 0xfffffef4, 0x1f, 0x140000, 0, 0xfffffc00, 0xe0003, 0x100000, 0,
    0xfffffff4, 0xe0003, 0x140400, 0, 0xfffffff4, 0xe001f, 0x140000, 0, 0xffffdff4, 0xc0007,
    0x100000, 0, 0xffffc400, 0x1e0003, 0x100400, 0, 0xfffbd400, 0x1e003f, 0x100406, 0, 0xfff84000,
    0x1e0003, 0x400, 0, 0xfffa4400, 0x1e0003, 0x100400, 0, 0xffea4400, 0x4e0003, 0x100400, 0,
    0xfffa4400, 0x3ffe0003, 0x108400, 0, 0xfffe4400, 0xe0003, 0x100400, 0, 0xfffe4400, 0x2bde0003,
    0x108400, 0, 0xffb35400, 0xc0003, 0, 0, 0xfffac000, 0xc003f, 0x100400, 0, 0x800000, 0x3c, 0, 0,
    0x800000, 0x14007c, 0, 0, 0x800000, 0x1d4007c, 0, 0, 0, 0x2ffe0070, 0x8000, 0, 0x20000, 0xf80,
    0, 0, 0x20000, 0x4f80, 0, 0, 0, 0x7f80, 0, 0, 0, 0xff00, 0, 0, 0, 0xff80, 0, 0, 0, 0x1fe00, 0,
    0, 0, 0x1fe00, 0, 0, 0, 0xff00, 0, 0, 0, 0x1fe00, 0, 0x1be, 0, 0x1b000, 0x201fe, 0, 0xffea4400,
    0xfffe0033, 0x108400, 0, 0xffea4000, 0xfffe007b, 0x108400, 0, 0xffe00000, 0xfffe0003, 0x8401,
    0, 0, 0xfffe0071, 0x8402, 0, 0x29000000, 0xfffe0061, 0x8402, 0, 0x20000000, 0xfffe0060, 0x8402,
    0, 0, 0xfffe0060, 0x8402, 0, 0, 0xfffe0060, 0x8417, 0, 0, 0xfffe0040, 0x8404, 0, 0, 0xfffe0040,
    0x8407, 0, 0x20000000, 0xfffe0000, 0xc417, 0xb8, 0x20000000, 0xfffe0040, 0xc43f, 0xa8,
    0x20000000, 0xfffe0040, 0xc43f, 0xa8, 0x20000000, 0xfffe0020, 0xc43f, 0xb8, 0x20000000,
    0xfffe0020, 0x1d47f, 0xbf, 0, 0xff000000, 0x3d1ff, 0xbf, 0, 0xd1010000, 0x3d1ff, 0xbf, 0,
    0xee000000, 0x3d1ff, 0xbf, 0, 0xc0010000, 0x351ff, 0x3f, 0, 0xe0010000, 0x351ff, 0x3f, 0,
    0xc8010000, 0x351ff, 0xbf, 0, 0xc0010000, 0xa51ff, 0xbf, 0, 0x10000, 0x301fe, 0xff, 0x4000,
    0x10000, 0xa19fe, 0xffffe000, 0x401, 0, 0x200, 0, 0x20000000, 0xfffe0000, 0x8400, 0x3fff2,
    0x4000, 0, 0xa0940, 0xbf, 0, 0xce010000, 0x351ff, 0x60000000, 0x8fa8b, 0, 0x102000, 0xb8, 0,
    0xffd00000, 0x3d0ff, 0, 0, 0xfffe0040, 0x8407, 0x2e, 0, 0xe0000000, 0x311ff, 0xbf, 0,
    0xc0010000, 0xb19ff, 0, 0x87fffff2, 0x80, 0x142000, 0x3eff6, 0x4000, 0x10000, 0xa09c8, 0,
    0x8bfbe0, 0, 0x142000
};
unsigned int track4Parts[121][4] = {
    0x3dff, 0, 0, 0x1f5e06, 0xdff, 0, 0, 0x1f5e06, 0, 0, 0, 0, 0x1dff, 0, 0, 0x195c02, 0x1dff, 2,
    0, 0x195c06, 0xdff, 0, 0, 0x195806, 0x427ff, 1, 0, 0x101006, 0x1dff, 0, 0, 0x19dc06,
    0x100473e0, 1, 0, 0x100002, 0x100473c0, 1, 0, 0x300000, 0x3fff, 0, 0, 0x195800, 0x7c1e, 0, 0,
    0x190800, 0xd0047f5e, 0x21, 0, 0x191800, 0x10047f5e, 0x21, 0, 0x100800, 0xd3ffff40, 0x361, 0,
    0x300000, 0xd3fff340, 0x371, 0, 0x300000, 0xfbbfc000, 0x63, 0, 0x300000, 0x90020000, 0x3b1, 0,
    0, 0xd33dfb40, 0x21, 0, 0x300000, 0xf73fc000, 0x21, 0, 0x300000, 0xbffb0000, 0x21, 0, 0x300000,
    0xbdfb0000, 0x21, 0, 0x300000, 0xfde00000, 0x3b, 0, 0x300000, 0xfdf00000, 0x3b, 0, 0x300000,
    0xffd00000, 0x3f, 0, 0x300000, 0xe2000000, 0x4002bf1, 0, 0, 0xfdfbc100, 0x21, 0, 0x300000,
    0xffe00000, 0x3b, 0, 0x300000, 0xffd00000, 0x3b, 0, 0x300000, 0xffd00000, 0x7f, 0, 0x300000,
    0xfbc00000, 0x7f, 0, 0x300000, 0xfbc00000, 0x3ff, 0, 0x300000, 0xffe00000, 0x1800007f, 0,
    0x300000, 0xff000000, 0x2fff, 0, 0x300000, 0xf9000000, 0x4fff, 0, 0x200000, 0xffd00000, 0x1ff,
    0, 0x300000, 0xf8000000, 0x4fff, 0, 0x200000, 0x68000000, 0x24004fff, 2, 0x200000, 0xf8004100,
    0x46fff, 0, 0, 0xc0000000, 0x5ffff, 0, 0, 0xe8000000, 0x65fff, 0, 0, 0xe8000000, 0x7fff, 0, 0,
    0x80000000, 0x2ffff4, 0, 0x1800000, 0x80000000, 0x3ffff4, 0, 0x800000, 0x100, 0x6fbdb0, 0,
    0x1800000, 0, 0x4affcb0, 0, 0x1800000, 0xd0000000, 0x7ffff, 0, 0, 0, 0x4ffbd20, 0, 0x1800000,
    0, 0x5fffc20, 0, 0x1800000, 0, 0x5ffbd00, 0x800, 0x1800000, 0, 0x4ffbc20, 0, 0x1800000, 0,
    0x5ffb820, 0, 0x1800000, 0, 0x5ff9c20, 0x800, 0x1800000, 0, 0x5ffb820, 0, 0x1800000, 0,
    0x5fc7c20, 0x800, 0x1800000, 0, 0x1ffd8020, 0x1802, 0x1800000, 0, 0x5fd80020, 0x1803, 0, 0,
    0x5fc80020, 0x7807, 0, 0, 0x5fc80df0, 0x7803, 0x1800000, 0, 0x7f800020, 0x80304f, 0, 0,
    0xff800020, 0x80304f, 0, 0, 0xff800026, 0x605f, 0x1800000, 0, 0xff800020, 0x8067ff, 0, 0,
    0xff800027, 0x605f, 0x1a00000, 0, 0xff800020, 0x28064ff, 0, 0, 0xff000020, 0x8067ff, 0, 0,
    0xfd000020, 0x28047ff, 0, 0, 0xfd000020, 0x8045ff, 0, 0, 0x7f000020, 0x2c047ff, 0, 0,
    0x77000020, 0x2c047ff, 0, 0, 0xff800020, 0x2c047ff, 0, 0x40000000, 0x7f000027, 0x2c0c7ff, 0, 0,
    0xfd000020, 0x2c0c7ff, 0, 0, 0x25000020, 0x2c0c7fe, 0, 0, 0x25000020, 0x3c3c7fe, 0, 0,
    0x1ffd9c20, 0x1806, 0x1800000, 0, 0x7f880020, 0x80380f, 0, 0, 0xff800020, 0x80747f, 0, 0,
    0xff000020, 0x2c047ff, 0, 0, 0x20, 0x578780, 0, 0, 0, 0x78680, 0x200000, 0, 0, 0xc2f8400,
    0x200000, 0, 0, 0x1c2f8000, 0x200000, 0, 0, 0x9eee0000, 1, 0, 0, 0x8308000, 0, 0, 0,
    0x3efe0000, 0, 0, 0, 0x1ee00402, 0, 0, 0x1000000, 0xbe04540, 0, 0, 0x1000000, 0xbe04440, 0, 0,
    0, 0x1ee00400, 0, 0x40000000, 3, 0xbeee0000, 1, 0, 0, 0xfeff0000, 0x11, 0, 0, 0xfe600000, 0x1f,
    0, 0, 0xfe200000, 0x1f, 0, 0, 0xfe200000, 0x5d, 0x80000000, 0, 0xf8000000, 0x780003, 0, 0,
    0xf0000000, 0x781c07, 4, 0, 0, 0x59fe07, 0, 0, 0, 0x593e07, 0, 0, 0xfa200000, 0x5928ff, 0, 0,
    0xf0000000, 0x59edff, 0, 0, 0xc0000000, 0x5dafff, 0, 0, 0x80000000, 0x580054, 0, 0, 0x80000000,
    0x5dafff, 0, 0, 0x80000000, 0x5dafff, 7, 0, 0, 0x5bfef6, 0x17, 0, 0, 0x5bfee4, 0xbf, 0, 0,
    0x5b3e06, 0x197, 0, 0, 0x5f7e04, 7, 0, 0x80000000, 0x5bffff, 1, 0, 0, 0x418c0, 0, 0, 0,
    0x58a5f6, 0x16, 0, 0, 0x7d7cf0, 0x97, 1, 0, 0x7b7e06, 0x1f, 0, 0, 0x5d70f0, 0x1f, 0, 0,
    0x5d70f0, 7, 0, 0, 0x5ffef6, 0xfff00000, 0x2b, 0, 0x300006, 0, 0, 0x80000000, 0x5da7ff, 0,
    0x5d83c00, 0x800, 0x1800000, 0, 0x4dc3e00, 0, 0x1800000
};
unsigned int track5Parts[103][4] = {
    0xffff, 0, 0x80ffc000, 0x21, 0xf3f, 0, 0xc40000, 0x20, 0x1ffff, 0, 0x80e40000, 0x23, 0x1f3f, 0,
    0xc00000, 0x21, 0x1d3f, 0, 0x800000, 0x20, 0xffff, 0, 0x80a00000, 0x37, 0xbff5, 0, 0x80e00000,
    0x61, 0xbfff, 0, 0x80a00000, 0x3f, 0x3ffff, 0, 0x80e00000, 0x33, 0x1fef, 0, 0x80e40000, 0x61,
    0x7fff, 0, 0x80a00000, 0x31, 0xcfffff, 0, 0x80e00000, 0x71, 0x7ff1, 0, 0x80a00000, 0x7f,
    0x9adfff7d, 2, 0x80000000, 0x60, 0xd3dfff7d, 0, 0x80800000, 0x60, 0x509fff4d, 0, 0x80000000,
    0x40, 0xfbdffd2d, 7, 0, 0x40, 0x71dffd0c, 1, 0, 0x40, 0xfffff400, 7, 0, 0x40, 0xfbfffc00, 0xb,
    0x8000000, 0x40, 0xfbff6400, 0xb, 0x8000000, 0x40, 0, 0, 0, 0x40, 0, 0, 0, 0, 0xffffe000, 0xf,
    0x8000000, 0x40, 0xfbff6400, 3, 0x8000000, 0x40, 0xfffd2000, 7, 0x8000000, 0x40, 0xffff2000,
    0xb, 0x8000000, 0x40, 0xfffd2000, 0xf, 0x8000000, 0x40, 0xffff0000, 7, 0x8000000, 0x40,
    0xfbffa200, 0x17, 0x8000000, 0x40, 0xfb9f0000, 7, 0, 0x40, 0xfbfde000, 0x5f, 0, 0x40,
    0xfafd2200, 0xf, 0, 0x40, 0xfbff2000, 0x1ff, 0, 0x40, 0xfa050000, 0x10bff, 0, 0x40, 0x83f80000,
    0xf, 0, 0x40, 0xe8000000, 0x33bff, 0, 0x40, 0, 0x37ff7, 0, 0x40, 0, 0x32ff7, 0, 0, 0, 0x20fff4,
    0, 0, 0, 0x20fff4, 0, 0, 0, 0x20fff4, 0, 0, 0, 0x460fff4, 0, 0, 0, 0x20ffa0, 0, 0, 0,
    0x8260ffa0, 0, 0, 0, 0xffe0ffe0, 0x3300002d, 0, 0, 0x43e0ff20, 0, 0, 0, 0xffe0fe08, 0x12000001,
    0, 0, 0x27fff7, 0x20000000, 0, 0, 0x2ffff4, 0x20000000, 0, 0, 0x3fffe0, 0x20000000, 0, 0,
    0xf7feffa0, 0x30000000, 0, 0, 0xfffcf800, 0x32000001, 0, 0, 0xfff8fa00, 0x3300002f, 0, 0,
    0xfff0fe00, 0x1300002f, 0, 0, 0xfff8fe80, 0x1700017f, 0, 0, 0xfff0fc80, 0x1700017f, 0, 0,
    0xfff0e800, 0x13000067, 0, 0, 0xffe0a800, 0x1300010f, 0, 0, 0xffe0a000, 0x10000000, 0, 0,
    0xfff0e000, 0x1200017f, 0, 0, 0xfff0f800, 0x1700097f, 0, 0, 0xfff0e800, 0x1700097f, 0, 0,
    0xffe0e800, 0x1700037f, 0, 0, 0xfff0e800, 0x17002b7f, 0, 0, 0xf3e02800, 0x17000b7f, 0, 0,
    0xf7e00000, 0x17003bff, 0, 0, 0xc2000000, 0x4706ffff, 0, 0, 0xe1e02800, 0x7009bff, 0, 0,
    0xc0000000, 0x7000bfb, 0, 0, 0xc0000000, 0x700dbff, 0, 0, 0xc3000000, 0x4506fbff, 0, 0,
    0xc7000000, 0x55067fff, 0, 0, 0, 0x447ffffe, 0, 3, 0, 0x44ffff00, 0, 0x200, 0xc0000000,
    0x4507ffdf, 0, 0x203, 0, 0x45ffffde, 0, 0x200, 0x80000000, 0x4537ffc5, 0x20, 0x203, 0x80000000,
    0x45ffffcc, 0x20, 0x203, 0x80000000, 0x44fffec8, 0x20, 0x203, 0, 0x44fffe40, 0x20, 0, 0, 0,
    0x20, 0, 0, 0, 0x20, 0xa0b, 0, 0x4fffe40, 0x20, 0x207, 0, 0x4fffe40, 0x60, 0x1fe7, 0,
    0x80ff8200, 0x61, 0xb5f, 0, 0x80ffce40, 0x20, 0x1fff, 0, 0x80ffee00, 0x21, 0, 0xe7604800,
    0x7007fff, 0, 0, 0xf7f0e800, 0x1700017f, 0, 3, 0, 0x47fbfffc, 0, 0xefffc000, 3, 0x8000000,
    0x40, 0, 0xffe0f800, 0x12000101, 0, 0, 0x3e0000, 0x20000000, 0, 0x200, 0, 0x4533ffcc, 0x20,
    0x1ffff, 0, 0x80e80000, 0x61, 0xffff, 0, 0x80a00000, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x7fff, 0, 0xc0ffe200, 0x21, 0xfffffa40, 0x3f, 0x88300000, 0x40
};
unsigned int track6Parts[122][4] = {
    7, 0, 0, 0x80, 0x1ff, 0, 0, 0x180, 0x1fe, 0, 0, 0x180, 0x1fe, 0, 0, 0x381, 0x3fe, 0, 0, 0x383,
    0x23fe, 0, 0, 0x383, 0, 0, 0, 0, 0, 0, 0, 0, 0x2f3e, 0, 0x40000000, 0x2000303, 0x3734, 0,
    0xe0000000, 0x2000247, 0x23f00, 0, 0x80000000, 0x200062b, 0x3800, 0, 0xc0000000, 0x47a,
    0x2fe00, 0, 0x80000000, 0x468, 0, 0, 0, 0, 0x3f000, 0, 0, 0xc00, 0x7f000, 0, 0, 0xc00, 0xfe000,
    0, 0, 0xc00, 0x3fa000, 0, 0, 0x800, 0x1f2000, 0, 0, 0x800, 0x5ff2000, 0, 0, 0x3800, 0x5fa2000,
    0, 0, 0x3800, 0x5fc3000, 0, 0, 0x3800, 0x7fa3000, 0, 0, 0x3800, 0x5ff82000, 0, 0, 0x3800,
    0x5ff80000, 0, 0, 0x7800, 0x5ff80000, 0, 0, 0x7000, 0x7fc00000, 0, 0, 0x6000, 0x7fc00000, 0, 0,
    0x6000, 0xffc00000, 1, 0, 0xc000, 0, 0, 0, 0, 0xff000000, 0xf, 0, 0xe000, 0xfc000000, 3, 0,
    0x1c000, 0xbc000000, 7, 0, 0x18000, 0xa4000000, 7, 0, 0x10000, 0x24000000, 0x2f, 0, 0x30000,
    0xe4000000, 0x3f, 0, 0x30000, 0, 0x38, 0, 0, 0x24000000, 0x7c, 0, 0x20000, 0x4000000, 0x3fe0,
    0, 0x60000, 0x4000000, 0x100037c0, 0, 0x60000, 0x4000000, 0x203fc0, 0, 0x60000, 0, 0x3c7fc0,
    0x100, 0xc0000, 0, 0x10783fc0, 0, 0x40000, 0, 0x13fc3fc0, 0x130, 0xc0000, 0, 0x33fc7fc0, 0x136,
    0xc0000, 0, 0x117cff40, 0, 0xc0000, 0, 0x4fa00, 0, 0xc0000, 0, 0x1f000, 0, 0, 0, 0x38000, 0, 0,
    0, 0x30000, 0x18a40, 0x300000, 0, 0x307cfe00, 0, 0xc0000, 0, 0xf3fc3800, 0x173, 0x80000, 0,
    0xfffc3f00, 0x133, 0xc0000, 0, 0xfffc3a00, 0x133, 0x1c0000, 0, 0x7ffc3a00, 0x132, 0x180000, 0,
    0xfffc3200, 0x132, 0x180000, 0, 0xfffc3a00, 0x13b, 0x180000, 0, 0xfff83000, 0x132, 0x180000, 0,
    0xfff81000, 0x17f, 0x180000, 0, 0xfff83000, 0xf7b, 0x180000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0xff200000, 0x3ff, 0x180000, 0, 0xfd200000, 0x3ff, 0x180000, 0, 0xfd200000,
    0x3ff, 0x100000, 0, 0xfd200000, 0x5ff, 0x100000, 0, 0, 0, 0, 0, 0x30000, 0x8a40, 0x300000, 0,
    0xf8030000, 0x1abff, 0x300000, 0, 0x38000000, 0x1abff, 0x300000, 0, 0, 0, 0, 0, 0xc0000000,
    0x1afe0, 0x300000, 0, 0, 0x6a5c08, 0, 0, 0, 0x5bbe0, 0x200000, 0, 0, 0x7a5400, 0, 0, 0, 0, 0,
    0, 0, 0xfffe60, 0x400000, 0, 0, 0xbdabe0, 0x600000, 0, 0, 0xfdebe0, 0x600000, 0, 0, 0xffffe0,
    0xc00000, 0, 0, 0x3fdc000, 0xe00000, 0, 0, 0x1fff700, 0x600000, 0, 0, 0xbfff600, 0xc00000, 0,
    0, 0, 0, 0, 0, 0xffd2000, 0x1c00000, 0, 0, 0x1ffc2000, 0x1c0000c, 0, 0, 0x1ffe2000, 0x1c00008,
    0, 0, 0x3ffc2000, 0x1c0000c, 0, 0, 0x3fe40000, 0x180000c, 0, 0, 0x3fe00000, 0x380000c, 0, 0,
    0x7fa00000, 0x380001c, 0x200, 0, 0xfc000000, 0x300005f, 0x300, 0, 0xe0000000, 0x2000207,
    0x1600, 0, 0xe0000000, 0x2000077, 0x738, 0, 0x60000000, 0x2000303, 0x130, 0, 0x60000000,
    0x2000207, 0, 0, 0, 0, 0x800, 0, 0xbf800000, 0x100007c, 0x2a00, 0, 0x99000000, 0x1000078,
    0x2a00, 0, 0xc0000000, 0x100007a, 0x3a00, 0, 0xc4000000, 0x100007a, 0x1ff, 0, 0, 0x180, 0x3fe,
    0, 0, 0x383, 0x2730, 0, 0x60000000, 0x2000303, 0xf000, 0, 0, 0x400, 0x1f2000, 0, 0, 0x800,
    0x5fa3000, 0, 0, 0x1800, 0x1ff80000, 0, 0, 0x7000, 0xff800000, 1, 0, 0x4000, 0xfc000000, 1, 0,
    0xc000, 0x24000000, 0xf, 0, 0x10000, 0xa4000000, 0xcef, 0, 0x70000, 0, 0x107c3fc0, 0x100,
    0x60000, 0, 0xfffc3800, 0x133, 0x80000, 0, 0xfd200000, 0x3ff, 0x100000, 0, 0, 0xa5abe0,
    0x200000, 0, 0, 0x3fff200, 0xe00000, 0, 0, 0x3ffc2000, 0x1c0000c, 0, 0, 0x7fa00000, 0x180000c,
    0x2a00, 0, 0xe0000000, 0x200007f
};
unsigned int dm1Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int dm2Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int dm3Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int dm4Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int dm5Parts[14][4] = {
    0x3fff, 0, 0, 0, 0x27ff, 0, 0, 0, 0x2fff, 0, 0, 0, 0x3fff, 0, 0, 0, 0x3fff, 0, 0, 0, 0x3fff, 0,
    0, 0, 0x3fff, 0, 0, 0, 0x3fff, 0, 0, 0, 0x3fff, 0, 0, 0, 0x3fff, 0, 0, 0, 0x3fef, 0, 0, 0,
    0x3ffd, 0, 0, 0, 0x3ff9, 0, 0, 0, 0x3ff9, 0, 0, 0
};
unsigned int dm6Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int dm7Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int dm8Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int stunt1Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int stunt2Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int stunt3Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int stunt4Parts[1][4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
unsigned int obstacle1Parts[25][4] = {
    7, 0, 0, 0, 0xf, 0, 0, 0, 0x1f, 0, 0, 0, 0x1f, 0, 0, 0, 0xfc, 0, 0, 0, 0xf0, 0, 0, 0, 0xf0, 0,
    0, 0, 0x7f0, 0, 0, 0, 0x780, 0, 0, 0, 0x780, 0, 0, 0, 0xf80, 0, 0, 0, 0x1e00, 0, 0, 0, 0x7800,
    0, 0, 0, 0x17000, 0, 0, 0, 0x1f000, 0, 0, 0, 0x7c000, 0, 0, 0, 0x1f000, 0, 0, 0, 0x68000, 0, 0,
    0, 0x1e0000, 0, 0, 0, 0x1c0000, 0, 0, 0, 0x3c0000, 0, 0, 0, 0x700000, 0, 0, 0, 0x1e00000, 0, 0,
    0, 0x1c00000, 0, 0, 0, 0x1c00000, 0, 0, 0
};
static unsigned char trackParts[19] = { 117, 117, 117, 121, 103, 122, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 25 };
static unsigned int showAllParts[4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };

void func_8C065F2C(tModelPart *Wnode, float *pos, int *best, float *dist, int *widx, int *index)
{
    float delta[3];
    float d;

    for (;;) {
        delta[0] = pos[0] - Wnode->mat.uvs[3][0];
        delta[1] = pos[1] - Wnode->mat.uvs[3][1];
        delta[2] = pos[2] - Wnode->mat.uvs[3][2];

        if (Wnode->min[0] <= delta[0] && delta[0] <= Wnode->max[0] && Wnode->min[1] <= delta[1] && Wnode->min[2] <= delta[2] &&
            delta[2] <= Wnode->max[2]) {
            d = sqrtf(delta[0] * delta[0] + delta[2] * delta[2]);
            if (delta[1] > 0.75f * Wnode->max[1]) {
                d += delta[1] - Wnode->min[1];
            }
            if (*best < 0 || d < *dist) {
                *best = *index;
                *widx = Wnode - WorldBase;
                *dist = d;
            }
        }
        if (Wnode->next >= 0) {
            (*index)++;
            Wnode = WorldBase + Wnode->next;
        } else {
            break;
        }
    }
}

void func_8C06602E(int unused, float *pos, int show)
{
    int dlindex; /* stack */
    int windex; /* stack */
    float dist; /* stack */
    int best; /* stack */
    int parts; /* r5 */
    int i; /* r8 */
    unsigned int mask; /* r0 */
    unsigned int *ptr; /* r30 */
    unsigned int *ptr_no_clip; /* r29 */
    int w; /* word of the part masks (not in the DWARF) */

    dlindex = 0;
    windex = 0;
    best = -1;
    if (!WorldBase) {
        return;
    }
    func_8C065F2C(WorldBase, pos, &best, &dist, &windex, &dlindex);
    ptr_no_clip = 0;
    if (best < 0) {
        ptr = showAllParts;
    } else {
switch (gTrackNum) {
        case 0:
            ptr = track1Parts[best];
            break;
        case 1:
            ptr = track2Parts[best];
            break;
        case 2:
            ptr = track3Parts[best];
            break;
        case 3:
            ptr = track4Parts[best];
            break;
        case 4:
            ptr = track5Parts[best];
            break;
        case 5:
            ptr = track6Parts[best];
            break;
        case 6:
            ptr = dm1Parts[best];
            break;
        case 7:
            ptr = dm2Parts[best];
            break;
        case 8:
            ptr = dm3Parts[best];
            break;
        case 9:
            ptr = dm4Parts[best];
            break;
        case 10:
            ptr = dm5Parts[best];
            break;
        case 11:
            ptr = dm6Parts[best];
            break;
        case 12:
            ptr = dm7Parts[best];
            break;
        case 13:
            ptr = dm8Parts[best];
            break;
        case 14:
            ptr = stunt1Parts[best];
            break;
        case 15:
            ptr = stunt2Parts[best];
            break;
        case 16:
            ptr = stunt3Parts[best];
            break;
        case 17:
            ptr = stunt4Parts[best];
            break;
        case 18:
            ptr = obstacle1Parts[best];
            break;
        }
    }
    parts = trackParts[gTrackNum];
    mask = 1;
    w = 0;
    for (i = 0; i < parts; i++) {
        if (i && !(i & 31)) {
            mask = 1;
            w++;
        }
        if (!gPauseState && ptr_no_clip && (mask & ptr_no_clip[w])) {
            WYawUV(0.0174532925f, ObjInstanceList[Worlddlidx + i].mat);
        }
        /* gInvisibleTrack is reread every pass (volatile here only; cheatscr.c needs it plain) */
        if (((mask & ptr[w]) || (ptr_no_clip ? mask & ptr_no_clip[w] : 0) || show) && !*(volatile signed char *)&gInvisibleTrack) {
            ObjInstanceList[Worlddlidx + i].flags &= ~0x80000000;
        } else {
            ObjInstanceList[Worlddlidx + i].flags |= 0x80000000;
        }
        mask <<= 1;
    }
}

anon_x_z_10 track1Fog[2] = { 1000.0f, 1800.0f, 1000.0f, 500.0f, 0.0f, 0.0f, 0.0f, 0.0f };
anon_x_z_10 *trackFog[19] = { (void *)&track1Fog, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static signed short fogDistance[4][4] = {
    4000, 2300, 1500, 400, 4000, 2300, 1500, 400, 4000, 2300, 1500, 400, 4000, 2300, 1500, 400
};
static float fogMin[4][4] = {
    0.0f, -2000.0f, -500.0f, 0.0f, 0.0f, -2000.0f, -500.0f, 0.0f, 0.0f, -2000.0f, -500.0f, 0.0f,
    0.0f, -2000.0f, -500.0f, 0.0f
};
static signed short fogMax[4][4] = {
    0, 1400, 1600, 2000, 0, 1400, 1600, 2000, 0, 1400, 1600, 2000, 0, 1400, 1600, 2000
};

void func_8C066278(int winidx, float *pos)
{
    tWindowInfo *win; /* r31 */
    anon_x_z_10 *wf; /* r30 */
    anon_x_z_10 *best; /* r29 */
    float bestDist; /* f30 */
    float ratio; /* f3 */
    float dist; /* f1 */
    float bestMax;

    win = &ZoidWinInfo[winidx];
    win->fogmin = fogMin[gNumPlayers - 1][gFogIntensity];
    win->fogmax = fogMax[gNumPlayers - 1][gFogIntensity];
    win->backclip = fogDistance[gNumPlayers - 1][gFogIntensity];
    gViewableDistance = win->backclip;
    wf = trackFog[gTrackNum];
    if (!wf || !gFogIntensity) {
        return;
    }
    bestDist = win->backclip;
    best = 0;
    for (; wf->dist != 0.0f; wf++) {
        dist = Distance2D(wf->x - pos[0], wf->z - pos[2]);
        if (dist < wf->dist && dist < bestDist) {
            best = wf;
            bestDist = dist;
        }
    }
    if (best) {
        if (best->dist > win->backclip) {
            ratio = bestDist / win->backclip;
        } else {
            ratio = bestDist / best->dist;
        }
        bestMax = best->max;
        win->fogmin = ratio * (win->fogmin - bestMax) + bestMax;
        if (win->fogmax < win->fogmin) {
            win->fogmax = win->fogmin;
        }
    }
}
