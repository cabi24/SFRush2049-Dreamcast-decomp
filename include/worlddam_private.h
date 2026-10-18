#include "dc_types.h"
#ifndef WORLDDAM_PRIVATE_H
#define WORLDDAM_PRIVATE_H

 /* Opaque: no polygon field dereference in this unit. */
typedef struct anon_full_oldest_68 {
    signed char full;      /* ACD8/ACEE byte+0. */
    unsigned short oldest; /* ACF0/AD32/AD4C signedload thenextu.w +2. */
    tPoly *polys[25];      /* AD02..0E +4; clear100bytes,4bytepointer. */
} anon_full_oldest_68;
 /* Target AEE8..AEF8 slot*(256+1)*8 proves stride808. */
extern tModelData model[];
extern unsigned char gNumModels;
extern unsigned short damTexture; /* targetpoolAEA4=8C1A701C; extent2only. */
extern anon_full_oldest_68 DamagePolys; /* targetpoolAE9C=8C1A6FB4. */
void *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
tPoly *ZOID_NewPoly(int,float (*)[3],unsigned short,tRGBA *,unsigned short,int);
void ZOID_HidePoly(tPoly *);
void ZOID_UpdatePoly(tPoly *,int,float (*)[3],unsigned short,tRGBA *,unsigned short,int);
void *RoadPosition(float *,float *,float (*)[3]);
#define fabs _builtin_fabsf
typedef char checkDamage[(sizeof(anon_full_oldest_68)==0x68)?1:-1];
typedef char checkModel[(sizeof(tModelData)==0x808)?1:-1];
#endif
