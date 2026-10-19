#include "weapons_types.h"
extern tWeaponCar gGameCar[];
extern tWeaponModel model[];
extern tWeaponParts gCarParts[];
extern unsigned short gObjList[];
extern unsigned int gWfxObjList[];
extern unsigned short gRipTextures[];
extern tTexture *gTexDefPtrs[];
extern volatile float gSeconds,gFrameSeconds;
extern void ZOID_RemoveObject(int);
extern void MBOX_HideObject(int,int,int);
extern void MBOX_ShowObject(int,int,int);
extern signed short MBOX_NewObject(int,tUVMatrix *,signed short,unsigned int);
extern int InitCarObj(int,int,int,unsigned int,signed short,int,int);
extern void ZOID_UpdateObject(int,float *,float *);
extern void ZOID_SetObjectDef(int,unsigned short);
extern void ZOID_SetColor(int,tRGBA *);
extern tUVMatrix *ZOID_FindMatrix(int);
extern void ZOID_SetTexture(int,tTexture *,int);
extern void CopyUV(float (*)[3],float (*)[3]);
extern void RollUV(float,float (*)[3]);
extern void ScaleUV(float (*)[3],float (*)[3],float);
extern float Random(float);
void _RemoveGattMFlash(signed short slot);
void _RemoveCannMFlash(signed short slot);
void AnimateWeapon(tVisual *v, signed short op);

