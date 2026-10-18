#include "dc_types.h"
/* Private C89 overlays: offsets and widths from the DC mbox instruction stream. */
#ifndef MBOX_PRIVATE_H
#define MBOX_PRIVATE_H

typedef struct tMboxInstance {
    char pad0[8];
    tUVMatrix *mat;        /* 8C03D2D8, +8 pointer; translation+24/+28/+2C. */
    char pad1[0x2A-0x0C];
    signed short child;   /* 8C03D114/11C +2A signed word, poolD1AC=base+2A. */
    signed short next;    /* 8C03D182/18A +2C signed word, poolD1B0=base+2C. */
    char pad2[0x58-0x2E];
} tMboxInstance;           /* stride88 from targetD0C8/D0CC andD2BC/D2BE. */
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
