/* DC headlight projections; MAT3 control flow, target-backed private layouts. */
#include "..\include\headlights_private.h"

void AnimateForwardLights(int idx, int slot, float *backpos, float *frontpos);
void AnimateDownwardLights(int idx, int slot, float *backpos, float *frontpos);
void AnimateHeadLights(tVisual *v, signed short op);

static float gHeadlightOffsets[13][3] = {
    0.0f, 0.47000001370906829833984375f, 7.1500003337860107421875f,
    1.824000060558319091796875f, 1.380000054836273193359375f, 5.7320001125335693359375f,
    1.160000026226043701171875f, 0.6800000369548797607421875f, 5.5800001621246337890625f,
    2.09000003337860107421875f, 0.5600000321865081787109375f, 6.0300004482269287109375f,
    1.410000026226043701171875f, 1.150000035762786865234375f, 5.6700003147125244140625f,
    2.15000021457672119140625f, 1.340000092983245849609375f, 6.8600003719329833984375f,
    1.610000073909759521484375f, 0.8200000226497650146484375f, 5.9200003147125244140625f,
    1.720000088214874267578125f, 1.350000083446502685546875f, 5.4000003337860107421875f,
    1.420000016689300537109375f, 1.320000112056732177734375f, 4.5600001811981201171875f,
    2.08000004291534423828125f, 0.5900000035762786865234375f, 7.4300000667572021484375f,
    2.28000009059906005859375f, 2.17000019550323486328125f, 7.2300002574920654296875f,
    2.26000010967254638671875f, 1.180000007152557373046875f, 5.2700002193450927734375f,
    2.21000015735626220703125f, 1.300000011920928955078125f, 5.1000001430511474609375f
};

/* 0x8001b8d8, size 0x208 */
void StartHeadLights(signed short slot)
{
    tCarData *car = &gGameCar[slot];
    tVisual *v = &car->visuals[1]; /* target forms base+110 explicitly */
    tRGBA clr = {255,255,255,255}; /* stack */
    tUVMatrix *mat; /* r4 */

    if (gTrackNum == 3 || gTrackNum == 5) {
        v->func = AnimateHeadLights;
        v->slot = slot;
        mat = ZOID_NewMatrix();
        gCarParts[slot].headlight[0] = MBOX_NewObject(gObjList[528], mat, -1, 0x42000);
        ZOID_SetColor(gCarParts[slot].headlight[0], &clr);
        mat = ZOID_NewMatrix();
        gCarParts[slot].headlight[1] = MBOX_NewObject(gObjList[528], mat, -1, 0x42000);
        ZOID_SetColor(gCarParts[slot].headlight[1], &clr);
        ObjInstanceList[gCarParts[slot].headlight[0]].w = 1.3500001f;
        ObjInstanceList[gCarParts[slot].headlight[1]].w = 1.3500001f;
        mat = ZOID_NewMatrix();
        gCarParts[slot].headlight[2] = MBOX_NewObject(gObjList[529], mat, -1, 0x42000);
        ZOID_SetColor(gCarParts[slot].headlight[2], &clr);
        mat = ZOID_NewMatrix();
        gCarParts[slot].headlight[3] = MBOX_NewObject(gObjList[529], mat, -1, 0x42000);
        ZOID_SetColor(gCarParts[slot].headlight[3], &clr);
        ObjInstanceList[gCarParts[slot].headlight[2]].w = 1.25f;
        ObjInstanceList[gCarParts[slot].headlight[3]].w = 1.25f;
    } else {
        v->func = 0;
        gCarParts[slot].headlight[0] = 0;
        gCarParts[slot].headlight[1] = 0;
        gCarParts[slot].headlight[2] = 0;
        gCarParts[slot].headlight[3] = 0;
    }

}