float weaponOffsets[8][13][3] = {
    0.0f, 2.5000000298023224f, 0.30000001564621925f, 0.0f, 3.0000000298023224f, 0.30000001564621925f, 0.0f, 2.7999999821186066f, 0.30000001564621925f, 0.0f, 3.0000000298023224f, 0.10000000242143869f, 0.0f, 3.0000000298023224f, 0.10000000242143869f, 0.0f,
    3.400000125169754f, -0.40000000968575478f, 0.0f, 3.0999999344348907f, 0.30000001564621925f, 0.0f, 3.2999999821186066f, 1.0000000149011612f, 0.0f, 3.3900001347064972f, -1.8200000673532486f, 0.0f, 3.0499999821186066f, 0.64999998360872269f, 0.0f,
    5.0400000214576721f, 1.3300000578165054f, 0.0f, 2.9500000774860382f, -0.89999998360872269f, 0.0f, 2.7899999916553497f, 0.51999998837709427f, 0.0f, 2.7000000774860382f, 2.0999999344348907f, 0.0f, 3.0999999344348907f, 1.1000000387430191f, 0.0f,
    2.7000000774860382f, 0.10000000242143869f, 0.0f, 2.900000125169754f, -0.10000000242143869f, 0.0f, 2.900000125169754f, 0.40000000968575478f, 0.0f, 3.5000000298023224f, 0.10000000242143869f, 0.0f, 3.0000000298023224f, 0.30000001564621925f, 0.0f,
    3.2999999821186066f, 1.200000062584877f, 0.0f, 3.3499999344348907f, -0.69000000506639481f, 0.0f, 3.0399999916553497f, 0.97000003606081009f, 0.0f, 5.0500002503395081f, 2.0200000107288361f, 0.0f, 2.9800000488758087f, -0.31000000610947609f,
    0.0f, 2.7899999916553497f, 0.92000002413988113f, 0.0f, 2.5000000298023224f, 1.2999999672174454f, 0.0f, 3.0000000298023224f, -0.5000000074505806f, 0.0f, 2.5999999344348907f, -0.60000003129243851f, 0.0f, 2.7000000774860382f, -1.1000000387430191f,
    0.0f, 2.7000000774860382f, -1.1000000387430191f, 0.0f, 3.2999999821186066f, -1.6000000387430191f, 0.0f, 2.900000125169754f, -0.60000003129243851f, 0.0f, 3.0000000298023224f, 0.10000000242143869f, 0.0f, 3.1300001442432404f, -1.7300000339746475f,
    0.0f, 2.8900001347064972f, -0.41000000014901161f, 0.0f, 4.8400002121925354f, 0.37000000849366188f, 0.0f, 2.7500000298023224f, -1.669999971985817f, 0.0f, 2.6600001156330109f, -0.34999999776482582f, 0.0f, 1.8999999910593033f,
    -5.3000002503395081f, 0.0f, 1.700000062584877f, -5.6999998688697815f, 0.0f, 1.8999999910593033f, -5.5999999642372131f, 0.0f, 2.0000000298023224f, -5.9000001549720764f, 0.0f, 1.3999999910593033f, -6.0999999642372131f, 0.0f, 1.3999999910593033f,
    -7.0000000596046448f, 0.0f, 1.8999999910593033f, -5.6999998688697815f, 0.0f, 1.7999999672174454f, -6.0000000596046448f, 0.0f, 1.2999999672174454f, -5.7100000977516174f, 0.0f, 1.6000000387430191f, -6.7300000786781311f, 0.0f,
    1.7100000530481339f, -9.279999852180481f, 0.0f, 1.7400000244379044f, -6.9499998688697815f, 0.0f, 1.2799999862909317f, -5.5400000214576721f, 0.0f, 2.2000000774860382f, 0.60000003129243851f, 0.0f, 2.5999999344348907f, -0.69999999552965164f,
    0.0f, 2.400000125169754f, -0.80000001937150955f, 0.0f, 2.400000125169754f, -1.1000000387430191f, 0.0f, 2.5999999344348907f, -0.5000000074505806f, 0.0f, 3.0000000298023224f, -1.700000062584877f, 0.0f, 2.5999999344348907f, -0.5000000074505806f,
    0.0f, 2.7000000774860382f, -0.5000000074505806f, 0.0f, 3.1600001156330109f, -2.5200000107288361f, 0.0f, 2.6700001060962677f, -0.38999998942017555f, 0.0f, 4.8000002503395081f, 0.44000000134110451f, 0.0f, 2.5200000107288361f,
    -2.150000125169754f, 0.0f, 2.3699999153614044f, -0.44000000134110451f, 0.0f, 0.0f, 6.4000001549720764f, 0.0f, 0.0f, 6.6999998688697815f, 0.0f, 0.0f, 6.0000000596046448f, 0.0f, 0.0f,
    6.0000000596046448f, 0.0f, 0.20000000484287739f, 6.4000001549720764f, 0.0f, 0.20000000484287739f, 6.8000002503395081f, 0.0f, 0.10000000242143869f, 5.9000001549720764f, 0.0f, 0.20000000484287739f, 5.8000002503395081f, 0.0f, 0.2500000037252903f,
    4.8600001931190491f, 0.0f, 0.44000000134110451f, 7.3800001740455627f, 0.0f, -0.18000000901520252f, 8.0699998140335083f, 0.0f, 0.1400000024586916f, 5.8899999260902405f, 0.0f, 0.1400000024586916f, 5.8899999260902405f, 0.0f,
    2.900000125169754f, 0.30000001564621925f, 0.0f, 3.2999999821186066f, -0.69999999552965164f, 0.0f, 3.0999999344348907f, -0.89999998360872269f, 0.0f, 3.2000000774860382f, -0.89999998360872269f, 0.0f, 3.2000000774860382f, -0.89999998360872269f, 0.0f,
    3.5999999344348907f, -1.3999999910593033f, 0.0f, 3.2999999821186066f, -0.69999999552965164f, 0.0f, 3.400000125169754f, -0.30000001564621925f, 0.0f, 3.650000125169754f, -2.0599999725818634f, 0.0f, 3.1700001060962677f, -0.44000000134110451f,
    0.0f, 5.2800002694129944f, 0.6600000336766243f, 0.0f, 3.150000125169754f, -1.5199999958276749f, 0.0f, 3.0799999535083771f, -0.60000003129243851f, 0.0f, 2.5999999344348907f, 0.40000000968575478f, 0.0f, 3.0000000298023224f,
    -0.80000001937150955f, 0.0f, 2.7000000774860382f, -0.80000001937150955f, 0.0f, 3.0999999344348907f, -0.40000000968575478f, 0.0f, 3.0999999344348907f, -0.20000000484287739f, 0.0f, 3.400000125169754f, -1.7999999672174454f, 0.0f, 3.0999999344348907f,
    -0.89999998360872269f, 0.0f, 3.0999999344348907f, -0.89999998360872269f, 0.0f, 3.5200000107288361f, -2.0000000298023224f, 0.0f, 3.400000125169754f, -0.33000001683831215f, 0.0f, 4.3800001740455627f, -4.3899999260902405f, 0.0f,
    3.2500000298023224f, -2.0499999821186066f, 0.0f, 3.1199999153614044f, -1.1100000292062759f
};
static signed char gCurWeaponType[4];
static tMuzzleObj gGattMuzzleObjs[4];
static tCMuzzleObj gCannMuzzleObjs[4];

