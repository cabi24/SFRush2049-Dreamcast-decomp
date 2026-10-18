#include "dc_types.h"
#ifndef DC_SMOKE_TYPES_H
#define DC_SMOKE_TYPES_H
typedef tModelData tSmokeModel;
typedef tCarData tSmokeCar;
typedef tZoidObjectDC tSmokeObj;
typedef struct tSmoke {
    tStatLLE ll; /* next+0: UpdateSmoke8C0507D2 */
    tUVMatrix mat; /* +4: 8C04F73A..42; pos+28: 8C04F7FE..81E */
    int mbIdx; /* +34: 8C04F8B6/F9EA; native MBOX result signedshort extended */
    char pad_unproven38[4]; /* unused +38 field identity unproven. */
    float lifeTime; /* +3c: 8C050BBA..BC8 */
    float swTime; /* +40: 8C04F6F2/F6F6; UpdateSmoke8C0509B0 */
    float vel[3]; /* +44/48/4c: 8C04F81A..838 */
    unsigned int data; /* +50: 8C04F6F8..710 */
    signed short slot; /* +54: 8C04F704..706 */
    signed short tIdx; /* +56: 8C04F8A4..A6/F9D6..DA */
} tSmoke;



typedef struct tSmokeReckon {
    char pad0[0x75c - 0x734];
    float suscomp[4]; /* model+75c pool8C0503A0 */
} tSmokeReckon;




typedef char dc_check_smoke[(sizeof(tSmoke)==0x58)?1:-1];
typedef char dc_check_smokecar[(sizeof(tSmokeCar)==0x3cc)?1:-1];
typedef char dc_check_smokemodel[(sizeof(tSmokeModel)==0x808)?1:-1];
typedef char dc_check_smokeobj[(sizeof(tSmokeObj)==0x58)?1:-1];
#endif
