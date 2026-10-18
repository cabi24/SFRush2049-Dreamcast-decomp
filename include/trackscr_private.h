#ifndef TRACKSCR_PRIVATE_H
#define TRACKSCR_PRIVATE_H
#include "dc_types.h"
typedef _tBlit _tBlit_2;
typedef struct tTrackButton {
 int buttonType,objIdx; float angle; tUVMatrix mat;
 unsigned char alpha; char pad3d[3];
} tTrackButton; /* stride40; type0/object4/angle8/matrixC/alpha3C */
typedef struct tTrackText {char pad0[4];unsigned char **single;char pad8[4];unsigned short *tab;unsigned char **taba;} tTrackText;
typedef struct tTrackCarousel {signed char gTrackNum;char pad1[3];float pos[3];float targetPos;signed char slot;char pad15[3];} tTrackCarousel; /* DEB2 byte0/DEB8 byte14;stride18 */
typedef struct tTrackCity {int objIdx;float cityX,cityY,cityRot,cityScale,scale;tUVMatrix mat;} tTrackCity; /* stride48;cleanup090AEA */
typedef struct tTrackProfileHead {tPlayerProfile **next;} tTrackProfileHead; /* partial prefix only;D23A/D23C */
typedef struct tTrackFilePak {char unknown00[0x10];unsigned char pak;} tTrackFilePak; /* partial prefix,D248/D250 */
typedef struct tTrackPlayerList {char unknown00[8];tPlayerProfile **head;} tTrackPlayerList;
typedef struct tTrackBlitInstance {unsigned char flags;char unknown1[0x1f];} tTrackBlitInstance; /* qualified flags-byte view, E75C..E770: 170F51+index*20 */
typedef struct tTrackFogTile {unsigned short uls,ult;} tTrackFogTile; /* F218/F22E/F232 */
typedef char check_track_button[(sizeof(tTrackButton)==0x40)?1:-1];
typedef char check_track_carousel[(sizeof(tTrackCarousel)==0x18)?1:-1];
typedef char check_track_city[(sizeof(tTrackCity)==0x48)?1:-1];
typedef char check_profile40[(sizeof(tPlayerProfile)==0x40)?1:-1];
extern unsigned int gGameState,gSkipToGameState,gActiveEdges,gActiveLevels,gActiveAutoEdges;
extern signed char gSoftReset,gTrackNum,gOptions[],gCircuitTrack,gCircuitPlayer,gMirrorControls,gBackwards,gMirrorWaves,gGhostSelect,gDeathMode,gFogColor,gFogIntensity,gWind,gDifficulty,gCatchup;
extern int gRace;
extern signed short gNumPlayers,gNumLaps,gNumDrones;
extern tCircuitTrack gCircuitTracks[];
extern tPadState gPlayers[4];
extern tPlayerProfile *gJustPlayers[4];
extern tTrackPlayerList gPlayerFiles;
extern tTrackBlitInstance trackBlitFlags[];
#define gBlitInstanceList trackBlitFlags
extern tTrackText gText;
extern unsigned char *gTrackTitles[];
extern tCamera gCamera[4];
extern tRGBA flashColors[];
extern volatile unsigned char gNumModels;
extern volatile float gFrameSeconds;
extern float unitmatrix[3][3];
extern unsigned char gFogColors[][3];
extern float trackButtonScale,trackButtonSpace,trackButtonX,trackMaxY,trackMinY,trackButtonZ,trackCursorZ;
extern float trackYFog[],trackPathPos,trackMapTransition,trackShowAngle,trackShowFogY;
extern float trackInitFogPos[3],trackStuntMapPos[3],trackBattleMapPos[3],trackScrollSpeed,trackCursorSpeed;
extern int trackPathWaveIdx,trackCurrentMap,trackDesiredMap,trackMapObject;
extern signed char trackInited;
extern _tBlit *trackBlit;
extern signed short trackOld,trackNumTracks,trackSelection,trackActiveOption,trackFirstOption,trackNumOptions,trackOpts[];
extern signed char trackActiveOptionEnabled,trackActiveZ,trackMapsBuilt,trackReady[];
extern tTrackCarousel trackCarousel[];
extern tTrackCity trackCities[];
extern tRGBA trackColors[],trackFogColor[];
extern tTrackFogTile trackFogTile;
extern char *trackMapObjNames[],*trackScreenNames[],*trackArrowNames[16][2];
extern int trackValidOption[7][21];
extern tUVMatrix trackMapMat;
extern tPoly *trackScreenPoly,*trackFogPoly;
extern void *gPathWave;
#define animTrackMap trackAnimTrackMap /* qualified trackscr external, DB46 poolDB80=8C08F784 */
#define buttonNames trackButtonNames
#define buttonInfo trackButtonInfo
#define buttonsInited trackButtonsInited
#define buttonsJustInited trackButtonsJustInited
#define mblit trackBlitDefs
#define inited trackInited
#define blit trackBlit
#define oldTrack trackOld
#define gNumTracks trackNumTracks
#define gTrack trackSelection
#define gActiveOption trackActiveOption
#define gFirstOption trackFirstOption
#define gNumOptions trackNumOptions
#define gOpts trackOpts
#define gActiveOptionEnabled trackActiveOptionEnabled
#define gActiveZ trackActiveZ
#define gTracks trackCarousel
#define validOption trackValidOption
#define yFog trackYFog
#define gButtonScale trackButtonScale
#define gButtonSpace trackButtonSpace
#define buttonX trackButtonX
#define maxY trackMaxY
#define minY trackMinY
#define buttonZ trackButtonZ
#define cursorZ trackCursorZ
#define pathWaveIdx trackPathWaveIdx
#define pathPos trackPathPos
#define currentTrackMap trackCurrentMap
#define desiredTrackMap trackDesiredMap
#define mapTransition trackMapTransition
#define trackMapObj trackMapObject
#define trackMap trackCities
#define gTrackReady trackReady
#define kOneRace 0
#define kPractice 1
#define kGhost 2
#define kCircuit 3
#define kStunt 4
#define kObstacle 5
#define kBattle 6
#define fabs _builtin_fabsf
float _builtin_fabsf(float);
void setGameState(unsigned int),SetOptions(void),ClearRushControls(void),createBG(void),removeBG(void);
void PlayStartABSound(unsigned int),PlayLRSound(unsigned int);
int AudPlayEffect(int,int,int,int);
signed char GetPlayerOption(tPlayerProfile **,unsigned char),GetPlayerTrack(tPlayerProfile **,int);
void SavePlayerOption(tPlayerProfile **,unsigned char,signed char),SavePlayerTrack(tPlayerProfile **,int,int);
int func_8C06D41A(int); /* D254 zero-extends byte; callee D41E multiplies full R4 by F0 */
int IsTrackUnlocked(int);
void ZOID_Reset(void),ZeroCamera(signed short),ZeroCameras(void),UpdateCameras(void);
void CopyUV(float (*)[3],float (*)[3]),YawUV(float,float (*)[3]),WPitchUV(float,float (*)[3]),WRollUV(float,float (*)[3]);
void ScaleUV(float (*)[3],float (*)[3],float),ScaleUVXYZ(float (*)[3],float (*)[3],float,float,float);
void ZOID_ProjectWindow(int,float *,tUVMatrix *,float *,signed short *);
void ZOID_SetColor(signed short,tRGBA *),ZOID_SetTexture(signed short,tTexture *,int),ZOID_RemoveObject(signed short);
void ZOID_RemovePoly(tPoly *),ZOID_UpdatePoly(tPoly *,void *,void *,void *,tRGBA *,void *,void *);
void MBOX_HideObject(int,int,int),MBOX_ShowObject(int,int,int);
int MBOX_FindObject_WORK(const char *,signed char,signed char,int);
signed short MBOX_NewObject(int,tUVMatrix *,int,unsigned int);
tTexture *MBOX_FindTexture_WORK(const char *,unsigned short *,signed char,signed char,int);
void *memcpy(void *,const void *,unsigned int);
_tBlit *NewMultiBlit(signed short,signed short,const tBlitDef *,signed short);
void RemoveBlit(_tBlit *),UpdateBlit(_tBlit *),RenameBlit(_tBlit *,const char *,int);
int Hidden(_tBlit *,int);
int HAL_SetFont(int);unsigned int HAL_GetTextWidth(void *,signed short);
void HAL_SetTextDepth(float),HAL_SetTextTranslucency(float);
void TB_SetJust(int,int),TB_SetTextColor(int),TB_Sprintf(void *,void *,...),TB_DrawText(signed short,signed short,void *),TB_FlashTextColors(tRGBA,tRGBA);
float TB_GetFlashTextCycle(void);
signed short TB_CenterText(void *,signed short);
int sprintf(char *,const char *,...);
int LoadWave(int,int,int,int,void *);void *GetWaveInstanceAddr(int);
void ReadCheckpointData(void),ReadPathData(void),UnloadWaveInstance(int,int);
void updateTrackScreen(void),_InitScreen(void),_UninitScreen(void),showTrackSelect(void);
int animNewSlider(_tBlit *),animArrows(_tBlit *),animFan(_tBlit *),animText(void),TrackValid(int),OptionValid(int);
void animButtonText(void),SetFogIntensity(int),BlowFog(void),loadTrackScreen(void),unloadTrackScreen(void),buildTrackMap(void),clearTrackMap(void),animTrackMap(void),buildTrackMaps(void),buildRaceTrackMaps(void),buildStuntTrackMaps(void),buildBattleTrackMaps(void),clearTrackMaps(void),animTrackMaps(void),loadPathData(void),unloadPathData(void),initButtons(void),uninitButtons(void),updateButtons(void);
#endif
