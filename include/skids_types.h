#include "dc_types.h"
#ifndef DC_SKIDS_TYPES_H
#define DC_SKIDS_TYPES_H
typedef tCarData tSkidCar;
typedef tModelData tSkidModel;


typedef struct tStatDLLE { void *next; char pad0[4]; } tStatDLLE;



/* DC packed-object backend is absent. Size0x24 at 8C04EE96. */
typedef struct tSkid {
    tStatDLLE ll;       /* next+0: 8C04EB1E, 8C04F2F0 */
    float lastTime;     /* +8: 8C04EA5E..62, 8C04F296..29E */
    tPoly *poly;        /* +c: 8C04EA92, 8C04EBB2 */
    float pos[3];       /* +10: 8C04EAE6, 8C04EC9A */
    tRGBA color;        /* +1c: 8C04EA5C..82, 8C04EB40..5A */
    int active;         /* +20: 8C04EB44, 8C04ECEA */
} tSkid;

/* sizeof0x5c: UpdateSkids step at 8C04EFE4 etc; six rows*four*0x5c=0x8a0 at 8C04EEA4. */
typedef struct tNewSkid {
    tSkid *skid;        /* +0: 8C04E8CA, 8C04EBEE */
    float start[3];     /* +4: 8C04EC04..18 and 8C04ED40..70 */
    float end[3];       /* +10: 8C04E968 and 8C04EDC8..DE8 */
    float lensq;        /* +1c: 8C04EBEA..F0, 8C04ED7C/99A */
    float dir[3];       /* +20: 8C04EDA6..DC8, 8C04EE2A..42 */
    float vert[4][3];   /* +2c: 8C04EA6A, 8C04EC06..66 */
} tNewSkid;

/* Array stride0x3cc from pools 8C04EA30/EE56/EF76; tire coordinates+74 from 8C04E8C2. */


/* Model stride0x808 at 8C04E8A2..BA and 8C04EEB0..EEDE. */


typedef struct tSkidCamera { char pad0[0x24]; float pos[3]; } tSkidCamera; /* 8C04EAE4 */
typedef struct tSkidSurface {
    char pad0[2]; unsigned short snd_flags; /* 8C04EF2E..F32 */
    char pad1[0x18 - 4]; /* stride0x18: 8C04EF22..F2C */
} tSkidSurface;

typedef char dc_check_skid[(sizeof(tSkid)==0x24)?1:-1];
typedef char dc_check_newskid[(sizeof(tNewSkid)==0x5c)?1:-1];
typedef char dc_check_skidcar[(sizeof(tSkidCar)==0x3cc)?1:-1];
typedef char dc_check_skidmodel[(sizeof(tSkidModel)==0x808)?1:-1];
typedef char dc_check_skidsurface[(sizeof(tSkidSurface)==0x18)?1:-1];

#endif
