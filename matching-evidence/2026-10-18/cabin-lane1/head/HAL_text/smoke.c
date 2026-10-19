#include "smoke_types.h"
extern tSmokeCar gGameCar[];
extern tSmokeModel model[];
extern volatile signed char gNumModels;
extern volatile float gFrameSeconds;
extern unsigned short gObjList[];
extern tSmokeObj ObjInstanceList[];
extern tTexture *gTexDefPtrs[];
extern unsigned char gCarWings[][13];
extern void InitStatLL(tStatList *,void *,int,int,char);
extern void *GetStatLLE(tStatList *);
extern void FreeStatLLE(tStatList *,void *);
extern tTexture *MBOX_FindTexture_WORK(char *,unsigned short *,signed char,signed char,int);
extern signed short MBOX_NewObject(unsigned short,tUVMatrix *,signed short,unsigned int);
extern void MBOX_HideObject(int,int,int);
extern void MBOX_ShowObject(int,int,int);
extern float NormalVector(float *);
extern void LookInDir(float *,tUVMatrix *);
extern void CopyUV(float (*)[3],float (*)[3]);
extern float Random(float);
extern void ZOID_SetTexture(int,tTexture *,int);
extern void ZOID_SetColor(int,tRGBA *);
extern void ZOID_SetObjectDef(int,unsigned short);
extern void ZOID_RemoveObject(int);
extern float _builtin_fabsf(float);
#define fabs(x) _builtin_fabsf(x)
static float gCarMiddles[13] = {
    1.3999999910593033f,1.5000000149011612f,1.2999999672174454f,1.5000000149011612f,1.5000000149011612f,1.700000062584877f,1.5000000149011612f,1.5000000149011612f,1.6000000387430191f,1.5000000149011612f,2.5000000298023224f,1.3999999910593033f,1.2999999672174454f
};
static unsigned int gCarSmokeDir[6];
static unsigned char gCarBlastSmokeClr[6];
static float gCarWingSmokePos[6][2][3];
static tSmoke gSmokeObjs[200];
tStatList gSmokeList;
static unsigned short gFireTexs[6];
void InitSmoke(void)
{
    static char * const FireTexNames[6] = {
        "FIREGEN05", "FIREGEN03", "FIREGEN01",
        "FIREGEN07", "FIREGEN10", "FIREGEN10"
    };
    int i;
    InitStatLL(&gSmokeList, gSmokeObjs, sizeof(gSmokeObjs[0]), 200, 0);
    for (i = 0; i < 6; i++)
        gCarSmokeDir[i] = 0;
    for (i = 0; i < 6; i++)
        MBOX_FindTexture_WORK(FireTexNames[i], &gFireTexs[i], 0,
                             gNumModels - 1, 1);
}

void InitSmokeBlast(signed short slot)
{
    gCarSmokeDir[slot] &= ~0x20u;
}

void InitWingSmoke(signed short slot, signed short side)
{
    if (side)
        gCarSmokeDir[slot] &= ~0x8000u;
    else
        gCarSmokeDir[slot] &= ~0x4000u;
}

