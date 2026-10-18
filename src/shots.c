/* Target-backed Dreamcast shot damage/lifecycle packet; renderer update remainsexternal. */
#include "shots_types.h"
extern char shotsName8C0B35A4[];
extern char shotsName8C0B35B4[];
extern char shotsName8C0B35C4[];
extern char shotsName8C0B35D0[];
extern char shotsName8C0B35DC[];
extern char shotsName8C0B35EC[];
extern char shotsName8C0B35F8[];
extern char shotsName8C0B3608[];
extern char shotsName8C0B3614[];
extern char shotsName8C0B3620[];
extern char shotsName8C0B362C[];
extern char shotsName8C0B3638[];
extern char shotsName8C0B3648[];
extern char shotsName8C0B3658[];
extern char shotsName8C0B3668[];
extern char shotsName8C0B3674[];
extern char shotsName8C0B3684[];
extern char shotsName8C0B3694[];
extern char shotsName8C0B36A0[];
extern char shotsName8C0B36AC[];
extern char shotsName8C0B36B8[];
extern char shotsName8C0B36C4[];
extern char shotsName8C0B36D0[];
extern char shotsName8C0B36E0[];
extern char shotsName8C0B36F0[];
extern char shotsName8C0B3700[];
extern char shotsName8C0B3710[];
const float CaseOffsets[1][3] = {{0.6000000387430191f,0.64999997615814209f,-0.68999999761581421f}};
const float ShotOffsets[10][3] = {{0.16000000014901161f,0.49000001698732376f,0.56999999284744263f},{0.0099999997764825821f,0.64999997615814209f,1.5900000333786011f},{0.0f,0.89999997615814209f,1.1000000536441803f},{0.0f,-0.30000001192092896f,-0.70000000298023224f},{0.0099999997764825821f,2.0400000214576721f,0.2800000011920929f},{0.0f,0.0f,0.0f},{-0.040000000037252903f,0.61000001430511475f,0.23000000789761543f},{0.0f,1.25f,0.0f},{0.0f,1.5f,2.0f},{0.0f,0.0f,0.0f}};
const char *const WprObjNames[10] = {shotsName8C0B35A4,shotsName8C0B35B4,shotsName8C0B35C4,shotsName8C0B35D0,shotsName8C0B35DC,shotsName8C0B35EC,shotsName8C0B35F8,shotsName8C0B3608,shotsName8C0B3614,shotsName8C0B3620};
const char *const WfxObjNames[7] = {shotsName8C0B362C,shotsName8C0B3638,shotsName8C0B3648,shotsName8C0B3658,shotsName8C0B3668,shotsName8C0B3674,shotsName8C0B3684};
const char *const MineTexNames[5] = {shotsName8C0B3694,shotsName8C0B36A0,shotsName8C0B36AC,shotsName8C0B36B8,shotsName8C0B36C4};
const char *const RipTexNames[5] = {shotsName8C0B36D0,shotsName8C0B36E0,shotsName8C0B36F0,shotsName8C0B3700,shotsName8C0B3710};
int sWeaponFx[10] = {28,25,24,26,27,29,30,31,20,-1};
void AnimateCase(void)
{
    tShotCaseDC *casing;
    tShotCaseDC *next;

    for (casing = gShotCaseList.head; casing; casing = next) {
        next = (tShotCaseDC *)casing->next;
        casing->time -= gFrameSeconds;
        if (casing->time <= 0.0f) {
            ZOID_RemoveObject(casing->mbIdx);
            FreeStatLLE(&gShotCaseList, casing);
        } else {
            casing->vel[1] -= 2.5f * (gGravity * gFrameSeconds);
            casing->mat.mat3.pos[0] += casing->vel[0] * gFrameSeconds;
            casing->mat.mat3.pos[1] += casing->vel[1] * gFrameSeconds;
            casing->mat.mat3.pos[2] += casing->vel[2] * gFrameSeconds;
        }
    }
}

void BattleDamage(tCarData *car, tCarData *hit, int damage)
{
    if (!model[hit->slot].crashflag && hit->slot != car->slot &&
        gCarTeam[hit->slot] != gCarTeam[car->slot]) {
        if (hit->battle.defense.active & 4)
            damage *= 0.20000000298023224f;
        hit->battle.health -= damage;
        SHOT_DAMAGE(hit) = damage;
        if (hit->battle.health <= 0) {
            hit->battle.health = 0;
            AddBattlePoint(car->slot, hit->slot, 1);
            model[hit->slot].crashflag = 1;
        }
    }
}

void BattleDamage2(tCarData *car1, tCarData *car2, int damage1, int damage2)
{
    if (!model[car1->slot].crashflag && !model[car2->slot].crashflag &&
        gCarTeam[car1->slot] != gCarTeam[car2->slot]) {
        if (car2->battle.defense.active & 4)
            damage1 *= 0.20000000298023224f;
        if (car1->battle.defense.active & 4)
            damage2 *= 0.20000000298023224f;
        car1->battle.health -= damage2;
        SHOT_DAMAGE(car1) = damage2;
        if (car1->battle.health <= 0) {
            car1->battle.health = 0;
            AddBattlePoint(car2->slot, car1->slot, 1);
            model[car1->slot].crashflag = 1;
        }
        car2->battle.health -= damage1;
        SHOT_DAMAGE(car2) = damage1;
        if (car2->battle.health <= 0) {
            car2->battle.health = 0;
            AddBattlePoint(car1->slot, car2->slot, 1);
            model[car2->slot].crashflag = 1;
        }
    }
}

