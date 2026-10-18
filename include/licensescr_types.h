#ifndef DC_LICENSESCR_TYPES_H
#define DC_LICENSESCR_TYPES_H
#include "dc_types.h"
typedef struct tLicenseText {char pad0[4];unsigned char **single;} tLicenseText;
extern tLicenseText gText; /*DBEC gText+4,DBCC index3cc=243*4*/
extern unsigned int gContLevels[]; /*DBF4 base12C184, DB82 load32*/
extern char *dcBuildDate,*dcBuildTime; /*DBFC/BF8 pointers B8DA0/A4*/
void HAL_SetTextDepth(float);
int HAL_SetFont(int);
signed char HAL_SetFontFixedWidth(signed char);
void HAL_SetFontSpacing(signed short);
void TB_SetJust(int,int);
void TB_SetTextColor(int);
void TB_DrawTextScrollBox(signed short,signed short,signed short,signed short,signed short,signed short,void *);
void TB_Printf(signed short,signed short,void *,...);
extern signed char pfsInited,lcdInited,gSofdecShown,gLicenseShown;
extern unsigned int gActiveEdges;
void func_8C044584(void); /*address only, DAE4 callback; no guessed invocation ABI*/
void func_8C06CADC(void *); /*DA10 opaque callback pointer R4*/
void func_8C06CD14(int,int); /*DA18 R4=1,R5=0*/
void func_8C06C2A6(void); /*DA26 no args*/
void SetCountdownTimer(float);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
int CountdownTimeOut(void);
void RemoveBlit(_tBlit *);
void setGameState(unsigned int);
#endif
