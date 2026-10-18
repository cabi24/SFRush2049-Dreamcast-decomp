/* Private collision overlays, C89. Target instruction/pool proof sites below. */
#ifndef COLLISION_TYPES_H
#define COLLISION_TYPES_H
typedef void streetyp;
typedef struct tReckon {
    char pad0[0x54];
    float rwv[3]; /* model788/78C/790:8C01DE54..58 */
    float rwr[3]; /* model794/798/79C:8C01D684..688 */
    float uvs[3][3]; /* model7A0:8C01D68E, pass8C01D612..61C */
} tReckon;
typedef struct tResurrect {
    char pad0[0x64];
    signed short moving_state; /* model6C4:8C01DC42, read8C01DB4C */
    char pad1[0x70-0x66];
} tResurrect;
typedef struct tModelData {
    char pad0[0xc4];
    float bodyForce[4][3]; /* 8C01D574, init loop strideC at8C01D4E8 */
    float bodyR[4][3]; /* 8C01D68C, size30 and strideC at8C01D604..6A6 */
    float centerForce[3]; /* 8C01D56E, 8C01D910; vector at8C01D842..8AE */
    char pad1[0x148-0x130];
    float peak_body_force[2][3]; /* 8C01D572, second row +C at8C01D454 */
    float peak_center_force[2][3]; /* 8C01D570, second row +C at8C01D442 */
    char pad2[0x220-0x178];
    float rwv[3]; /* 8C01DAE4, half multiply8C01DA00..26 */
    char pad3[0x3a0-0x22c];
    streetyp *lasttp[4]; /* 8C01D576; loop end+10/step4 at8C01D4AC..E6 */
    char pad4[0x3f0-0x3b0];
    float magvel; /* 8C01DC52 */
    char pad5[0x5c4-0x3f4];
    float collisionmass; /* 8C01DF98/8C01E414 */
    char pad6[0x63c-0x5c8];
    float crashthreshold; /* 8C01E09E */
    signed char crashflag; /* 8C01D912, 8C01E0A0 */
    char pad7[0x654-0x641];
    float colrad; /* 8C01D68A */
    char pad8[0x660-0x658];
    tResurrect resurrect;
    char pad9[0x734-0x6d0];
    tReckon reckon;
    char pad10[0x7c6-0x7c4];
    signed short slot; /* 8C01DADC */
    signed short in_game; /* 8C01D680, mov.w8C01D592 */
    char pad11[0x7cc-0x7ca];
    signed char drone_type; /* 8C01DF9E */
    char pad12[0x7ea-0x7cd];
    signed char collidable; /* 8C01D578/8C01D682 */
    char pad13[0x808-0x7eb]; /* target stride8C01D57A/7C2 */
} tModelData;
typedef struct tWeapon {
    signed char type; /* car39C:8C01DC40 */
    signed char ammoCnt; /* +1: decrement follows reload at8C01DBEC..DC06 */
} tWeapon;
typedef struct tBattle {
    tWeapon weapon;
    char pad0[0x24-2];
    float reloadingTime; /* car3C0:8C01DC44 */
} tBattle;
typedef struct tCarData {
    char pad0[0x370];
    signed char crash_state; /* 8C01DC46 */
    signed char dead; /* 8C01E0A2 */
    char pad1[1];
    signed char slot; /* 8C01DC3E */
    char pad2[0x377-0x374];
    signed char carMine; /* 8C01DAE0 */
    char pad3[0x39c-0x378];
    tBattle battle;
    char pad4[0x3cc-0x3c4]; /* stride8C01DADE */
} tCarData;
typedef struct tBBoxColl {
    float left; /* 8C01E146..14A */
    float right; /* +4:8C01E136..142 */
    float front; /* +8:8C01E118..124 */
    float back; /* +C:8C01E128..12C */
    char pad0[0x1c-0x10];
    float weight; /* +1C:8C01E326..33C */
} tBBoxColl;
typedef struct tMiniModelData {
    tBBoxColl *bboxInfo; /* 8C01E11E */
    float uvs[3][3]; /* +4:8C01E110 */
    float pos[3]; /* +28/2C/30:8C01E0E8..102 */
    float vel[3]; /* +34/38/3C:8C01E170..194 */
} tMiniModelData;
typedef char check_model[(sizeof(tModelData)==0x808)?1:-1];
typedef char check_car[(sizeof(tCarData)==0x3cc)?1:-1];
typedef char check_reckon[(sizeof(tReckon)==0x90)?1:-1];
typedef char check_mini[(sizeof(tMiniModelData)==0x40)?1:-1];
extern tModelData model[6];
extern tCarData gGameCar[6];
extern streetyp *stree;
extern int gRace;
extern signed char gCarTeam[4],gDemolitionBattle;
void BodyVector(float *v,float *out,float uvs[3][3]);
void WorldVector(float *v,float *out,float uvs[3][3]);
int PointInBody(tModelData *m,float *pt,float top);
void setFBCollisionForce(tModelData *m,tModelData *m1,tModelData *m2,float *dir,float *pos,float top);
void ForceApart(tModelData *m,tModelData *m1,tModelData *m2,float *dir);
void MiniForceApart(tModelData *m,float *dir);
void BattleDamage(tCarData *car,tCarData *hit,int damage);
void BattleDamage2(tCarData *car,tCarData *hit,int damage,int damage2);
float magnitude(float *v);
float _builtin_sqrtf(float v);
float _builtin_fabsf(float v);
#endif