void StartWater(signed short slot, unsigned int tire)
{
    tSmokeCar *car = &gGameCar[slot];
    float unused1[3];
    float pos2[3];
    float unused2[3];
    float offset;
    float offset2;
    float fmph;
    signed short mph;
    signed short mph2;
    int j;
    int k;
    tRGBA wht = { 255,255,255,255 };
    tSmoke *smk;
    tUVMatrix mat;

    mph = car->rwmph >> 2;
    mph2 = fabs(mph);
    switch (tire) {
    case 0:
        if (gCarSmokeDir[slot] & 0x100u) return;
        gCarSmokeDir[slot] |= 0x100u;
        break;
    case 1:
        if (gCarSmokeDir[slot] & 0x200u) return;
        gCarSmokeDir[slot] |= 0x200u;
        break;
    case 2:
        if (gCarSmokeDir[slot] & 0x400u) return;
        gCarSmokeDir[slot] |= 0x400u;
        break;
    case 3:
        if (gCarSmokeDir[slot] & 0x800u) return;
        gCarSmokeDir[slot] |= 0x800u;
        break;
    }
    smk = GetStatLLE(&gSmokeList);
    if (!smk) return;
    smk->swTime = 0.03333330200985074f; /* the original literal: 0x3D088880, not 1/30 (0x3D088889) */
    smk->data = tire << 17;
    smk->slot = slot;
    smk->data |= 0x10u;
    pos2[0] = car->dr_vel[0];
    pos2[1] = car->dr_vel[1];
    pos2[2] = car->dr_vel[2];
    NormalVector(pos2);
    LookInDir(pos2, &mat);
    CopyUV(mat.uvs, smk->mat.uvs);
    if (mph2 < 90) {
        fmph = mph2;
        if (mph < 0)
            offset2 = 0.2500000037252903f + 0.7500000074505806f * (fmph / 70.000000953674316f);
        else
            offset2 = 0.15000000782310963f + 0.85000003129243851f * (fmph / 90.000000953674316f);
        /* `(&...uvs[0][0])[j]`, not a `(float *)` cast: the cast leaves an extra address temporary that raises smk's interference and rotates every callee-saved register */
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) smk->mat.uvs[j][k] *= offset2;
        }
    }
    smk->mat.mat3.pos[0] = car->dr_tirepos[tire][0];
    smk->mat.mat3.pos[1] = car->dr_tirepos[tire][1];
    smk->mat.mat3.pos[2] = car->dr_tirepos[tire][2];
    smk->vel[0] = car->dr_uvs[0][0];
    smk->vel[1] = car->dr_uvs[0][1];
    smk->vel[2] = car->dr_uvs[0][2];
    offset = Random(1.0000000149011612f) - 0.5000000074505806f;
    smk->vel[0] *= offset;
    smk->vel[1] *= offset;
    smk->vel[2] *= offset;
    smk->mat.mat3.pos[0] += smk->vel[0];
    smk->mat.mat3.pos[1] += smk->vel[1];
    smk->mat.mat3.pos[2] += smk->vel[2];
    smk->mat.mat3.pos[1] += 1.2500000149011612f;
    smk->tIdx = 0;
    smk->mbIdx = MBOX_NewObject(gObjList[516], &smk->mat, -1, 0x40000u);

    smk = GetStatLLE(&gSmokeList);
    if (!smk) return;
    smk->swTime = 0.03333330200985074f; /* the original literal: 0x3D088880, not 1/30 (0x3D088889) */
    smk->data = tire << 17;
    smk->data |= 0x10u;
    smk->slot = slot;
    CopyUV(mat.uvs, smk->mat.uvs);
    if (mph2 < 90) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) smk->mat.uvs[j][k] *= offset2;
        }
    }
    smk->mat.mat3.pos[0] = car->dr_tirepos[tire][0];
    smk->mat.mat3.pos[1] = car->dr_tirepos[tire][1];
    smk->mat.mat3.pos[2] = car->dr_tirepos[tire][2];
    smk->vel[0] = car->dr_uvs[0][0];
    smk->vel[1] = car->dr_uvs[0][1];
    smk->vel[2] = car->dr_uvs[0][2];
    offset = Random(1.0000000149011612f) - 0.5000000074505806f;
    smk->vel[0] *= offset;
    smk->vel[1] *= offset;
    smk->vel[2] *= offset;
    smk->mat.mat3.pos[0] += smk->vel[0];
    smk->mat.mat3.pos[1] += smk->vel[1];
    smk->mat.mat3.pos[2] += smk->vel[2];
    smk->mat.mat3.pos[1] += 1.2500000149011612f;
    smk->tIdx = 4;
    smk->mbIdx = MBOX_NewObject(gObjList[516 + smk->tIdx], &smk->mat, -1, 0x40000u);
}

