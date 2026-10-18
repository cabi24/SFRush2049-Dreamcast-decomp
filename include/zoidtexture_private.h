#include "dc_types.h"
/* Private Dreamcast overlays, proven by zoidtexture target instructions. */
#ifndef ZOIDTEXTURE_PRIVATE_H
#define ZOIDTEXTURE_PRIVATE_H




           /* 0x58 stride: 8C0692AC/B0, all setter/getter entries. */

typedef struct tZoidModelLOD {
    unsigned short texidx; /* 8C069376 and 8C0693A2 word stores. */
    unsigned short flags;  /* 8C069378/37C word load/store +2. */
    char pad0[0x0C - 0x04];
} tZoidModelLOD;           /* 12-byte stride: 8C069352..35C, 8C06938C..39A. */

typedef struct tZoidModelDef {
    char pad0[0x16];
    signed short lodCount; /* 8C06935E, 8C069382, 8C0693AA: signed word at22. */
    tZoidModelLOD lod[4];   /* base24 from 8C06936C/372, size72 at 8C069338..350. */
} tZoidModelDef;

typedef struct tZoidRomObjDefEntry {
    tZoidModelDef *romObjDefs; /* 8C069344 pointer load at entry+0. */
    char pad0[4];
} tZoidRomObjDefEntry;      /* 8-byte stride: 8C06933C/340. */

extern tZoidObjectDC ObjInstanceList[]; /* pool8C0693B8=8C160870. */
extern tZoidRomObjDefEntry gRomObjDefPtrs[]; /* pool8C0693BC=8C0BEBE8. */

typedef char checkZoidObject[(sizeof(tZoidObjectDC)==0x58)?1:-1];
typedef char checkZoidLOD[(sizeof(tZoidModelLOD)==0x0C)?1:-1];
typedef char checkZoidModel[(sizeof(tZoidModelDef)==0x48)?1:-1];
typedef char checkZoidRomEntry[(sizeof(tZoidRomObjDefEntry)==0x08)?1:-1];
#endif
