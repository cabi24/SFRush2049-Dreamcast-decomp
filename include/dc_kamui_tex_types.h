/* Private DC overlays. All anonymous function IDs and original ranges retained. */
#ifndef DC_KAMUI_TEX_TYPES_H
#define DC_KAMUI_TEX_TYPES_H
#include "dc_types.h"
typedef struct tDcVibrationRecord {
 char unknown0[6]; unsigned char active; char unknown7[0x4d];
 unsigned char info[0x88]; char level; char submittedLevel; char unknownDE[0x12];
} tDcVibrationRecord; /* F0 stride E5FC/E7DA; active+6, info54, levelDC/DD. */
typedef struct tDcVibrationRequest {
 unsigned char channel,continuous,power,frequency,unknown4,unknown5,unknown6,unknown7;
} tDcVibrationRequest;
typedef struct tDcControllerPresent { unsigned char present; char unknown1[0x13]; } tDcControllerPresent;
typedef struct tDcKmSurface {
 unsigned int unknown0,unknown4,pixelFormat,unknownC,unknown10,bytes,unknown18;
 void *pixels;
} tDcKmSurface; /* 20 stride E982..98A; pixelFormat8, bytes14, pixels1C. */
typedef struct tDcKmContext { char opaque[0x40]; } tDcKmContext;
typedef struct tDcTexture {
 char unknown0[0x10]; unsigned short width,height; char unknown14;
 unsigned char kind; char unknown16[2]; void *pixels;
 unsigned int flags,extraFlags; tDcKmSurface *surface; tDcKmContext *context;
} tDcTexture; /* Only accessed prefix; no resource-array stride asserted. */
typedef struct tDcKmSetup {
 unsigned int size,listType,unknown8,unknownC,unknown10,unknown14,unknown18,unknown1C;
 unsigned int mirrorMode,unknown24,unknown28,unknown2C,unknown30,unknown34,unknown38,unknown3C;
 unsigned int unknown40,unknown44,unknown48,unknown4C;
 unsigned int srcBlend,dstBlend,unknown58,unknown5C,unknown60,unknown64,alpha,unknown6C;
 unsigned int clampV,clampU,filter,unknown7C,unknown80,unknown84,unknown88;
 tDcKmSurface *surface;
} tDcKmSetup; /* 90 bytes from EB38/EC44; meaningful assigned members at target offsets. */
extern tDcVibrationRecord dcVibrationRecords[8];
extern tDcControllerPresent dcControllerPresent[4];
extern unsigned char dcVibrationPorts[8];
extern unsigned char dcVibrationEnabled;
extern int dcTextureScanIndex,dcTextureCount;
extern tDcTexture *dcTextureTable[1024];
extern tDcKmSurface dcKmSurfaces[1024];
extern tDcKmContext dcKmContexts[1024];
extern tDcKmContext dcKmDefaultContext,dcKmOtherContext;
extern unsigned int gTextureRamUsed,dcTextureRamHigh;
extern unsigned char gMirrorWaves,gFogIntensity;
extern const char dcCreateTextureName[],dcLoadTextureName[],dcFreeTextureName[];
extern int func_8C0D7954(unsigned int);
extern int func_8C0D7B2A(unsigned int,void *);
extern int func_8C0D7CFC(unsigned int,void (*)(int,int,void *,int),int);
extern int func_8C0D7A2E(unsigned int,tDcVibrationRequest *,unsigned int);
extern void func_8C06E51A(int,int,void *,int);
extern void func_8C0D9076(void *,unsigned int);
extern int func_8C0DB9C0(tDcKmContext *,int);
extern int func_8C0DBFE0(tDcKmSurface *,unsigned int,unsigned int,unsigned int);
extern int func_8C0DC100(tDcKmSurface *,const void *);
extern int func_8C0DC0A0(tDcKmSurface *);
extern int func_8C0DBBE0(tDcKmContext *,tDcKmSetup *);
extern int func_8C0DBD60(tDcKmContext *,tDcKmSetup *);
extern void dcCheckKm(int,const char *);
extern tDcTexture **func_8C06E8BC(tDcTexture *);
extern void func_8C06EBB2(tDcKmContext *,tDcKmSurface *,unsigned int);
#endif
