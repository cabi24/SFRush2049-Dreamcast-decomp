#ifndef PAUSESCR_TYPES_H
#define PAUSESCR_TYPES_H
#include "dc_types.h"
#include "text_types.h"
/* Four byte channels at BFC1A..1D; byte alignment is proved by the pointer at47588. */
typedef struct dcPauseColor { unsigned char r,g,b,a; } dcPauseColor;
typedef struct dcPauseTitlePos { signed short x,y; } dcPauseTitlePos;
/* 24-byte indexed rectangles: 47270..282; cx+10,cy+14 at471CA/1CC. */
typedef struct dcPauseRect { int x,y,w,h,cx,cy; } dcPauseRect;
/* Only pak+10 is named:46A58..68 and46B98..BA2. */
typedef struct dcPauseFile { char pad0[16]; unsigned char pak; } dcPauseFile;
extern int gPauseState;
#define gPlayers ((tPadState *)0x8C006000)
extern volatile tPadInfo gControllerData[4];
extern signed short gNumPlayers,gNumWindows;
extern signed char gSoftReset,gRestartGame,gRetireGame;
extern signed char gPauseControl[4];
extern unsigned int gGameState;
extern float gModelTime;
extern int playerTextColor[4];
extern tText gText;
void ResumeTimer(void); void PauseTimer(void);
void AudStopAllEffects(void); void AudPauseAllEffects(void); void AudResumeAllEffects(void);
void AbortGame(void); void PauseGameMusic(void); void ResumeGameMusic(void); void StartGameMusic(void);
void StartListeners(void); void StopListeners(void); void modelstop(void); void modelgo(void); void PauseTargets(void);
void SetControllerStickEdges(int,int); void SaveFlush(int); void SaveServer(int,int);
int SaveMessage(int,int,int,int,int *,int *,int (*)(int));
void AudPlayEffect(int,int,int,int);
void updateOptionScreen(void); void updateControlScreen(void); void updateAudioScreen(void); void updateVideoScreen(void);
void PlayUDSound(unsigned int); void PlayLRSound(unsigned int); void PlayStartABSound(unsigned int);
int PS_PakInserted(int); int PS_PakRumble(int);
void SaveSelectPlayer(tPlayerProfile **);
void RemoveBlit(_tBlit *); void RemovePanes(int);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void *InitPane(int,int,int,int,int,int (*)(_tBlit *),int,int);
void ChangePaneSize(void *,int,int,int,int);
int HAL_SetFont(int); void HAL_SetTextDepth(float); unsigned int HAL_GetTextWidth(void *,signed short);
signed short TB_GetFontLineHeight(void);
void TB_Sprintf(void *,void *,...);
void TB_SetTextColor(int); signed short TB_CenterText(void *,signed short); void TB_DrawText(signed short,signed short,void *);
int Hidden(_tBlit *,int); void UpdateBlit(_tBlit *);
#endif
