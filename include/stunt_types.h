#include "dc_types.h"
/* Private C89 DC overlays. Unproved fields remain padding. */
#ifndef STUNT_TYPES_H
#define STUNT_TYPES_H
typedef struct tStuntPlayer {
    signed short show, crash; /* +0/+2: 8C054DAC,8C054D46 */
    float time; /* +4: 8C054DB2 */
    int maxStuntPoints, points, pointsHud, totalPoints;
    int numstunts, lastStuntType; /* +18/+1C: 8C054D4C,8C054DE0 */
    signed short stunts[10], stuntsHud[10], totalStunts[10];
    char pad0[4]; /* +5C..60 unproved */
    float wingTime; /* +60: 8C055BAA..BB4 */
    signed char wingBonus, wingBonusHud, doneBonus, doneBonusHud;
    int bonus, bonusHud, showBonus, done;
} tStuntPlayer; /* stride78: 8C054D36 and 8C055BA4 */
typedef struct tStuntTimes { float startTime, endTime; } tStuntTimes;
typedef struct tStuntInfo {
    unsigned int active; /* +0: 8C054D26 */
    float stuntW[4][3]; /* +4/+10/+1C/+28: 8C0551F0,5500,5366,56E0 */
    tStuntTimes time[4]; /* +34..54: _CheckAirTime/_Check2Wheel* */
    float endTime; /* +54: CheckForStunt */
    char pad0[0x7c-0x58];
} tStuntInfo; /* stride7C: 8C054D08..20 and 8C054D58 */

 /* stride808: 8C055B74..84 */
 /* stride3CC: pools8C0553CC/5BD4 */
typedef char stunt_player_size[(sizeof(tStuntPlayer)==0x78)?1:-1];
typedef char stunt_info_size[(sizeof(tStuntInfo)==0x7c)?1:-1];
typedef char stunt_model_size[(sizeof(tModelData)==0x808)?1:-1];
typedef char stunt_car_size[(sizeof(tCarData)==0x3cc)?1:-1];
extern tStuntPlayer gStuntPlayer[4];
extern tModelData model[6];
extern tCarData gGameCar[6];
extern int gStuntsDone;
extern signed short gNumPlayers;
extern unsigned char gCoastFlag;
extern float gModelTime;
extern volatile float gFrameSeconds;
void *memset(void *dst,int value,unsigned int size);
float _builtin_fabsf(float value);
#endif