/* TODO: remaining original bodies. These stubs retain NonMatching status. */
void StartSmoke(signed short slot, unsigned int tire, int water, float *smkoffset)
{
    tSmokeCar *car = &gGameCar[slot];
    tSmokeModel *m = &model[slot];
    float pos2[3];
    float gap;
    float offset;
    float offset2;
    signed short surface = model[slot].sviscode[tire];
    signed short first;
    signed short mph2;
    int howmany;
    int howmanydone;
    tSmoke *smk;
    tRGBA wht = { 255,255,255,255 };
    tRGBA wg1 = { 180,255,180,160 };
    tRGBA wg2 = { 185,180,255,160 };
    tRGBA wg3 = { 255,192,187,160 };
    tRGBA rdClr = { 133,125,117,255 };
    tRGBA dtClr = { 71,108,157,255 };
    float pos[3];
    float prevPos[3];

    howmany = 1;
    howmanydone = 0;
    mph2 = fabs((signed short)(car->rwmph >> 2));
    if ((surface == 8 && tire < 5) || (mph2 < 10 && tire < 4))
        return;
    first = 1;
    if (tire > 5) gap=0.7500000074505806f; else gap=2.2500000298023224f;
    if (water) {
        StartWater(slot,tire);
        return;
    }
    if (tire == 5) {
        howmany = 5;
    } else if (tire < 4 || tire > 5) {
        if (mph2 >= 115) howmany = 4;
        else if (mph2 >= 100) howmany = 3;
        else if (mph2 >= 85) howmany = 2;
        if (tire > 5) {
            if (tire == 6 && !(gCarSmokeDir[slot] & 0x4000u)) howmany = 1;
            else if (tire == 7 && !(gCarSmokeDir[slot] & 0x8000u)) howmany = 1;
            else howmany += 5;
            if (mph2 >= 150 && howmany != 1) howmany += 3;
        }
    } else {
        if (gCarSmokeDir[slot] & 0x40u) {
            gCarSmokeDir[slot] &= ~0x40u;
            gCarSmokeDir[slot] |= 0x80u;
            return;
        }
        if (gCarSmokeDir[slot] & 0x80u) {
            gCarSmokeDir[slot] &= ~0x80u;
            return;
        }
    }
    do {
        smk = GetStatLLE(&gSmokeList);
        if (!smk) return;
        smk->swTime = 0.03333330200985074f;
        smk->data = tire << 17;
        smk->slot = slot;
        if (tire == 5) {
            smk->tIdx = -1;
            smk->mbIdx = MBOX_NewObject(gObjList[515], &smk->mat, -1, 0x52000u);
            ObjInstanceList[smk->mbIdx].w = 1.010000005364418f;
        } else {
            smk->tIdx = (int)Random(8.0000001192092896f);
            smk->mbIdx = MBOX_NewObject(gObjList[507 + smk->tIdx], &smk->mat, -1, 0x52000u);
        }
        if (tire > 5) smk->lifeTime = 0.079999999143183231f + Random(0.070000001229345798f);
        else if (howmany == 1) smk->lifeTime = 0.5000000074505806f + Random(1.5000000149011612f);
        else if (tire == 5) smk->lifeTime = 0.5000000074505806f + Random(2.2500000298023224f);
        else smk->lifeTime = 0.5000000074505806f + Random(0.2500000037252903f);
        if (tire < 5) {
            smk->vel[0] = car->dr_vel[0];
            smk->vel[1] = car->dr_vel[1];
            smk->vel[2] = car->dr_vel[2];
            smk->vel[0] *= 0.12500000186264515f;
            smk->vel[1] *= 0.12500000186264515f;
            smk->vel[2] *= 0.12500000186264515f;
            if (tire == 4) {
                smk->vel[0] *= 0.5000000074505806f;
                smk->vel[1] *= 0.5000000074505806f;
                smk->vel[2] *= 0.5000000074505806f;
            }
        } else if (tire > 5) {
            smk->vel[0] = car->dr_vel[0];
            smk->vel[1] = car->dr_vel[1];
            smk->vel[2] = car->dr_vel[2];
            smk->vel[0] *= 0.5000000074505806f;
            smk->vel[1] *= 0.5000000074505806f;
            smk->vel[2] *= 0.5000000074505806f;
        }
        switch (tire) {
        case 0:
            if (gCarSmokeDir[slot] & 1u) gCarSmokeDir[slot] &= ~1u;
            else { gCarSmokeDir[slot] |= 1u; smk->data |= 1u; }
            break;
        case 1:
            if (gCarSmokeDir[slot] & 2u) gCarSmokeDir[slot] &= ~2u;
            else { gCarSmokeDir[slot] |= 2u; smk->data |= 1u; }
            break;
        case 2:
            if (gCarSmokeDir[slot] & 4u) gCarSmokeDir[slot] &= ~4u;
            else { gCarSmokeDir[slot] |= 4u; smk->data |= 1u; }
            break;
        case 3:
            if (gCarSmokeDir[slot] & 8u) gCarSmokeDir[slot] &= ~8u;
            else { gCarSmokeDir[slot] |= 8u; smk->data |= 1u; }
            break;
        case 4:
            gCarSmokeDir[slot] |= 0x40u;
            smk->lifeTime += 1.0000000149011612f;
            if (!(gCarSmokeDir[slot] & 0x20u)) {
                gCarSmokeDir[slot] |= 0x20u;
                gCarBlastSmokeClr[slot] = 175;
            }
            if (gCarSmokeDir[slot] & 0x10u) gCarSmokeDir[slot] &= ~0x10u;
            else { gCarSmokeDir[slot] |= 0x10u; smk->data |= 1u; }
            pos[0] = car->dr_pos[0]; pos[1] = car->dr_pos[1]; pos[2] = car->dr_pos[2];
            pos[0] += car->dr_uvs[0][0] * (Random(5.0000000596046448f) - 2.5000000298023224f);
            pos[1] += car->dr_uvs[0][1] * (Random(5.0000000596046448f) - 2.5000000298023224f);
            pos[2] += car->dr_uvs[0][2] * (Random(5.0000000596046448f) - 2.5000000298023224f);
            pos[0] += car->dr_uvs[2][0] * (Random(7.0000000596046448f) - 3.5000000298023224f);
            pos[1] += car->dr_uvs[2][1] * (Random(7.0000000596046448f) - 3.5000000298023224f);
            pos[2] += car->dr_uvs[2][2] * (Random(7.0000000596046448f) - 3.5000000298023224f);
            pos[0] += car->dr_uvs[1][0] * gCarMiddles[model[slot].body_type];
            pos[1] += car->dr_uvs[1][1] * gCarMiddles[model[slot].body_type];
            pos[2] += car->dr_uvs[1][2] * gCarMiddles[model[slot].body_type];
            pos[1] += 0.7500000074505806f;
            ObjInstanceList[smk->mbIdx].scale = 0.7500000074505806f + Random(0.5000000074505806f);
            break;
        case 5:
            ObjInstanceList[smk->mbIdx].scale = 0.20000000484287739f;
            ZOID_SetTexture(smk->mbIdx, &gTexDefPtrs[gFireTexs[howmanydone] >> 10][gFireTexs[howmanydone] & 1023], -1);
            pos[0] = car->dr_pos[0]; pos[1] = car->dr_pos[1]; pos[2] = car->dr_pos[2];
            smk->vel[0] = Random(4.0000000596046448f) - 2.0000000298023224f;
            smk->vel[2] = Random(8.0000001192092896f) - 4.0000000596046448f;
            pos[0] += car->dr_uvs[0][0] * smk->vel[0];
            pos[1] += car->dr_uvs[0][1] * smk->vel[0];
            pos[2] += car->dr_uvs[0][2] * smk->vel[0];
            pos[0] += car->dr_uvs[2][0] * smk->vel[2];
            pos[1] += car->dr_uvs[2][1] * smk->vel[2];
            pos[2] += car->dr_uvs[2][2] * smk->vel[2];
            pos[0] += car->dr_uvs[1][0] * gCarMiddles[model[slot].body_type];
            pos[1] += car->dr_uvs[1][1] * gCarMiddles[model[slot].body_type];
            pos[2] += car->dr_uvs[1][2] * gCarMiddles[model[slot].body_type];
            break;
        case 6:
            if (gCarSmokeDir[slot] & 0x1000u) gCarSmokeDir[slot] &= ~0x1000u;
            else { gCarSmokeDir[slot] |= 0x1000u; smk->data |= 1u; }
            break;
        case 7:
            if (gCarSmokeDir[slot] & 0x2000u) gCarSmokeDir[slot] &= ~0x2000u;
            else { gCarSmokeDir[slot] |= 0x2000u; smk->data |= 1u; }
            break;
        }
        if (tire < 4) {
            offset2 = gap * howmanydone;
            pos[0] = car->dr_uvs[2][0] * (-2.0000000298023224f - offset2);
            pos[1] = car->dr_uvs[2][1] * (-2.0000000298023224f - offset2);
            pos[2] = car->dr_uvs[2][2] * (-2.0000000298023224f - offset2);
            pos[0] += car->dr_tirepos[tire][0];
            pos[1] += car->dr_tirepos[tire][1];
            pos[2] += car->dr_tirepos[tire][2];
            pos[0] += car->dr_uvs[0][0] * (Random(2.0000000298023224f) - 1.0000000149011612f);
            pos[1] += car->dr_uvs[0][1] * (Random(2.0000000298023224f) - 1.0000000149011612f);
            pos[2] += car->dr_uvs[0][2] * (Random(2.0000000298023224f) - 1.0000000149011612f);
            pos[1] += m->reckon.suscomp[tire];
            ObjInstanceList[smk->mbIdx].scale = 0.2500000037252903f + Random(0.7500000074505806f);
        }
        if (tire > 5) {
            pos[0] = car->dr_pos[0]; pos[1] = car->dr_pos[1]; pos[2] = car->dr_pos[2];
            pos[0] += car->dr_uvs[1][0] * smkoffset[1];
            pos[1] += car->dr_uvs[1][1] * smkoffset[1];
            pos[2] += car->dr_uvs[1][2] * smkoffset[1];
            pos[0] += car->dr_uvs[2][0] * smkoffset[2];
            pos[1] += car->dr_uvs[2][1] * smkoffset[2];
            pos[2] += car->dr_uvs[2][2] * smkoffset[2];
            pos[0] += -1.1499999910593033f * car->dr_uvs[2][0];
            pos[1] += -1.1499999910593033f * car->dr_uvs[2][1];
            pos[2] += -1.1499999910593033f * car->dr_uvs[2][2];
            if (tire == 6) {
            pos[0] += car->dr_uvs[0][0] * smkoffset[0];
            pos[1] += car->dr_uvs[0][1] * smkoffset[0];
            pos[2] += car->dr_uvs[0][2] * smkoffset[0];
                pos[0] += -2.5999999344348907f * car->dr_uvs[0][0];
                pos[1] += -2.5999999344348907f * car->dr_uvs[0][1];
                pos[2] += -2.5999999344348907f * car->dr_uvs[0][2];
            } else {
            pos[0] += car->dr_uvs[0][0] * smkoffset[0];
            pos[1] += car->dr_uvs[0][1] * smkoffset[0];
            pos[2] += car->dr_uvs[0][2] * smkoffset[0];
                pos[0] += 2.5999999344348907f * car->dr_uvs[0][0];
                pos[1] += 2.5999999344348907f * car->dr_uvs[0][1];
                pos[2] += 2.5999999344348907f * car->dr_uvs[0][2];
            }
            pos[0] += 0.5000000074505806f * car->dr_uvs[1][0];
            pos[1] += 0.5000000074505806f * car->dr_uvs[1][1];
            pos[2] += 0.5000000074505806f * car->dr_uvs[1][2];
            if (first) {
                if (tire == 6) {
                    if (!(gCarSmokeDir[slot] & 0x4000u)) gCarSmokeDir[slot] |= 0x4000u;
                    else { prevPos[0]=gCarWingSmokePos[slot][0][0]; prevPos[1]=gCarWingSmokePos[slot][0][1]; prevPos[2]=gCarWingSmokePos[slot][0][2]; }
                    gCarWingSmokePos[slot][0][0]=pos[0]; gCarWingSmokePos[slot][0][1]=pos[1]; gCarWingSmokePos[slot][0][2]=pos[2];
                } else if (tire == 7) {
                    if (!(gCarSmokeDir[slot] & 0x8000u)) gCarSmokeDir[slot] |= 0x8000u;
                    else { prevPos[0]=gCarWingSmokePos[slot][1][0]; prevPos[1]=gCarWingSmokePos[slot][1][1]; prevPos[2]=gCarWingSmokePos[slot][1][2]; }
                    gCarWingSmokePos[slot][1][0]=pos[0]; gCarWingSmokePos[slot][1][1]=pos[1]; gCarWingSmokePos[slot][1][2]=pos[2];
                }
            } else {
                pos2[0]=prevPos[0]-pos[0]; pos2[1]=prevPos[1]-pos[1]; pos2[2]=prevPos[2]-pos[2];
                offset=NormalVector(pos2);
                if (offset) {
                    offset2=howmanydone*(offset/howmany);
                    pos[0]+=pos2[0]*offset2; pos[1]+=pos2[1]*offset2; pos[2]+=pos2[2]*offset2;
                }
            }
            ObjInstanceList[smk->mbIdx].scale = 0.075000003911554813f + Random(0.10000000242143869f);
        }
        MBOX_ShowObject(smk->mbIdx,0,15);
        if (car->camera >= 0 && (car->view == 0 || car->view == 1))
            MBOX_HideObject(smk->mbIdx,0,1<<car->camera);
        if (tire > 5) {
            switch(gCarWings[slot+1][m->body_type]) {
            case 0: ZOID_SetColor(smk->mbIdx,&wg1); break;
            case 1: ZOID_SetColor(smk->mbIdx,&wg2); break;
            case 2: ZOID_SetColor(smk->mbIdx,&wg3); break;
            }
        } else if (tire == 4) {
            ObjInstanceList[smk->mbIdx].color.c[0]=gCarBlastSmokeClr[slot];
            ObjInstanceList[smk->mbIdx].color.c[1]=gCarBlastSmokeClr[slot];
            ObjInstanceList[smk->mbIdx].color.c[2]=gCarBlastSmokeClr[slot];
            ObjInstanceList[smk->mbIdx].color.c[3]=255;
            gCarBlastSmokeClr[slot]-=3;
            if(gCarBlastSmokeClr[slot]<53) gCarBlastSmokeClr[slot]=53;
        } else if (tire == 5) ZOID_SetColor(smk->mbIdx,&wht);
        else if (surface == 1) ZOID_SetColor(smk->mbIdx,&dtClr);
        else ZOID_SetColor(smk->mbIdx,&rdClr);
        smk->mat.mat3.pos[0]=pos[0];
        smk->mat.mat3.pos[1]=pos[1];
        smk->mat.mat3.pos[2]=pos[2];
        howmanydone++;
        first=0;
    } while(howmany != howmanydone);
}
void UpdateSmoke(void)
{
    signed short tire;
    int j;
    int k;
    float t;
    float pos[3];
    float pos2[3];
    tSmokeCar *gc;
    tSmoke *smk;
    tSmoke *next;
    signed short mph;
    signed short mph2;
    float fmph;
    tUVMatrix mat;

    for (smk = gSmokeList.head; smk; smk = next) {
        signed short slot = smk->slot;
        tire = (smk->data >> 17) & 15;
        next = smk->ll.next;
        gc = &gGameCar[slot];
        if (smk->data & 0x10u) {
            mph = gc->rwmph >> 2;
            mph2 = fabs(mph);
            if (gc->view < 2)
                MBOX_HideObject(smk->mbIdx, 1, 1 << gc->camera);
            else
                MBOX_ShowObject(smk->mbIdx, 0, 1 << gc->camera);
            if (model[slot].sviscode[tire] != 2 || mph2 < 10) {
                switch (tire) {
                case 0: gCarSmokeDir[slot] &= ~0x100u; break;
                case 1: gCarSmokeDir[slot] &= ~0x200u; break;
                case 2: gCarSmokeDir[slot] &= ~0x400u; break;
                case 3: gCarSmokeDir[slot] &= ~0x800u; break;
                }
                goto release;
            }
            pos2[0] = gc->dr_vel[0];
            pos2[1] = gc->dr_vel[1];
            pos2[2] = gc->dr_vel[2];
            NormalVector(pos2);
            LookInDir(pos2, &mat);
            CopyUV(mat.uvs, smk->mat.uvs);
            if (mph2 < 90) {
                fmph = mph2;
                if (mph < 0)
                    t = 0.2500000037252903f + 0.7500000074505806f * (fmph / 70.000000953674316f);
                else
                    t = 0.15000000782310963f + 0.85000003129243851f * (fmph / 90.000000953674316f);
                for (j = 0; j < 3; j++) {
                    for (k = 0; k < 3; k++) smk->mat.uvs[j][k] = smk->mat.uvs[j][k] * (t);
                }
            }
            smk->mat.mat3.pos[0] = gc->dr_tirepos[tire][0];
            smk->mat.mat3.pos[1] = gc->dr_tirepos[tire][1];
            smk->mat.mat3.pos[2] = gc->dr_tirepos[tire][2];
            smk->mat.mat3.pos[0] = smk->mat.mat3.pos[0] + (smk->vel[0]);
            smk->mat.mat3.pos[1] = smk->mat.mat3.pos[1] + (smk->vel[1]);
            smk->mat.mat3.pos[2] = smk->mat.mat3.pos[2] + (smk->vel[2]);
            smk->mat.mat3.pos[1] = smk->mat.mat3.pos[1] + (1.2500000149011612f);
            smk->swTime = smk->swTime - (gFrameSeconds);
            if (smk->swTime <= 1.7516230804060213e-46f) {
                ZOID_SetObjectDef(smk->mbIdx, gObjList[516 + smk->tIdx]);
                smk->tIdx++;
                if (smk->tIdx >= 8) smk->tIdx = 0;
                smk->swTime = 0.03333330200985074f;
            }
        } else if (tire == 5) {
            if (!model[slot].crashflag) goto release;
            if (smk->tIdx == -1) {
                if (ObjInstanceList[smk->mbIdx].scale < smk->lifeTime)
                    ObjInstanceList[smk->mbIdx].scale = ObjInstanceList[smk->mbIdx].scale + (0.039999999571591616f);
                else
                    smk->tIdx = 0;
            } else {
                if (smk->tIdx <= 7)
                    ObjInstanceList[smk->mbIdx].scale = ObjInstanceList[smk->mbIdx].scale - (0.02999999956227839f);
                else
                    ObjInstanceList[smk->mbIdx].scale = ObjInstanceList[smk->mbIdx].scale + (0.02999999956227839f);
                if (smk->tIdx >= 14)
                    smk->tIdx = 0;
                else
                    smk->tIdx++;
            }
            pos[0] = gc->dr_pos[0];
            pos[1] = gc->dr_pos[1];
            pos[2] = gc->dr_pos[2];
            pos[0] = pos[0] + (gc->dr_uvs[0][0] * smk->vel[0]);
            pos[1] = pos[1] + (gc->dr_uvs[0][1] * smk->vel[0]);
            pos[2] = pos[2] + (gc->dr_uvs[0][2] * smk->vel[0]);
            pos[0] = pos[0] + (gc->dr_uvs[2][0] * smk->vel[2]);
            pos[1] = pos[1] + (gc->dr_uvs[2][1] * smk->vel[2]);
            pos[2] = pos[2] + (gc->dr_uvs[2][2] * smk->vel[2]);
            pos[0] = pos[0] + (gc->dr_uvs[1][0] * gCarMiddles[model[slot].body_type]);
            pos[1] = pos[1] + (gc->dr_uvs[1][1] * gCarMiddles[model[slot].body_type]);
            pos[2] = pos[2] + (gc->dr_uvs[1][2] * gCarMiddles[model[slot].body_type]);
            smk->mat.mat3.pos[0] = pos[0];
            smk->mat.mat3.pos[1] = pos[1];
            smk->mat.mat3.pos[2] = pos[2];
        } else {
            smk->lifeTime = smk->lifeTime - (gFrameSeconds);
            if (smk->lifeTime <= 1.7516230804060213e-46f) {
release:
                ZOID_RemoveObject(smk->mbIdx);
                FreeStatLLE(&gSmokeList, smk);
                continue;
            }
            t = gFrameSeconds;
            if (smk->lifeTime < 1.0000000149011612f) {
                if (ObjInstanceList[smk->mbIdx].color.c[3] >= 9)
                    ObjInstanceList[smk->mbIdx].color.c[3] -= 9;
            }
            if (tire == 4) {
                smk->mat.mat3.pos[0] = smk->mat.mat3.pos[0] + (Random(0.30000001564621925f) - 0.15000000782310963f);
                smk->mat.mat3.pos[1] = smk->mat.mat3.pos[1] + (Random(0.2500000037252903f));
                smk->mat.mat3.pos[2] = smk->mat.mat3.pos[2] + (Random(0.15000000782310963f) - 0.075000003911554813f);
                ObjInstanceList[smk->mbIdx].scale = ObjInstanceList[smk->mbIdx].scale + (0.050000001210719347f);
            }
            smk->swTime = smk->swTime - (gFrameSeconds);
            if (smk->swTime <= 1.7516230804060213e-46f) {
                ZOID_SetObjectDef(smk->mbIdx, gObjList[507 + smk->tIdx]);
                if (smk->data & 1u) {
                    smk->tIdx++;
                    if (smk->tIdx >= 8) smk->tIdx = 0;
                } else {
                    smk->tIdx--;
                    if (smk->tIdx < 0) smk->tIdx = 7;
                }
                smk->swTime = 0.03333330200985074f;
            }
            if (tire > 5)
                ObjInstanceList[smk->mbIdx].scale = ObjInstanceList[smk->mbIdx].scale + (0.02999999956227839f);
            else
                ObjInstanceList[smk->mbIdx].scale = ObjInstanceList[smk->mbIdx].scale + (0.050000001210719347f);
            smk->mat.mat3.pos[0] = smk->mat.mat3.pos[0] + (smk->vel[0] * gFrameSeconds);
            smk->mat.mat3.pos[1] = smk->mat.mat3.pos[1] + (smk->vel[1] * gFrameSeconds);
            smk->mat.mat3.pos[2] = smk->mat.mat3.pos[2] + (smk->vel[2] * gFrameSeconds);
        }
    }
}
