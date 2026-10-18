#include "dc_types.h"
/* Private C89 powerups overlays, with DC field proof sites. */
#ifndef POWERUPS_TYPES_H
#define POWERUPS_TYPES_H
 /* InitBattlePowerUps 8C095CF6 */
typedef struct tTarget {
    tStatLLE ll;
    unsigned char Flags; /* +4: 8C095CCE, 8C095F8E..92 */
    char pad0[0xc-5];
    int dlIdx; /* +C: 8C095FAA, 8C095FBA */
    char pad1[0x50-0x10];
    signed short type; /* +50: 8C095F94..9E */
} tTarget;

typedef struct tBattlePowerUpT {
    tTarget *t; /* init8C095CE8, update8C095EBC */
    signed short last; /* +4: 8C095CEE, 8C095EEA */
    signed short type; /* +6: 8C095CF4, 8C095F5E */
} tBattlePowerUpT; /* stride8: 8C095CD8/CE0, 8C095EB8/BA */
typedef struct tBattlePowerUps {
    signed char num; /* 8C095C96, 8C095CA0 */
    signed char hIdx; /* +1: 8C095EB4, 8C095F70 */
    signed char sIdx; /* +2: 8C096010 */
    signed char iIdx; /* +3: 8C096176 */
    float healthTLeft; /* +4: 8C095D08/0C, 8C095EAA/AC */
    float invsTLeft; /* +8: 8C095D1C/1E */
    float shieldTLeft; /* +C: 8C095D26/28 */
    tBattlePowerUpT *tS; /* +10: 8C095CBE, 8C095CE2 */
} tBattlePowerUps;


 /* stride3CC from targetpool8C095DA6 and increment8C095E70 */
typedef char check_powerup[(sizeof(tBattlePowerUpT)==8)?1:-1];
typedef char check_powerups[(sizeof(tBattlePowerUps)==0x14)?1:-1];
typedef char check_car[(sizeof(tCarData)==0x3cc)?1:-1];
extern tStatList gTargetList;
extern tCarData gGameCar[6];
extern signed short gNumPlayers;
extern unsigned short gObjList[701];
extern volatile float gFrameSeconds;
void *AllocateMemoryHeap(void *heap,unsigned int size);
float Random(float max);
void ZOID_SetObjectDef(int objnum,unsigned short def);
void MBOX_ShowObject(int dlidx,int mode,int camlink);
#endif
