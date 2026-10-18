#ifndef HAL_TEXT_TYPES_H
#define HAL_TEXT_TYPES_H
#include "dc_types.h"
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
