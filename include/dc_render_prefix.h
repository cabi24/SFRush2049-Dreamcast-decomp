#ifndef DC_RENDER_PREFIX_H
#define DC_RENDER_PREFIX_H
#include "dc_types.h"
typedef volatile unsigned int dcSQWord;
typedef struct dcVertexBits20 { unsigned int x,y,z,u,v; } dcVertexBits20;
typedef struct dcClipVertex40 {
 unsigned int opaque00[3];
 int clip; /* +C:06EFDA, fullstride28 from06EFCC..FD8. */
 unsigned int words[6]; /* +10..24:06EF14..2C copies into32-byte SQ packet. */
} dcClipVertex40;
typedef struct dcRendererPass {
 unsigned int **slots; /* +0:06F0AC; slotindex*4 at06F0AE..B6. */
 unsigned int opaque04;
 int *listType; /* +8:06F0A4..A8, dereference at06F0D6. */
} dcRendererPass;
typedef struct dcRendererStats {
 unsigned int selector; /* EE D4:7, API EED8 receives488-byte stackpacket. */
 char opaque04[0x28-4];
 unsigned int value28; /* EEDE loads stack+28. */
 char opaque2c[0x1e8-0x2c];
} dcRendererStats;
typedef dcSQWord *(*dcClipFunction)(dcSQWord *,dcClipVertex40 *,dcClipVertex40 *,dcClipVertex40 *,int,unsigned int);
typedef char dc_check_vertex20[(sizeof(dcVertexBits20)==20)?1:-1];
typedef char dc_check_clip40[(sizeof(dcClipVertex40)==40)?1:-1];
typedef char dc_check_stats488[(sizeof(dcRendererStats)==488)?1:-1];
extern dcRendererPass dcPass8C19E970;
extern unsigned int *dcSavedOpaque8C19F4F8,*dcSavedTrans8C19F4FC,*dcSavedPunch8C19F500;
extern unsigned int dcUsedOpaque8C19F4E0,dcUsedTrans8C19F4E4,dcUsedPunch8C19F4E8;
extern unsigned int dcMaxOpaque8C19F4EC,dcMaxTrans8C19F4F0,dcMaxPunch8C19F4F4;
extern signed char dcSampleEnabled8C0C9754,dcReverseQuad8C15FA14;
extern unsigned int dcStatsValue8C19E9D0,dcStatsMaximum8C19E9D4;
extern unsigned int dcPerfA8C19E95C,dcPerfB8C19E964,dcPerfC8C19E96C;
extern unsigned int dcPackets8C19E940,dcPlainTriangles8C19E930,dcClippedTriangles8C19E950;
extern dcClipFunction dcClipFunctions8C0C9778[16];
extern int dcCornerOrderA8C0C9758[4],dcCornerOrderB8C0C9768[4];
extern unsigned int **dcSelectedSlot8C19E9D8;
extern int dcCurrentList8C19E9DC;
extern unsigned int dcListStrips8C19E900[3],dcStrips8C19E920,dcTriangles8C19E928,dcVertices8C19E938;
void func_8C0D9076(int,unsigned int);
void func_8C0D8BC0(void);
void func_8C0DC2A0(dcRendererStats *);
void func_8C0DAC20(dcSQWord *,const void *,int);
#define DC_HW_CONTROL (*(volatile unsigned int *)0xff00001c)
#define DC_QACR0 (*(volatile unsigned int *)0xff000038)
#define DC_QACR1 (*(volatile unsigned int *)0xff00003c)
#endif