/* 0x8001bae0, size 0x428 */
void AnimateForwardLights(int idx, int slot, float *backpos, float *frontpos)
{
    signed short roadflag; /* r0 */
    tCarData *car = &gGameCar[slot]; /* target forms car before color copy */
    tUVMatrix *mat; /* r30 */
    float dotx; /* f31 */
    float dx; /* f4 */
    float dx3; /* f3 */
    float delta[3]; /* target stack+56, declared before uvs */
    float surface_uvs[3][3]; /* stack+20 */
    float (*uvs)[3] = surface_uvs; /* target retained address r8 */
    tHeadlightSurface *surf; /* r0 */
    tRGBA clr = {255,255,255,243}; /* stack */

    surf = ShotToWorldCollision(backpos, frontpos, uvs, 0.5f, 1, 65535);
    if (surf) {
        mat = ZOID_FindMatrix(gCarParts[slot].headlight[idx]);
        mat->uvs[3][0] = frontpos[0];
        mat->uvs[3][1] = frontpos[1];
        mat->uvs[3][2] = frontpos[2];
        dotx = fabs(uvs[1][0]*car->dr_uvs[2][0] + uvs[1][1]*car->dr_uvs[2][1] + uvs[1][2]*car->dr_uvs[2][2]);
        roadflag = surf->flags & 15;
        if (dotx < 0.25f && roadflag != 5 && roadflag != 6) {
            MBOX_HideObject(gCarParts[slot].headlight[idx], 1, 15);
            return;
        }
        if (dotx != 0.0f) {
        
            mat->uvs[1][0] = uvs[1][0];
            mat->uvs[1][1] = uvs[1][1];
            mat->uvs[1][2] = uvs[1][2];
            mat->uvs[0][0] = uvs[1][1] * car->dr_uvs[2][2] - uvs[1][2] * car->dr_uvs[2][1];
            mat->uvs[0][1] = uvs[1][2] * car->dr_uvs[2][0] - uvs[1][0] * car->dr_uvs[2][2];
            mat->uvs[0][2] = uvs[1][0] * car->dr_uvs[2][1] - uvs[1][1] * car->dr_uvs[2][0];
            NormalVector(mat->uvs[0]);
            mat->uvs[2][0] = mat->uvs[0][1] * mat->uvs[1][2] - mat->uvs[0][2] * mat->uvs[1][1];
            dx3 = 1.0f + 3.0f * (1.0f-dotx);
            mat->uvs[2][1] = mat->uvs[0][2] * mat->uvs[1][0] - mat->uvs[0][0] * mat->uvs[1][2];
            mat->uvs[2][2] = mat->uvs[0][0] * mat->uvs[1][1] - mat->uvs[0][1] * mat->uvs[1][0];
            mat->uvs[2][0] *= dx3; mat->uvs[2][1] *= dx3; mat->uvs[2][2] *= dx3;

        } else CopyUV(uvs, mat->mat3.uvs);
        {
            delta[0] = backpos[0]-frontpos[0];
            delta[1] = backpos[1]-frontpos[1];
            delta[2] = backpos[2]-frontpos[2];
            dx = sqrtf(delta[0]*delta[0]+delta[1]*delta[1]+delta[2]*delta[2]);
        }
        dx3 = 1.0f + dx/7.5f;
        mat->uvs[2][0] *= dx3; mat->uvs[2][1] *= dx3; mat->uvs[2][2] *= dx3;
        mat->uvs[0][0] *= dx3; mat->uvs[0][1] *= dx3; mat->uvs[0][2] *= dx3;
        dx = 1.0f - (dx-45.0f)/45.0f;
        if (dx < 0.0f) dx = 0.0f;
        else if (dx > 1.0f) dx = 1.0f;
        clr.c[3] = 233.0f * dx;
        ZOID_SetColor(gCarParts[slot].headlight[idx], &clr);
    } else MBOX_HideObject(gCarParts[slot].headlight[idx], 1, 15);

}

