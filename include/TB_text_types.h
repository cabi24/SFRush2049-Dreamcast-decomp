#include "dc_types.h"
#ifndef TB_TEXT_TYPES_H
#define TB_TEXT_TYPES_H
typedef int enum_kTB_JustBottom;
typedef int enum_kTB_FlashText;
typedef struct anon_h_v_8 { int h,v; } anon_h_v_8; /* targetA73C/A740 long stores+0/+4 */
 /* targetA7FA..A806 complete4-byte value */
extern volatile float gFrameSeconds; /* targetA85C=8C12254C */
void HAL_InstallOncePerFrameFunc(void (*func)(void));
unsigned int HAL_GetTextWidth(void *,signed short);
void HAL_SetTextColor(unsigned char,unsigned char,unsigned char,unsigned char);
unsigned char HAL_GetFontAscent(void);
unsigned char HAL_GetFontDescent(void);
signed char HAL_GetFontLeading(void);
signed char HAL_GetFontSpacing(void);
unsigned int HAL_GetCharWidth(unsigned short);
void HAL_SetTextPos(signed short,signed short);
void HAL_DrawText(void *,signed short);
int vsprintf(char *,const char *,char *);
/* TargetAC22..AC3C andAC5E..AC80: next pointer after fmt, rounded to4-byte boundary by +4 or+8. */
typedef char *va_list;
#define va_start(ap,last) ((ap)=((unsigned int)((char *)&(last)+4)&3)?(char *)&(last)+8:(char *)&(last)+4)
#define va_end(ap) ((void)0)
typedef char checkRGBA[(sizeof(tRGBA)==4)?1:-1];
typedef char checkJust[(sizeof(anon_h_v_8)==8)?1:-1];
#endif