/* 0x8005c600, size 0x90 */
void InitBattlePlayer(signed short slot)
{
    gGameCar[slot].battle.defense.active = 0;
    gGameCar[slot].battle.fadeState = 0;
    gGameCar[slot].battle.shoot = 0;
    gGameCar[slot].battle.loaded = 0;
    gGameCar[slot].battle.weapon.type = 8;
    gGameCar[slot].battle.weapon.ammoCnt = -1;
    gCurWeaponType[slot] = 9;
    gCannMuzzleObjs[slot].ripdl = -1;
    gCannMuzzleObjs[slot].ripCnt = -1;
    gCannMuzzleObjs[slot].onTime = 0.0f;
    gGattMuzzleObjs[slot].onTime = 0.0f;
    {
        int i;
        for (i = 0; i < 5; i++)
            gGattMuzzleObjs[slot].mdls[i] = -1;
    }
}

/* 0x8005c690, size 0x78 */
void _RemoveGattMFlash(signed short slot)
{
    int i; /* r29 */
    for (i = 0; i < 5; i++) {
        if (gGattMuzzleObjs[slot].mdls[i] != -1) {
            ZOID_RemoveObject(gGattMuzzleObjs[slot].mdls[i]);
            gGattMuzzleObjs[slot].mdls[i] = -1;
        }
    }
}

/* 0x8005c708, size 0xb8 */
void _RemoveCannMFlash(signed short slot)
{
    int i; /* r28 */
    for (i = 0; i < 5; i++) {
        if (gCannMuzzleObjs[slot].mdls[i] != -1) {
            ZOID_RemoveObject(gCannMuzzleObjs[slot].mdls[i]);
            gCannMuzzleObjs[slot].mdls[i] = -1;
        }
    }
    if (gCannMuzzleObjs[slot].ripdl != -1) {
        ZOID_RemoveObject(gCannMuzzleObjs[slot].ripdl);
        gCannMuzzleObjs[slot].ripdl = -1;
        gCannMuzzleObjs[slot].ripTIdx = -1;
    }
}

/* 0x8005c7c0, size 0xf4 */
void StartWeapon(signed short slot)
{
    tVisual *v; /* r31 */
    v = gGameCar[slot].visuals;
    v += 21;
    if (v->objnum < 0) {
        v->index = 0;
        v->func = AnimateWeapon;
        v->timeStamp = gSeconds;
        v->slot = slot;
        v->objnum = gCarParts[slot].weap = InitCarObj(gObjList[531], 15, gCarParts[slot].frame, 0x2000, slot, -1, 1);
        ZOID_UpdateObject(v->objnum, weaponOffsets[1][model[slot].body_type], 0);
    }
}