/* 0x8001bf08, size 0x424 */
void AnimateDownwardLights(int idx, int slot, float *backpos, float *frontpos)
{
    tCarData *car = &gGameCar[slot]; /* target forms car before color copy */
    tUVMatrix *mat; /* r29 */
    float dotx; /* f31 */
    float dx; /* f1 */
    float dx3; /* f4 */
    float delta[3]; /* target stack+56, declared before uvs */
    float surface_uvs[3][3]; /* stack+20 */
    float (*uvs)[3] = surface_uvs; /* target retained address r8 */
    float scf; /* f30 */
    float scl; /* f29 */
    tHeadlightSurface *surf; /* r0 */
    tRGBA clr = {255,255,255,243}; /* stack */

    surf = ShotToWorldCollision(backpos, frontpos, uvs, 1.0f, 1, 65535);
    if (surf) {
        scl = 0.0f; scf = 0.0f;
        mat = ZOID_FindMatrix(gCarParts[slot].headlight[idx]);
        mat->uvs[3][0] = frontpos[0]; mat->uvs[3][1] = frontpos[1]; mat->uvs[3][2] = frontpos[2];
        dotx = fabs(uvs[1][0]*car->dr_uvs[2][0] + uvs[1][1]*car->dr_uvs[2][1] + uvs[1][2]*car->dr_uvs[2][2]);
        if (dotx > 0.80000002f) { MBOX_HideObject(gCarParts[slot].headlight[idx], 1, 15); return; }
        if (dotx != 0.0f) {
        
            mat->uvs[1][0] = uvs[1][0];
            mat->uvs[1][1] = uvs[1][1];
            mat->uvs[1][2] = uvs[1][2];
            mat->uvs[0][0] = uvs[1][1] * car->dr_uvs[2][2] - uvs[1][2] * car->dr_uvs[2][1];
            mat->uvs[0][1] = uvs[1][2] * car->dr_uvs[2][0] - uvs[1][0] * car->dr_uvs[2][2];
            mat->uvs[0][2] = uvs[1][0] * car->dr_uvs[2][1] - uvs[1][1] * car->dr_uvs[2][0];
            NormalVector(mat->uvs[0]);
            mat->uvs[2][0] = mat->uvs[0][1] * mat->uvs[1][2] - mat->uvs[0][2] * mat->uvs[1][1];
            scf += 1.0f + 7.0f * (1.0f-dotx);
            mat->uvs[2][1] = mat->uvs[0][2] * mat->uvs[1][0] - mat->uvs[0][0] * mat->uvs[1][2];
            mat->uvs[2][2] = mat->uvs[0][0] * mat->uvs[1][1] - mat->uvs[0][1] * mat->uvs[1][0];
            

        } else CopyUV(uvs, mat->mat3.uvs);
        {
            delta[0] = backpos[0]-frontpos[0];
            delta[1] = backpos[1]-frontpos[1];
            delta[2] = backpos[2]-frontpos[2];
            dx = sqrtf(delta[0]*delta[0]+delta[1]*delta[1]+delta[2]*delta[2]);
        }
        dx3 = dx / 4.0f - 1.125f;
        if (dx3 < 1.0f) dx3 = 1.0f;
        scf += dx3; scl += dx3;
        mat->uvs[2][0] *= scf; mat->uvs[2][1] *= scf; mat->uvs[2][2] *= scf;
        mat->uvs[0][0] *= scl; mat->uvs[0][1] *= scl; mat->uvs[0][2] *= scl;
        dx = 1.0f - (dx-25.0f)/25.0f;
        if (dx < 0.0f) dx = 0.0f;
        else if (dx > 1.0f) dx = 1.0f;
        clr.c[3] = 243.0f * dx;
        ZOID_SetColor(gCarParts[slot].headlight[idx], &clr);
    } else MBOX_HideObject(gCarParts[slot].headlight[idx], 1, 15);

}

