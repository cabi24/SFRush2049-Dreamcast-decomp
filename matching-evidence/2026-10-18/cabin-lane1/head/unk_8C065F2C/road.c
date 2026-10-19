/* road.c - per-tick road contact for the four tyres, and car placement at the start.
   Original: D:\rush_ngc\Rush_NGC\GameSource\src\road.c; assembly: build/2049LE/asm/GameSource/src/road.s */
#include "..\include\road_private.h"

/* 0x8003758c, size 0xc4 */
void make_super_speed(tModelData *m)
{
    float z; /* f1 */
    float *x, *y, *v; /* target separate component addresses and stores */
    z = invmagnitude(m->uvs[0]);
    z *= 1.00025f;
    x = &m->uvs[0][0]; *x *= z;
    y = &m->uvs[0][1]; *y *= z;
    v = &m->uvs[0][2]; *v *= z;
    z = invmagnitude(m->uvs[1]);
    z *= 1.00025f;
    x = &m->uvs[1][0]; *x *= z;
    y = &m->uvs[1][1]; *y *= z;
    v = &m->uvs[1][2]; *v *= z;
    z = invmagnitude(m->uvs[2]);
    z *= 1.00025f;
    x = &m->uvs[2][0]; *x *= z;
    y = &m->uvs[2][1]; *y *= z;
    v = &m->uvs[2][2]; *v *= z;
}

/* 0x80037650, size 0x22c */
void init_road(tModelData *m)
{
    float temp[3]; /* stack */
    int i; /* r27 */
    CopyUV(m->initin.uvs, m->uvs);
    if (gSuperSpeed) {
        make_super_speed(m);
    }
    CopyUV(m->initin.uvs, m->reckon.uvs);
    CopyUV(m->initin.uvs, gGameCar[m->slot].dr_uvs);
    WorldVector(m->initin.offset, temp, m->uvs);
    m->rwr[0] = m->initin.pos[0] + temp[0];
    m->rwr[1] = m->initin.pos[1] + temp[1];
    m->rwr[2] = m->initin.pos[2] + temp[2];
    m->reckon.rwr[0] = m->rwr[0];
    m->reckon.rwr[1] = m->rwr[1];
    m->reckon.rwr[2] = m->rwr[2];
    m->reckon.base_rwr[0] = m->rwr[0];
    m->reckon.base_rwr[1] = m->rwr[1];
    m->reckon.base_rwr[2] = m->rwr[2];
    gGameCar[m->slot].dr_pos[0] = m->rwr[0];
    gGameCar[m->slot].dr_pos[1] = m->rwr[1];
    gGameCar[m->slot].dr_pos[2] = m->rwr[2];
    for (i = 0; i < 4; i++) {
        WorldVector(m->car->tireR[i], m->tireRwr[i], m->uvs);
        m->tireRwr[i][0] += m->rwr[0];
        m->tireRwr[i][1] += m->rwr[1];
        m->tireRwr[i][2] += m->rwr[2];
        m->suscomp[i] = 0.0f;
        m->tpcomp[i] = 0.0f;
        m->roadcode[i] = m->sviscode[i] = 8;
        m->roadboost[i] = m->sound_flags[i] = 0;
        WorldVector(m->bodyR[i], m->bodyrwr[i], m->uvs);
        m->bodyrwr[i][0] += m->rwr[0];
        m->bodyrwr[i][1] += m->rwr[1];
        m->bodyrwr[i][2] += m->rwr[2];
        m->bodyrwr2[i][0] = m->bodyrwr[i][0];
        m->bodyrwr2[i][1] = m->bodyrwr[i][1];
        m->bodyrwr2[i][2] = m->bodyrwr[i][2];
    }
}

/* 0x8003787c, size 0x19c */
void road(tModelData *m)
{
    float igpos[3]; /* stack */
    float last_airdist; /* f2 */
    int i = 0; /* target zero persists across CollideCarBody */
    int j, k; /* target body-force clearing column/row loops */
    for (j = 0; j < 3; j++) {
        for (k = 0; k < 4; k++) {
            m->bodyForce[k][j] = 0.0f;
        }
    }
    CollideCarBody(m);
    /* one chained assignment: the zero register is shared with i */
    m->movingSurface = (void *)i;
    for (; i < 4; i++) {
        TireSurf(m, m->tireRwr[i], igpos, &m->roadcode[i], m->roaduvs[i], i);
        last_airdist = m->airdist[i];
        m->suscomp[i] = m->tpcomp[i] - igpos[1];
        m->airdist[i] = -m->suscomp[i];
        if (m->suscomp[i] > 3.0f) {
            m->suscomp[i] = 3.0f;
        } else if (m->suscomp[i] < -3.0f) {
            m->suscomp[i] = -3.0f;
        }
        m->tpcomp[i] = (m->suscomp[i] < 0.0f) ? 0.0f : m->suscomp[i];
        if (m->suscomp[i] < 0.0f) {
            m->roadcode[i] = m->sviscode[i] = 8;
        }
        m->airvel[i] = m->idt * (m->airdist[i] - last_airdist);
        if (m->airvel[i] < -40.0f) {
            m->airvel[i] = -40.0f;
        } else if (m->airvel[i] > 40.0f) {
            m->airvel[i] = 40.0f;
        }
    }
    collision(m);
}