void ProximityDamage(float *pos, float *old, int owner, float radiussq, int damage)
{
    tCarData *hit;
    float pos2[3];
    float pos3[3];
    float distsq;
    float y;

    for (hit = gGameCar; hit < &gGameCar[gNumCars]; hit++) {
        if (model[hit->slot].crashflag || !hit->collidable || hit->dead)
            continue;
        {
            float delta[3];
            delta[0] = pos[0] - hit->dr_pos[0];
            delta[1] = pos[1] - hit->dr_pos[1];
            delta[2] = pos[2] - hit->dr_pos[2];
            distsq = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
        }
        if (distsq < radiussq) {
            y = (radiussq - distsq) / radiussq;
            if (hit->battle.weapon.type == 5) {
                pos2[0] = old[0] - pos[0];
                pos2[1] = old[1] - pos[1];
                pos2[2] = old[2] - pos[2];
                BodyVector(pos2, pos3, hit->dr_uvs);
                if (fabs(fatan2(pos3[0], pos3[2])) < 1.35f)
                    BattleDamage(&gGameCar[owner], hit, 0.35f * (damage * (y * y)));
                else
                    BattleDamage(&gGameCar[owner], hit, damage * (y * y));
            } else {
                BattleDamage(&gGameCar[owner], hit, damage * (y * y));
            }
        }
    }
}

void ShotInit(void)
{
    int i;

    wallTidx = 0;
    InitStatLL(&gShotList, gShots, sizeof(gShots[0]), 24, 0);
    InitStatLL(&gHitPolyList, gHitPolys, sizeof(gHitPolys[0]), 24, 0);
    InitStatLL(&gShotObjList, gShotObjs, sizeof(gShotObjs[0]), 24, 0);
    InitStatLL(&gShotCaseList, gShotCase, sizeof(gShotCase[0]), 30, 0);
    for (i = 0; i < 10; i++)
        gWprObjList[i] = MBOX_FindObject_WORK(WprObjNames[i], 0, gNumModels - 1, 0);
    for (i = 0; i < 7; i++)
        gWfxObjList[i] = MBOX_FindObject_WORK(WfxObjNames[i], 0, gNumModels - 1, 0);
    for (i = 0; i < 5; i++)
        MBOX_FindTexture_WORK(MineTexNames[i], &gMineTextures[i], 0, gNumModels - 1, 1);
    for (i = 0; i < 5; i++)
        MBOX_FindTexture_WORK(RipTexNames[i], &gRipTextures[i], 0, gNumModels - 1, 1);
    for (i = 0; i < 4; i++)
        gCarShield[i].dlIdx = -1;
    InitStreaks();
}

void ShotDone(tShotInfoDC *shot)
{
    tShotDC *o;
    switch (shot->type) {
    case 3:
        o = shot->obj2;
        ZOID_RemoveObject(o->mbIdx);
        FreeStatLLE(&gShotObjList, o);
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
    case 7:
    case 8:
        o = shot->obj;
        ZOID_RemoveObject(o->mbIdx);
        FreeStatLLE(&gShotObjList, o);
        break;
    }
}

tShotDC *_CreateShotObject(int idx, int parent, unsigned int flags, int o2)
{
    tShotDC *o;
    tUVMatrix *mat = 0; /* DC97332 passesNULLwheno2; backenddefaultmatrix. */
    o = GetStatLLE(&gShotObjList);
    if (o2)
        o->mbIdx = MBOX_NewObject(gWprObjList[idx], mat, parent, flags);
    else
        o->mbIdx = MBOX_NewObject(gWprObjList[idx], &o->mat, parent, flags);
    return o;
}

void _UpdateShotObject(tShotInfoDC *shot, int unt)
{
    tShotDC *o = shot->obj;
    o->mat.mat3.pos[0] = shot->pos[0];
    o->mat.mat3.pos[1] = shot->pos[1];
    o->mat.mat3.pos[2] = shot->pos[2];
    if (unt == 1)
        CopyUV(unitmatrix, o->mat.mat3.uvs);
    else if (unt != 2)
        CopyUV(shot->uvs, o->mat.mat3.uvs);

    if (shot->flags & 0x70) {
        if ((shot->flags & 0x80) && (shot->flags & 0x10) &&
            (shot->type == 1 || shot->type == 8)) {
reduceScale:
            o->mat.mat3.uvs[2][0] *= 0.20000000298023224f;
            o->mat.mat3.uvs[2][1] *= 0.20000000298023224f;
            o->mat.mat3.uvs[2][2] *= 0.20000000298023224f;
        } else if (shot->type == 0) {
            if (shot->flags & 0x80) {
                goto reduceScale;
            } else {
                o->mat.mat3.uvs[2][0] *= 0.4f;
                o->mat.mat3.uvs[2][1] *= 0.4f;
                o->mat.mat3.uvs[2][2] *= 0.4f;
            }
        }
    }
    if (shot->flags & 2)
        PitchUV(0.18f, o->mat.mat3.uvs);
}

void _InitNewShot(tShotInfoDC *shot, tCarData *car)
{
    shot->slotDamaged = 0;
    shot->obj = 0;
    shot->obj2 = 0;
    shot->flags = 0x10;
    shot->type = car->battle.weapon.type;
    shot->slot = car->slot;
    shot->incTime = 0.0f;
    shot->vel[0] = 0.0f;
    shot->vel[1] = 0.0f;
    shot->vel[2] = 0.0f;
    shot->pos[0] = car->dr_pos[0];
    shot->pos[1] = car->dr_pos[1];
    shot->pos[2] = car->dr_pos[2];
    shot->old[0] = car->dr_pos[0];
    shot->old[1] = car->dr_pos[1];
    shot->old[2] = car->dr_pos[2];
    CopyUV(car->dr_uvs, shot->uvs);
}