/* 0x8001c32c, size 0x654 */
void AnimateHeadLights(tVisual *v, signed short op)
{
    signed short slot = v->slot;
    tCarData *car = &gGameCar[slot];
    tModelData *m = &model[slot];
    float tmpb[3]; /* stack */
    float tmpf[3]; /* stack */
    float b[3]; /* stack */
    float f[3]; /* stack */
    float dx; /* f31 */
    float zero; /* target persistent fr14 */
    tRGBA clr = {255,255,255,243}; /* target copies unused initializer */

    if (!op) { v->func = 0; v->objnum = -1; return; }
    if (m->resurrect.moving_state > -1 || gInvisibleCar || (car->appearance & 16) || m->hide_car) {
        MBOX_HideObject(gCarParts[slot].headlight[0],1,15);
        MBOX_HideObject(gCarParts[slot].headlight[1],1,15);
        MBOX_HideObject(gCarParts[slot].headlight[2],1,15);
        MBOX_HideObject(gCarParts[slot].headlight[3],1,15);
        return;
    }
    MBOX_ShowObject(gCarParts[slot].headlight[0],0,15);
    MBOX_ShowObject(gCarParts[slot].headlight[1],0,15);
    MBOX_ShowObject(gCarParts[slot].headlight[2],0,15);
    MBOX_ShowObject(gCarParts[slot].headlight[3],0,15);
    if (gInvisibleCar || (car->appearance & 8)) {
        ObjInstanceList[gCarParts[slot].headlight[0]].flags ^= 0x80000000;
        ObjInstanceList[gCarParts[slot].headlight[1]].flags ^= 0x80000000;
        ObjInstanceList[gCarParts[slot].headlight[2]].flags ^= 0x80000000;
        ObjInstanceList[gCarParts[slot].headlight[3]].flags ^= 0x80000000;
    } else {
        ObjInstanceList[gCarParts[slot].headlight[1]].flags &= ~0x80000000;
        ObjInstanceList[gCarParts[slot].headlight[2]].flags &= ~0x80000000;
        ObjInstanceList[gCarParts[slot].headlight[3]].flags &= ~0x80000000;
        /* The original clears extraparts[0] here, rather than headlight[0]. */
        ObjInstanceList[gCarParts[slot].extraparts[0]].flags &= ~0x80000000;
    }
    zero = 0.0f;
    dx = m->body_type == 0 ? zero : 1.0f;
    tmpb[0] = -gHeadlightOffsets[m->body_type][0]; tmpb[1] = 5.0f; tmpb[2] = gHeadlightOffsets[m->body_type][2] - 11.0f;
    WorldVector(tmpb,b,car->dr_uvs);
    b[0] += car->dr_pos[0]; b[1] += car->dr_pos[1]; b[2] += car->dr_pos[2];
    tmpf[0] = -gHeadlightOffsets[m->body_type][0]-dx; tmpf[1] = -12.0f; tmpf[2] = 24.0f + gHeadlightOffsets[m->body_type][2];
    WorldVector(tmpf,f,car->dr_uvs);
    f[0] += car->dr_pos[0]; f[1] += car->dr_pos[1]; f[2] += car->dr_pos[2];
    AnimateDownwardLights(0,slot,b,f);
    if (m->body_type != 0) {
        tmpb[0] = gHeadlightOffsets[m->body_type][0]; WorldVector(tmpb,b,car->dr_uvs);
        b[0] += car->dr_pos[0]; b[1] += car->dr_pos[1]; b[2] += car->dr_pos[2];
        tmpf[0] = dx + gHeadlightOffsets[m->body_type][0]; WorldVector(tmpf,f,car->dr_uvs);
        f[0] += car->dr_pos[0]; f[1] += car->dr_pos[1]; f[2] += car->dr_pos[2];
        AnimateDownwardLights(1,slot,b,f);
    }
    dx = m->body_type == 0 ? zero : 13.0f;
    tmpb[0] = -gHeadlightOffsets[m->body_type][0]; tmpb[1] = 0.75f + gHeadlightOffsets[m->body_type][1]; tmpb[2] = gHeadlightOffsets[m->body_type][2];
    WorldVector(tmpb,b,car->dr_uvs);
    b[0] += car->dr_pos[0]; b[1] += car->dr_pos[1]; b[2] += car->dr_pos[2];
    tmpf[0] = -gHeadlightOffsets[m->body_type][0]-dx; tmpf[1] = 2.0f + gHeadlightOffsets[m->body_type][1]; tmpf[2] = 90.0f + gHeadlightOffsets[m->body_type][2];
    WorldVector(tmpf,f,car->dr_uvs);
    f[0] += car->dr_pos[0]; f[1] += car->dr_pos[1]; f[2] += car->dr_pos[2];
    AnimateForwardLights(2,slot,b,f);
    if (m->body_type != 0) {
        tmpb[0] = gHeadlightOffsets[m->body_type][0]; WorldVector(tmpb,b,car->dr_uvs);
        b[0] += car->dr_pos[0]; b[1] += car->dr_pos[1]; b[2] += car->dr_pos[2];
        tmpf[0] = dx + gHeadlightOffsets[m->body_type][0]; WorldVector(tmpf,f,car->dr_uvs);
        f[0] += car->dr_pos[0]; f[1] += car->dr_pos[1]; f[2] += car->dr_pos[2];
        AnimateForwardLights(3,slot,b,f);
    }

}

