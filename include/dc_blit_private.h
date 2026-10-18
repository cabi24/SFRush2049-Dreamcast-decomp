#ifndef DC_BLIT_PRIVATE_H
#define DC_BLIT_PRIVATE_H
#include "dc_types.h"
typedef struct tDCBlitTexture {
 char name[16];
 unsigned short width,height; /* 06B596..5A6: +10/+12, extu.w. */
 char opaque14[0x1c-0x14];
 unsigned int flags; /* 06B572/57A: +1C, bit20. */
 char opaque20[0x2c-0x20];
 void *strip; /* 06B6BA: +2C; SDKcallee06F090 dereferences this handle. */
} tDCBlitTexture;
typedef struct tDCBlitVertex {
 float fX,fY,fZ,fU,fV; /* 06B5FA..610,62A..666,67C..6AC; 4*20 stackbytes. */
} tDCBlitVertex;
typedef char dc_blit_texture_size[(sizeof(tDCBlitTexture)==0x30)?1:-1];
typedef char dc_blit_vertex_size[(sizeof(tDCBlitVertex)==20)?1:-1];
void func_8C06F062(void *,int,tDCBlitVertex *,unsigned int);
void func_8C06F1EC(int,int,int,int,int,int,int,int,float);
#endif
