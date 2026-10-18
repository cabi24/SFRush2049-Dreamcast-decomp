#ifndef DC_WEAPONS_TYPES_H
#define DC_WEAPONS_TYPES_H
#include "dc_types.h"

typedef struct tWeaponType { signed char type,ammoCnt; } tWeaponType;
typedef struct tWeaponDefense { unsigned int active; char pad0[12]; } tWeaponDefense;
typedef struct tWeaponBattle {
    tWeaponType weapon; /* car+39c/39d: 8C0993C0..C6,9712..71C */
    char pad0[8 - 2];
    tWeaponDefense defense; /* car+3a4: pool8C0994D6, mov.l8C0993B2 */
    signed char shoot; /* +3b4: 8C0993B8..BA; AnimateWeapon signed cmp5 */
    unsigned char fadeAlpha; /* +3b5 pool8C09980A, extu.b8C09978C */
    signed char fadeState; /* +3b6:8C0993B4..B6 */
    char pad1[1];
    int loaded; /* +3b8:8C0993BC..BE */
    char pad2[0x30 - 0x20];
} tWeaponBattle;

typedef struct tWeaponCar {
    char pad0[0x110];
    tVisual visuals[22]; /* +110, visual21+1f8, pools8C099730/732 */
    char pad1[0x364 - 0x320];
    tRGBA IRclr; /* pool8C099806; word load8C099772 */
    char pad2[0x374 - 0x368];
    signed char camera,view; /* camera/view+374/375: AnimateWeapon signed compares */
    char pad3[0x39c - 0x376];
    tWeaponBattle battle;
} tWeaponCar; /* stride3cc: pools8C0994D4,972E */

typedef struct tWeaponModel {
    char pad0[8];
    unsigned char body_type; /* literal8C14CBBC=model+8, 8C099674/67A */
    char pad1[0x640 - 9];
    signed char crashflag; /* literal8C14D1F4=model+640, 8C0996EC */
    char pad2[0x808 - 0x641]; /* slot*808 at8C0996E2..EA */
} tWeaponModel;

typedef struct tWeaponParts {
    int frame; /* base8C15D994 and dereference8C09965A/7C4 */
    char pad0[0x38 - 4];
    int weap; /* base8C15D9CC=+38, 8C099662/7AE */
    char pad1[0x60 - 0x3c]; /* slot*96 at8C09962E..640 */
} tWeaponParts;

typedef struct tWeaponMat { tUVMatrix mat; } tWeaponMat;
typedef struct tMuzzleObj {
    int mdls[5]; /* loop8C099432..4C0; matrix base14 */
    tWeaponMat mats[5]; /* +14 and stride30: 8C099B1E/9B22 */
    int on; /* +104 */
    float onTime; /* +108: pool8C0994DE, store8C0993F8 */
} tMuzzleObj; /* stride10c: pool8C0994DC */
typedef struct tCMuzzleObj {
    int mdls[5];
    tWeaponMat mats[5];
    int ripdl; /* +104: pool8C0994DA/95EA */
    tUVMatrix mat; /* +108..138, five*30 matrix array and target passed address */
    signed char ripCnt; /* +138:8C0993DC..E0 */
    signed char ripTIdx; /* +139:8C0995D0..D2 */
    signed short on; /* +13a: AnimateWeapon word accesses */
    float ripScale; /* +13c: AnimateWeapon ScaleUV */
    float ripTime; /* +140: AnimateWeapon timer */
    float onTime; /* +144:8C0993E2..EA */
} tCMuzzleObj; /* stride148:pool8C0994D8/95E8 */

typedef char dc_check_weaponcar[(sizeof(tWeaponCar)==0x3cc)?1:-1];
typedef char dc_check_weaponmodel[(sizeof(tWeaponModel)==0x808)?1:-1];
typedef char dc_check_weaponparts[(sizeof(tWeaponParts)==0x60)?1:-1];
typedef char dc_check_gatt[(sizeof(tMuzzleObj)==0x10c)?1:-1];
typedef char dc_check_cann[(sizeof(tCMuzzleObj)==0x148)?1:-1];
#endif
