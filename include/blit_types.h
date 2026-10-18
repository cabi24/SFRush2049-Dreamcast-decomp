#include "dc_types.h"
#ifndef DC_BLIT_TYPES_H
#define DC_BLIT_TYPES_H
/* Full DC blit overlay; target InitBlits proves stride0x40. */

 /* stride24:8C018B86..96 */
typedef char dc_check_blit[(sizeof(_tBlit)==0x40)?1:-1];
typedef char dc_check_blitdef[(sizeof(tBlitDef)==0x24)?1:-1];
extern _tBlit *inactiveBlit;
extern volatile unsigned char gNumModels;
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void ZOID_RemoveBlit(int idx);
void ZOID_UpdateBlit(int idx,int texIndex,tTexture *,void *,int x,int y,int width,int height);
void ZOID_CropBlit(int idx,int top,int bot,int left,int right);
void ZOID_SetBlitData(int idx,int hide,int alpha,unsigned short z);
void ZOID_FlipBlit(int idx,int flip);
void ZOID_TextBlit(int idx,int text);
int ZOID_AddBlit(int texIndex,tTexture *,void *,int x,int y,int width,int height);
void InitBlit(_tBlit *blit);
void UpdateBlit(_tBlit *blit);
void RenameBlit(_tBlit *blit,const char *name,int preserve);
_tBlit *NewBlit(const char *name,int x,int y,unsigned int cont);
#endif
