#ifndef DC_CARSCR_TYPES_H
#define DC_CARSCR_TYPES_H
#include "dc_types.h"
/* Private minimum file prefix; allocation size/stride remains unknown. */
struct tPS_File {
 char unknown00[0x10];
 unsigned char pak; /* 771C0..1CC and728DC..8EA. Minimum known field extent only. */
};
typedef struct tCarScreenMenuButton {
 int buttonType,objIdx; /* 0/+4:78E8A,788B8; objIdx reset78E94. */
 float angle; /* +8:76BA8..BB2 and common optionscr proof. */
 tUVMatrix mat; /* +C:788AE..B6; all48bytes initialized, matrix/pos accessed. */
 unsigned char alpha; /* +3C:76BC8/76C88. */
 char structpad[3]; /* stride40:78E96/78ED0; rowA00:7888C..896. */
} tCarScreenMenuButton;
typedef struct tCarScreenComponent {
 signed short idx,level; /* +0/+2:76A94/76A96. */
 tUVMatrix mat; /* +4, size30; record stride34:76A98/76AB8. */
} tCarScreenComponent;
typedef struct tCarScreenProfileLink {
 tPlayerProfile **next,**prev; /* next+0:771B2/B4; handle list prefix only. */
} tCarScreenProfileLink;
typedef struct tCarScreenGhost tCarScreenGhost;
struct tCarScreenGhost {
 tCarScreenGhost **next,**prev; /* A108/+4 A154. */
 tPS_File **fileh; /* +8:A184. */
 signed char track; /* +C:A158. */
 unsigned char backward; /* +D:A162/A166 extu.b. */
 char unknown0e[0x28-0x0e];
 void **datah; /* +28:A196. Last known field, no full allocation stride claim. */
};
typedef struct tCarScreenGridLOD {
 char unknown00[8];
 void *displayList; /* +8:786A4/786B2/786BA, DC differs from NGC layout. */
} tCarScreenGridLOD;
typedef char checkCarScreenButton[(sizeof(tCarScreenMenuButton)==0x40)?1:-1];
typedef char checkCarScreenComponent[(sizeof(tCarScreenComponent)==0x34)?1:-1];
extern tCarScreenComponent carscrComponents8C0CC3D4[][11]; /* row23C:76A72 */
extern int carscrPartFrame8C0CC380;
extern tPadState carscrPlayers8C006000[4]; /* stride50/save+4C:7717C..18E. */
extern tPlayerProfile *carscrJustPlayers8C15044C[4];
extern tDLList carscrPlayerFiles8C15045C,carscrGhostFiles8C150480;
extern int carscrButtonsInited8C19F604[4];
extern signed short gNumPlayers;
extern int gRace;
extern signed char gEnableTireScaling;
extern signed short carscrOption7Available8C0BEEB6; /* 77D20/22; public identity unknown. */
extern int carscrNumOptions8C19FAB4[4],carscrOptions8C19FAC4[4][17];
extern float carscrStats8C19FBD4[4][4];
extern signed char carscrSelectionRows8C19FA3C[4][13]; /* 77D8E,77DA4 and byteDBE; element0 selected car view, other12unknown. */
extern signed char carscrTrans8C0BBA04[][13],carscrHandling8C0BBBC8[][13],carscrEngines8C0BBC09[][13],carscrTires8C0BBD24[][13],carscrFrame8C0BBDB8[][13]; /* +13 row offset77DC2/DD8/DF2/E0C/E22; ends unknown. */
extern float carscrTransStats8C0CC680[][4],carscrHandlingStats8C0CC700[][4],carscrEngineStats8C0CC730[][4],carscrTireStats8C0CC7C0[][4],carscrFrameStats8C0CC810[][4]; /* row16/float width:77DC4..DCA etc. */
extern tCarScreenGridLOD carscrGridLOD8C19FA14;
extern int carscrGridCarIdx8C0CC65C;
extern tCarScreenGhost **carscrLimboGhosts8C19FA7C;
extern tCarScreenGhost **carscrGhosts8C15046C[4];
extern signed char gTrackNum,gBackwards;
void ZOID_RemoveObject(signed short);
void DeallocateMemory(void *);
int PS_PakChanged(int); /* unsigned pak byte promoted to R4 with extu.b at771CC. */
void carscrAnimButtonText8C077230(int);
void carscrShowGridMap8C0786E8(int);
int carscrReadGhostData8C04DED2(tCarScreenGhost **);
int validOption(int,int);
#endif
