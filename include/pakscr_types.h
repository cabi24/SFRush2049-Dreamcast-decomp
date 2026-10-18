#ifndef PAKSCR_TYPES_H
#define PAKSCR_TYPES_H
#include "dc_types.h"
#include "text_types.h"
/* Partial file: next+0 EAF6..EB08; pak+10 F096..F0A0; filename+18 F63x;
   blocks+38 F660..F664. No full file size or filename extent is claimed. */
struct tPS_File {
 tDLLE ll;
 char pad08[8];
 unsigned char pak;
 char pad11[0x18-0x11];
 char filename[1];
 char pad19[0x38-0x19];
 unsigned int blocks;
};
/* Both profile and ghost linked nodes expose next+0,fileh+8: F08C..F0CC. */
typedef struct dcPakPlayer { tDLLE ll; tPS_File **fileh; } dcPakPlayer;
typedef struct dcPakGhost { tDLLE ll; tPS_File **fileh; } dcPakGhost;
/* MAT3 names retain the actual enum identity, not the generated typedef-int erasure. */
typedef enum dcPakState { kStatePickPak=0,kStateFileDel,kStateFileDelVerify,kStateRumblePak,kStateDamaged,kStateNoController,kStateNoPak,kNumStates } dcPakState;
extern tDLList gPS_Files[8],gPlayerFiles,gGhostFiles;
extern volatile tPadInfo gControllerData[4];
extern tText gText;
extern unsigned int gGameState,gActiveEdges,gActiveAutoEdges,gActiveLevels;
int PS_PakChanged(); int PS_PakDamaged(); int PS_PakInserted(); int PS_PakRumble();
unsigned int PS_GetFreeBlocks(int); unsigned int PS_GetFreeFiles(int);
void PS_DeleteFile(tPS_File **);
void ShowPakError(int,signed char *,signed char *,signed char *,int (*)(int));
void SetPakError(int,int,void *,int,int,int,int,int);
void createBG(); void removeBG(); void SetControllerActive(int,int); void setGameState(unsigned int);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *); int AudPlayEffect(int,int,int,unsigned char);
void PlayLRSound(unsigned int); void PlayStartABSound(unsigned int);
void HAL_SetTextDepth(float); int HAL_SetFont(int); void TB_SetTextColor(int);
signed short TB_GetFontLineHeight(); signed short TB_CenterText(void *,signed short); signed short TB_RightText(void *,signed short);
void TB_DrawText(signed short,signed short,void *); void TB_SetJust(int,int);
void TB_DrawTextScrollBox(signed short,signed short,signed short,signed short,signed short,signed short,void *);
void TB_Sprintf(void *,void *,...); void *TB_Strcpy(void *,void *); void *TB_Strcat(void *,void *);
#endif