/* 0x8005c8b4, size 0x14a4 */
void AnimateWeapon(tVisual *v, signed short op)
{
    tWeaponCar *car = &gGameCar[v->slot];
    int def;
    int i;
    int r1;
    float r2;
    tRGBA clr;
    tRGBA clr2 = { 0, 255, 0, 255 };
    tUVMatrix *mat;
    float pos[3];

    if (!op || model[v->slot].crashflag) {
        if (gCurWeaponType[v->slot] == 1)
            _RemoveGattMFlash(v->slot);
        else if (gCurWeaponType[v->slot] == 0)
            _RemoveCannMFlash(v->slot);
        car->battle.weapon.type = 8;
        car->battle.weapon.ammoCnt = -1;
        MBOX_HideObject(v->objnum, 0, 15);
        return;
    }
    clr.rgba = car->IRclr.rgba;
    if (car->battle.defense.active & 1) {
        clr.c[3] = car->battle.fadeAlpha;
        if (clr.c[3] < 48)
            clr.c[3] = 48;
    }
    ZOID_SetColor(gCarParts[v->slot].weap, &clr);
    mat = ZOID_FindMatrix(gCarParts[v->slot].frame);
    if (car->battle.weapon.type == 8) {
        MBOX_HideObject(v->objnum, 1, 15);
    } else if (car->camera >= 0 && car->view <= 1) {
        MBOX_ShowObject(v->objnum, 0, 15);
        MBOX_HideObject(v->objnum, 1, 1 << car->camera);
    } else {
        MBOX_ShowObject(v->objnum, 0, 15);
    }
    if (gCurWeaponType[v->slot] != car->battle.weapon.type) {
        pos[0] = weaponOffsets[car->battle.weapon.type][model[v->slot].body_type][0];
        pos[1] = weaponOffsets[car->battle.weapon.type][model[v->slot].body_type][1];
        pos[2] = weaponOffsets[car->battle.weapon.type][model[v->slot].body_type][2];
        if (gCurWeaponType[v->slot] == 1)
            _RemoveGattMFlash(v->slot);
        else if (gCurWeaponType[v->slot] == 0)
            _RemoveCannMFlash(v->slot);
        gCurWeaponType[v->slot] = car->battle.weapon.type;
        switch (car->battle.weapon.type) {
        case 8:
            return;
        case 0:
            def = 530;
            pos[0] += 0.15999999828636646f;
            pos[1] += 0.47999999299645424f;
            pos[2] += 2.5699999630451202f;
            for (i = 0; i < 5; i++) {
                CopyUV(mat->mat3.uvs, gCannMuzzleObjs[v->slot].mats[i].mat.mat3.uvs);
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[0][0] * pos[0] + mat->mat3.pos[0];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[0][1] * pos[0] + mat->mat3.pos[1];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[0][2] * pos[0] + mat->mat3.pos[2];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[1][0] * pos[1] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[1][1] * pos[1] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[1][2] * pos[1] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[2][0] * pos[2] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[2][1] * pos[2] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[2][2] * pos[2] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2];
                gCannMuzzleObjs[v->slot].mdls[i] = MBOX_NewObject(gWfxObjList[i], &gCannMuzzleObjs[v->slot].mats[i].mat, -1, 0x42000);
                ZOID_SetColor(gCannMuzzleObjs[v->slot].mdls[i], &clr2);
                MBOX_HideObject(gCannMuzzleObjs[v->slot].mdls[i], 1, 15);
            }
            CopyUV(mat->mat3.uvs, gCannMuzzleObjs[v->slot].mat.mat3.uvs);
            gCannMuzzleObjs[v->slot].mat.mat3.pos[0] = mat->mat3.uvs[0][0] * pos[0] + mat->mat3.pos[0];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[1] = mat->mat3.uvs[0][1] * pos[0] + mat->mat3.pos[1];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[2] = mat->mat3.uvs[0][2] * pos[0] + mat->mat3.pos[2];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[0] = mat->mat3.uvs[1][0] * pos[1] + gCannMuzzleObjs[v->slot].mat.mat3.pos[0];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[1] = mat->mat3.uvs[1][1] * pos[1] + gCannMuzzleObjs[v->slot].mat.mat3.pos[1];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[2] = mat->mat3.uvs[1][2] * pos[1] + gCannMuzzleObjs[v->slot].mat.mat3.pos[2];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[0] = mat->mat3.uvs[2][0] * pos[2] + gCannMuzzleObjs[v->slot].mat.mat3.pos[0];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[1] = mat->mat3.uvs[2][1] * pos[2] + gCannMuzzleObjs[v->slot].mat.mat3.pos[1];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[2] = mat->mat3.uvs[2][2] * pos[2] + gCannMuzzleObjs[v->slot].mat.mat3.pos[2];
            gCannMuzzleObjs[v->slot].ripdl = MBOX_NewObject(gWfxObjList[6], &gCannMuzzleObjs[v->slot].mat, -1, 0x42000);
            ZOID_SetColor(gCannMuzzleObjs[v->slot].ripdl, &clr2);
            MBOX_HideObject(gCannMuzzleObjs[v->slot].ripdl, 1, 15);
            gCannMuzzleObjs[v->slot].ripCnt = -1;
            gCannMuzzleObjs[v->slot].ripTime = 0.0f;
            gCannMuzzleObjs[v->slot].ripScale = 1.5000000149011612f + Random(1.5000000149011612f);
            break;
        case 1:
            def = 531;
            pos[0] += 0.0099999998928979039f;
            pos[1] += 0.64999998360872269f;
            pos[2] += 3.5899999439716339f;
            for (i = 0; i < 5; i++) {
                CopyUV(mat->mat3.uvs, gGattMuzzleObjs[v->slot].mats[i].mat.mat3.uvs);
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[0][0] * pos[0] + mat->mat3.pos[0];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[0][1] * pos[0] + mat->mat3.pos[1];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[0][2] * pos[0] + mat->mat3.pos[2];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[1][0] * pos[1] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[1][1] * pos[1] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[1][2] * pos[1] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[2][0] * pos[2] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[2][1] * pos[2] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[2][2] * pos[2] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2];
                gGattMuzzleObjs[v->slot].mdls[i] = MBOX_NewObject(gWfxObjList[i], &gGattMuzzleObjs[v->slot].mats[i].mat, -1, 0);
                MBOX_HideObject(gGattMuzzleObjs[v->slot].mdls[i], 1, 15);
            }
            break;
        case 2: def = 532; break;
        case 3: def = 533; break;
        case 4: def = 534; break;
        case 5: def = 535; break;
        case 6: def = 536; break;
        case 7: def = 537; break;
        }
        ZOID_SetObjectDef(gCarParts[v->slot].weap, gObjList[def]);
        ZOID_UpdateObject(gCarParts[v->slot].weap, weaponOffsets[car->battle.weapon.type][model[v->slot].body_type], 0);
    }
    {
        pos[0] = weaponOffsets[car->battle.weapon.type][model[v->slot].body_type][0];
        pos[1] = weaponOffsets[car->battle.weapon.type][model[v->slot].body_type][1];
        pos[2] = weaponOffsets[car->battle.weapon.type][model[v->slot].body_type][2];
        if (gCurWeaponType[v->slot] == 1) {
            pos[0] += 0.0099999998928979039f;
            pos[1] += 0.64999998360872269f;
            pos[2] += 3.5899999439716339f;
            for (i = 0; i < 5; i++) {
                CopyUV(mat->mat3.uvs, gGattMuzzleObjs[v->slot].mats[i].mat.mat3.uvs);
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[0][0] * pos[0] + mat->mat3.pos[0];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[0][1] * pos[0] + mat->mat3.pos[1];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[0][2] * pos[0] + mat->mat3.pos[2];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[1][0] * pos[1] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[1][1] * pos[1] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[1][2] * pos[1] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[2][0] * pos[2] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[2][1] * pos[2] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1];
                gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[2][2] * pos[2] + gGattMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2];
            }
            if (gGattMuzzleObjs[v->slot].on) {
                gGattMuzzleObjs[v->slot].onTime += gFrameSeconds;
                if (gGattMuzzleObjs[v->slot].onTime >= 0.03333330200985074f) {
                    gGattMuzzleObjs[v->slot].on = 0;
                    for (i = 0; i < 5; i++)
                        MBOX_HideObject(gGattMuzzleObjs[v->slot].mdls[i], 1, 15);
                }
            }
            if (car->battle.shoot >= 5) {
                gGattMuzzleObjs[v->slot].on = 1;
                gGattMuzzleObjs[v->slot].onTime = 0.0f;
                MBOX_ShowObject(gGattMuzzleObjs[v->slot].mdls[0], 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    MBOX_HideObject(gGattMuzzleObjs[v->slot].mdls[0], 1, 1 << car->camera);
                r1 = (int)Random(4.0000000596046448f) + 1;
                r2 = Random(0.5000000074505806f);
                RollUV(r2, gGattMuzzleObjs[v->slot].mats[r1].mat.mat3.uvs);
                ScaleUV(gGattMuzzleObjs[v->slot].mats[r1].mat.mat3.uvs, gGattMuzzleObjs[v->slot].mats[r1].mat.mat3.uvs, 0.85000003129243851f + Random(0.7500000074505806f));
                MBOX_ShowObject(gGattMuzzleObjs[v->slot].mdls[r1], 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    MBOX_HideObject(gGattMuzzleObjs[v->slot].mdls[r1], 1, 1 << car->camera);
                r1++;
                if (r1 >= 5)
                    r1 = 1;
                RollUV(r2, gGattMuzzleObjs[v->slot].mats[r1].mat.mat3.uvs);
                ScaleUV(gGattMuzzleObjs[v->slot].mats[r1].mat.mat3.uvs, gGattMuzzleObjs[v->slot].mats[r1].mat.mat3.uvs, 0.85000003129243851f + Random(0.7500000074505806f));
                MBOX_ShowObject(gGattMuzzleObjs[v->slot].mdls[r1], 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    MBOX_HideObject(gGattMuzzleObjs[v->slot].mdls[r1], 1, 1 << car->camera);
            }
        }
        if (gCurWeaponType[v->slot] == 0) {
            pos[0] += 0.15999999828636646f;
            pos[1] += 0.47999999299645424f;
            pos[2] += 2.5699999630451202f;
            for (i = 0; i < 5; i++) {
                CopyUV(mat->mat3.uvs, gCannMuzzleObjs[v->slot].mats[i].mat.mat3.uvs);
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[0][0] * pos[0] + mat->mat3.pos[0];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[0][1] * pos[0] + mat->mat3.pos[1];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[0][2] * pos[0] + mat->mat3.pos[2];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[1][0] * pos[1] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[1][1] * pos[1] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[1][2] * pos[1] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0] = mat->mat3.uvs[2][0] * pos[2] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[0];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1] = mat->mat3.uvs[2][1] * pos[2] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[1];
                gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2] = mat->mat3.uvs[2][2] * pos[2] + gCannMuzzleObjs[v->slot].mats[i].mat.mat3.pos[2];
            }
            CopyUV(mat->mat3.uvs, gCannMuzzleObjs[v->slot].mat.mat3.uvs);
            gCannMuzzleObjs[v->slot].mat.mat3.pos[0] = mat->mat3.uvs[0][0] * pos[0] + mat->mat3.pos[0];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[1] = mat->mat3.uvs[0][1] * pos[0] + mat->mat3.pos[1];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[2] = mat->mat3.uvs[0][2] * pos[0] + mat->mat3.pos[2];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[0] = mat->mat3.uvs[1][0] * pos[1] + gCannMuzzleObjs[v->slot].mat.mat3.pos[0];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[1] = mat->mat3.uvs[1][1] * pos[1] + gCannMuzzleObjs[v->slot].mat.mat3.pos[1];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[2] = mat->mat3.uvs[1][2] * pos[1] + gCannMuzzleObjs[v->slot].mat.mat3.pos[2];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[0] = mat->mat3.uvs[2][0] * pos[2] + gCannMuzzleObjs[v->slot].mat.mat3.pos[0];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[1] = mat->mat3.uvs[2][1] * pos[2] + gCannMuzzleObjs[v->slot].mat.mat3.pos[1];
            gCannMuzzleObjs[v->slot].mat.mat3.pos[2] = mat->mat3.uvs[2][2] * pos[2] + gCannMuzzleObjs[v->slot].mat.mat3.pos[2];
            if (gCannMuzzleObjs[v->slot].ripCnt != -1) {
                gCannMuzzleObjs[v->slot].ripTime -= gFrameSeconds;
                if (gCannMuzzleObjs[v->slot].ripTime <= 0.0f) {
                    gCannMuzzleObjs[v->slot].ripTime = 0.03333330200985074f;
                    gCannMuzzleObjs[v->slot].ripCnt++;
                    if (gCannMuzzleObjs[v->slot].ripCnt >= 5) {
                        MBOX_HideObject(gCannMuzzleObjs[v->slot].ripdl, 1, 15);
                        gCannMuzzleObjs[v->slot].ripCnt = -1;
                    } else {
                        gCannMuzzleObjs[v->slot].ripTIdx++;
                        if (gCannMuzzleObjs[v->slot].ripTIdx >= 5)
                            gCannMuzzleObjs[v->slot].ripTIdx = 0;
                        ZOID_SetTexture(gCannMuzzleObjs[v->slot].ripdl, &gTexDefPtrs[gRipTextures[gCannMuzzleObjs[v->slot].ripTIdx] >> 10][gRipTextures[gCannMuzzleObjs[v->slot].ripTIdx] & 1023], -1);
                    }
                }
            }
            if (gCannMuzzleObjs[v->slot].on) {
                gCannMuzzleObjs[v->slot].onTime += gFrameSeconds;
                if (gCannMuzzleObjs[v->slot].onTime >= 0.066666700877249241f) {
                    gCannMuzzleObjs[v->slot].on = 0;
                    for (i = 0; i < 5; i++)
                        MBOX_HideObject(gCannMuzzleObjs[v->slot].mdls[i], 1, 15);
                }
            }
            if (car->battle.shoot >= 5) {
                gCannMuzzleObjs[v->slot].ripScale = 1.5000000149011612f + Random(1.5000000149011612f);
                gCannMuzzleObjs[v->slot].ripTime = 0.03333330200985074f;
                gCannMuzzleObjs[v->slot].ripCnt = 0;
                gCannMuzzleObjs[v->slot].ripTIdx = (int)Random(5.0000000596046448f);
                ZOID_SetTexture(gCannMuzzleObjs[v->slot].ripdl, &gTexDefPtrs[gRipTextures[0] >> 10][gRipTextures[0] & 1023], -1);
                MBOX_ShowObject(gCannMuzzleObjs[v->slot].ripdl, 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    MBOX_HideObject(gCannMuzzleObjs[v->slot].ripdl, 1, 1 << car->camera);
                gCannMuzzleObjs[v->slot].on = 1;
                gCannMuzzleObjs[v->slot].onTime = 0.0f;
                MBOX_ShowObject(gCannMuzzleObjs[v->slot].mdls[0], 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    MBOX_HideObject(gCannMuzzleObjs[v->slot].mdls[0], 1, 1 << car->camera);
                for (i = 1; i < 5; i++) {
                    RollUV(Random(1.2500000149011612f) - 0.5000000074505806f, gCannMuzzleObjs[v->slot].mats[i].mat.mat3.uvs);
                    ScaleUV(gCannMuzzleObjs[v->slot].mats[i].mat.mat3.uvs, gCannMuzzleObjs[v->slot].mats[i].mat.mat3.uvs, 0.40000000968575478f + Random(0.2500000037252903f));
                    MBOX_ShowObject(gCannMuzzleObjs[v->slot].mdls[i], 0, 15);
                    if (car->camera >= 0 && car->view <= 1)
                        MBOX_HideObject(gCannMuzzleObjs[v->slot].mdls[i], 1, 1 << car->camera);
                }
            }
            ScaleUV(gCannMuzzleObjs[v->slot].mat.mat3.uvs, gCannMuzzleObjs[v->slot].mat.mat3.uvs, gCannMuzzleObjs[v->slot].ripScale);
        }
    }
}
