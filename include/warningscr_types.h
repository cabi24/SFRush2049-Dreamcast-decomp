#ifndef WARNINGSCR_TYPES_H
#define WARNINGSCR_TYPES_H
typedef struct tWarningBlit _tBlit;
typedef struct tWarningBlitDef {
    const char *texname;
    signed short dulx,duly,width,height,top,bot,left,right;
    unsigned int zdepth,alpha;
    int (*animfunc)(_tBlit *);
    unsigned int animid;
} tWarningBlitDef;
/* B638..B644 indexes (i*5)*4 and reads byte+0. No other fields claimed. */
typedef struct tWarningControllerData {
    signed char attached;
    char pad0[0x14-1];
} tWarningControllerData;
/* B7A8/B7B8 reads gText.single at +4, matching text's full verified layout. */
typedef struct tWarningText {
    unsigned short numSingle;
    unsigned char **single;
} tWarningText;
typedef char dc_check_warning_blitdef[(sizeof(tWarningBlitDef)==0x24)?1:-1];
typedef char dc_check_warning_controller[(sizeof(tWarningControllerData)==0x14)?1:-1];
extern tWarningControllerData gControllerData[4];
extern tWarningText gText;
extern int long_8C150464;
extern signed char byte_8C0BF760;
extern volatile unsigned int gActiveEdges;
extern unsigned char gBlocksNeeded;
int PS_PakInserted(int pak);
int PS_PakDamaged(int pak);
int PS_PakRumble(int pak);
unsigned int PS_GetFreeBlocks(int pak);
unsigned int PS_GetFreeFiles(int pak);
unsigned int PS_GetBlocks(int numBytes);
_tBlit *NewMultiBlit(signed short x,signed short y,const tWarningBlitDef *def,signed short count);
void RemoveBlit(_tBlit *blit);
void SetControllerActive(int which,int active);
void SetControllerStickEdges(int which,int active);
void PlayStartABSound(void);
void setGameState(unsigned int state);
void HAL_SetTextDepth(float depth);
int HAL_SetFont(int font);
void TB_SetJust(int horizontal,int vertical);
void TB_SetTextColor(int color);
signed short TB_GetFontLineHeight(void);
void TB_DrawTextScrollBox(signed short x,signed short y,signed short width,signed short height,signed short page,signed short line,void *text);
void TB_DrawText(signed short x,signed short y,void *text);
int sprintf(char *dest,const char *format,...);
int func_8C093780(_tBlit *blit);
#endif
