/* collision.c - car collision screening and force response. Reconstructed from the GC executable.
   Original: D:\rush_ngc\Rush_NGC\GameSource\src\collision.c; assembly: build/2049LE/asm/GameSource/src/collision.s */
#include "collision_types.h"


signed char gNoCarCollisions;
signed char gSuicideMode;

/* 0x8000b7bc, size 0x94 */
void init_collision(tModelData *m)
{
    int i, j;
    for (i = 0; i < 3; i++) {
        m->centerForce[i] = 0.0f;
        m->peak_center_force[0][i] = 0.0f;
        m->peak_center_force[1][i] = 0.0f;
        m->peak_body_force[0][i] = 0.0f;
        m->peak_body_force[1][i] = 0.0f;
    }
    for (i = 0; i < 4; i++) {
        m->lasttp[i] = stree;
        for (j = 0; j < 3; j++) m->bodyForce[i][j] = 0.0f;
    }
}

/* 0x8000b850, size 0x280 */
void collision(tModelData *m)
{
    tModelData *m2; /* r27 */
    float vec[3]; /* stack */
    float pos[3]; /* stack */
    float posr[3]; /* stack */
    float dsq; /* f3 */
    int i; /* r28 */
    float (*corner)[3];
    if (gNoCarCollisions && gRace != 1) return;
    if (!m->collidable) return;
    if (gRace == 2) return;
    for (m2 = m + 1; m2 < model + 6; m2++) {
        if (!m2->in_game) continue;
        if (!m2->collidable) continue;
        vec[0] = m->reckon.rwr[0] - m2->reckon.rwr[0];
        vec[1] = m->reckon.rwr[1] - m2->reckon.rwr[1];
        vec[2] = m->reckon.rwr[2] - m2->reckon.rwr[2];
        dsq = 0.0f;
        for (i = 0; i < 3; i++) {
            dsq += vec[i] * vec[i];
        }
        if (dsq > (m->colrad + m2->colrad) * (m->colrad + m2->colrad)) continue;
        for (corner = m->bodyR; corner < m->bodyR + 4; ++corner) {
            WorldVector(*corner, pos, m->reckon.uvs);
            pos[0] += m->reckon.rwr[0];
            pos[1] += m->reckon.rwr[1];
            pos[2] += m->reckon.rwr[2];
            pos[0] -= m2->reckon.rwr[0];
            pos[1] -= m2->reckon.rwr[1];
            pos[2] -= m2->reckon.rwr[2];
            BodyVector(pos, posr, m2->reckon.uvs);
            if (PointInBody(m2, posr, 5.0f)) {
                setFBCollisionForce(m, m, m2, vec, posr, 5.0f);
                return;
            }
        }
        for (corner = m2->bodyR; corner < m2->bodyR + 4; ++corner) {
            WorldVector(*corner, pos, m2->reckon.uvs);
            pos[0] += m2->reckon.rwr[0];
            pos[1] += m2->reckon.rwr[1];
            pos[2] += m2->reckon.rwr[2];
            pos[0] -= m->reckon.rwr[0];
            pos[1] -= m->reckon.rwr[1];
            pos[2] -= m->reckon.rwr[2];
            BodyVector(pos, posr, m->reckon.uvs);
            if (PointInBody(m, posr, 5.0f)) {
                setFBCollisionForce(m, m2, m, vec, posr, 5.0f);
                return;
            }
        }
    }
}

/* 0x8000bad0, size 0x70 */
int PointInBody(tModelData *m, float *pt, float top)
{
    if (pt[0] > m->bodyR[0][0] || pt[0] < m->bodyR[3][0]) return 0;
    if (pt[1] > top || pt[1] < -1.0f) return 0;
    if (pt[2] > m->bodyR[0][2] || pt[2] < m->bodyR[3][2]) return 0;
    return 1;
}

/* 0x8000bb40, size 0x214 */
void ForceApart(tModelData *m, tModelData *m1, tModelData *m2, float *dir)
{
    int i;
    float force;
    float invdist; /* f4 */
    float temp[3]; /* stack */
    float cent[3]; /* stack */
    tModelData *other;
    float *cf, *of;
    other = m == m1 ? m2 : m1;
    force = 40000.0f;
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if (invdist < 0.0001f) {
        temp[1] = temp[2] = 0.0f;
        temp[0] = force;
    } else {
        invdist = force / _builtin_sqrtf(invdist);
        for (i = 0; i < 3; i++) temp[i] = invdist * dir[i];
    }
    BodyVector(temp, cent, m->reckon.uvs);
    cf = m->centerForce;
    cf[0] += cent[0];
    cf[1] += cent[1];
    cf[2] += cent[2];
    BodyVector(temp, cent, other->reckon.uvs);
    of = other->centerForce;
    of[0] -= cent[0];
    of[1] -= cent[1];
    of[2] -= cent[2];
}

/* 0x8000bd54, size 0x1c8 */
void MiniForceApart(tModelData *m, float *dir)
{
    int i;
    float invdist; /* f5 */
    float temp[3]; /* stack */
    float cent[3]; /* stack */
    float force;
    float *cf;
    force = m->crashflag == 0 ? 250000.0f : 100000.0f;
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if (invdist < 0.0001f) {
        temp[1] = temp[2] = 0.0f;
        temp[0] = force;
    } else {
        invdist = force / _builtin_sqrtf(invdist);
        for (i = 0; i < 3; i++) temp[i] = invdist * dir[i];
    }
    BodyVector(temp, cent, m->reckon.uvs);
    cf = m->centerForce;
    cf[0] += cent[0];
    cf[1] += cent[1];
    cf[2] += cent[2];
}

/* 0x8000bf1c, size 0x8dc */
void setFBCollisionForce(tModelData *m, tModelData *m1, tModelData *m2, float *dir, float *pos, float top)
{
    float force[3]; /* stack */
    float temp[3]; /* stack */
    float rvel[3]; /* stack */
    float cent[3]; /* stack */
    float cent2[3]; /* stack */
    float mass; /* f31 */
    float scale; /* f3 */
    int damage; /* r29 */
    int p1; /* r28 */
    int p2; /* r27 */
    int damage2; /* r26 */
    tCarData *gc1; /* r25 */
    tCarData *gc2; /* r24 */
    int beside, behind, i, hum_car;
    gc1 = &gGameCar[m1->slot];
    gc2 = &gGameCar[m2->slot];
    if (m1->slot >= 0 && m2->slot >= 0 && gRace != 6) {
        if (gc1->carMine) {
            m2->crashflag = 1;
            gc1->carMine = 0;
            m2->rwv[0] = 0.5f * m2->rwv[0];
m2->rwv[1] = 0.5f * m2->rwv[1];
m2->rwv[2] = 0.5f * m2->rwv[2];
        }
        if (gc2->carMine) {
            m1->crashflag = 1;
            gc2->carMine = 0;
            m1->rwv[0] = 0.5f * m1->rwv[0];
m1->rwv[1] = 0.5f * m1->rwv[1];
m1->rwv[2] = 0.5f * m1->rwv[2];
        }
    }
    temp[0] = m1->reckon.rwr[0] - m2->reckon.rwr[0];
temp[1] = m1->reckon.rwr[1] - m2->reckon.rwr[1];
temp[2] = m1->reckon.rwr[2] - m2->reckon.rwr[2];
    BodyVector(temp, cent, m2->reckon.uvs);
    beside = cent[2] < m2->bodyR[0][2] && cent[2] > m2->bodyR[3][2];
    behind = cent[0] < m2->bodyR[0][0] && cent[0] > m2->bodyR[3][0];
    if (gRace == 6 && gCarTeam[gc1->slot] != gCarTeam[gc2->slot]) {
        if (gDemolitionBattle && gc1->battle.weapon.type == 5 &&
            m2->resurrect.moving_state == -1 && gc1->battle.reloadingTime <= 0.0f &&
            !gc2->crash_state && gc2->battle.weapon.type == 5 &&
            m1->resurrect.moving_state == -1 && gc2->battle.reloadingTime <= 0.0f &&
            !gc1->crash_state) {
            temp[0] = m2->reckon.rwr[0] - m1->reckon.rwr[0];
temp[1] = m2->reckon.rwr[1] - m1->reckon.rwr[1];
temp[2] = m2->reckon.rwr[2] - m1->reckon.rwr[2];
            p1 = p2 = 0;
            BodyVector(temp, cent2, m1->reckon.uvs);
            if (cent2[2] > 2.25f) {
                gc1->battle.reloadingTime = 0.33333332f;
                damage = 160;
                p1 = 1;
                gc1->battle.weapon.ammoCnt--;
                damage += 7.0f * m1->magvel;
            }
            if (cent[2] > 2.25f) {
                gc2->battle.reloadingTime = 0.33333332f;
                damage2 = 160;
                p2 = 1;
                gc2->battle.weapon.ammoCnt--;
                damage2 += 7.0f * m2->magvel;
            }
            if (p1 && !p2) BattleDamage(gc1, gc2, damage);
            else if (!p1 && p2) BattleDamage(gc2, gc1, damage2);
            else if (p1 && p2) BattleDamage2(gc1, gc2, damage, damage2);
        } else if (gc1->battle.weapon.type == 5 && m2->resurrect.moving_state == -1 &&
                   gc1->battle.reloadingTime <= 0.0f && !gc2->crash_state) {
            temp[0] = m2->reckon.rwr[0] - m1->reckon.rwr[0];
temp[1] = m2->reckon.rwr[1] - m1->reckon.rwr[1];
temp[2] = m2->reckon.rwr[2] - m1->reckon.rwr[2];
            BodyVector(temp, cent2, m1->reckon.uvs);
            if (cent2[2] > 2.25f) {
                gc1->battle.reloadingTime = 0.33333332f;
                damage = 160;
                gc1->battle.weapon.ammoCnt--;
                damage += 7.0f * m1->magvel;
                BattleDamage(gc1, gc2, damage);
            }
        } else if (gc2->battle.weapon.type == 5 && m1->resurrect.moving_state == -1 &&
                   gc2->battle.reloadingTime <= 0.0f && !gc1->crash_state && cent[2] > 2.25f) {
            int damage;
            gc2->battle.reloadingTime = 0.33333332f;
            damage = 160;
            gc2->battle.weapon.ammoCnt--;
            damage += 7.0f * m2->magvel;
            BattleDamage(gc2, gc1, damage);
        }
    }
    if (beside && behind) {
        ForceApart(m, m1, m2, dir);
        return;
    }
    temp[0] = m1->reckon.rwv[0] - m2->reckon.rwv[0];
temp[1] = m1->reckon.rwv[1] - m2->reckon.rwv[1];
temp[2] = m1->reckon.rwv[2] - m2->reckon.rwv[2];
    BodyVector(temp, rvel, m2->reckon.uvs);
    if (beside) force[2] = 0.0f;
    else {
        force[2] = 2000.0f * rvel[2];
        if (pos[2] > 0.0f) {
            if (force[2] > -4000.0f) force[2] = -4000.0f;
        } else if (pos[2] < 0.0f) {
            if (force[2] < 4000.0f) force[2] = 4000.0f;
        }
    }
    if (behind) force[0] = 0.0f;
    else {
        force[0] = 2000.0f * rvel[0];
        if (pos[0] > 0.0f) {
            if (force[0] > -4000.0f) force[0] = -4000.0f;
            if (beside) {
                temp[0] = pos[0] - m2->bodyR[2][0];
                temp[0] = temp[0] * (20000.0f * _builtin_fabsf(temp[0]));
                if (force[0] > temp[0]) force[0] = temp[0];
            }
        } else if (pos[0] < 0.0f) {
            if (force[0] < 4000.0f) force[0] = 4000.0f;
            if (beside) {
                temp[0] = pos[0] - m2->bodyR[1][0];
                temp[0] = temp[0] * (20000.0f * _builtin_fabsf(temp[0]));
                if (force[0] < temp[0]) force[0] = temp[0];
            }
        }
    }
    force[1] = 100.0f * rvel[1];
    mass = 0.5f * (m1->collisionmass + m2->collisionmass);
    WorldVector(force, temp, m2->reckon.uvs);
    BodyVector(temp, cent, m1->reckon.uvs);
    scale = m2->collisionmass / mass;
    cent[0] *= scale;
cent[1] *= scale;
cent[2] *= scale;
    m1->centerForce[0] -= cent[0];
m1->centerForce[1] -= cent[1];
m1->centerForce[2] -= cent[2];
    hum_car = m1->drone_type == 2 || m2->drone_type == 2;
    if (gSuicideMode == 1 || (gSuicideMode == 2 && hum_car) ||
        magnitude(cent) > 0.5f * m1->crashthreshold) {
        if (gRace != 6 && !m1->crashflag && gc1->dead != 2) m1->crashflag = 1;
    }
    scale = m1->collisionmass / mass;
    force[0] *= scale;
force[1] *= scale;
force[2] *= scale;
    m2->centerForce[0] += force[0];
m2->centerForce[1] += force[1];
m2->centerForce[2] += force[2];
    if (gSuicideMode == 1 || (gSuicideMode == 2 && hum_car) ||
        magnitude(force) > 0.5f * m2->crashthreshold) {
        if (gRace != 6 && !m2->crashflag && gc2->dead != 2) m2->crashflag = 1;
    }
}

/* 0x8000c7f8, size 0x448 */
void setMiniFBCollisionForce(tModelData *m, tModelData *m1, tMiniModelData *m2, float *dir, float *pos)
{
    float force[3]; /* stack */
    float temp[3]; /* stack */
    float rvel[3]; /* stack */
    float cent[3]; /* stack */
    float mass; /* f31 */
    float scale; /* f3 */
    int hum_car; /* r3 */
    int beside, behind;
    int i;
    temp[0] = m1->reckon.rwr[0] - m2->pos[0];
    temp[1] = m1->reckon.rwr[1] - m2->pos[1];
    temp[2] = m1->reckon.rwr[2] - m2->pos[2];
    BodyVector(temp, cent, m2->uvs);
    beside = cent[2] < m2->bboxInfo->front && cent[2] > m2->bboxInfo->back;
    behind = cent[0] < m2->bboxInfo->right && cent[0] > m2->bboxInfo->left;
    if (beside && behind && m1->crashflag) {
        MiniForceApart(m1, dir);
        return;
    }
    temp[0] = m1->reckon.rwv[0] - m2->vel[0];
temp[1] = m1->reckon.rwv[1] - m2->vel[1];
temp[2] = m1->reckon.rwv[2] - m2->vel[2];
    BodyVector(temp, rvel, m2->uvs);
    if (beside && !behind) force[2] = 0.0f;
    else {
        force[2] = 2500.0f * rvel[2];
        if (pos[2] > 0.0f) {
            if (force[2] > -4000.0f) force[2] = -4000.0f;
            if (behind) {
                temp[2] = pos[2] - m2->bboxInfo->front;
                temp[2] = temp[2] * (10000.0f * _builtin_fabsf(temp[2]));
                if (force[2] > temp[2]) force[2] = temp[2];
            }
        } else if (pos[2] < 0.0f) {
            if (force[2] < 4000.0f) force[2] = 4000.0f;
            if (behind) {
                temp[2] = pos[2] - m2->bboxInfo->back;
                temp[2] = temp[2] * (10000.0f * _builtin_fabsf(temp[2]));
                if (force[2] < temp[2]) force[2] = temp[2];
            }
        }
    }
    if (behind && !beside) force[0] = 0.0f;
    else {
        force[0] = 2500.0f * rvel[0];
        if (pos[0] > 0.0f) {
            if (force[0] > -4000.0f) force[0] = -4000.0f;
            if (beside) {
                temp[0] = pos[0] - m2->bboxInfo->right;
                temp[0] = temp[0] * (20000.0f * _builtin_fabsf(temp[0]));
                if (force[0] > temp[0]) force[0] = temp[0];
            }
        } else if (pos[0] < 0.0f) {
            if (force[0] < 4000.0f) force[0] = 4000.0f;
            if (beside) {
                temp[0] = pos[0] - m2->bboxInfo->left;
                temp[0] = temp[0] * (20000.0f * _builtin_fabsf(temp[0]));
                if (force[0] < temp[0]) force[0] = temp[0];
            }
        }
    }
    force[1] = 100.0f * rvel[1];
    mass = 0.5f * (m1->collisionmass + m2->bboxInfo->weight);
    WorldVector(force, temp, m2->uvs);
    BodyVector(temp, cent, m1->reckon.uvs);
    scale = m2->bboxInfo->weight / mass;
    cent[0] *= scale;
cent[1] *= scale;
cent[2] *= scale;
    m1->centerForce[0] -= cent[0];
m1->centerForce[1] -= cent[1];
m1->centerForce[2] -= cent[2];
    hum_car = m1->drone_type == 2;
    if (gSuicideMode == 1 || (gSuicideMode == 2 && hum_car) ||
        magnitude(cent) > 0.80000002f * m1->crashthreshold) {
        if (gRace != 6 && !m1->crashflag && gGameCar[m1->slot].dead != 2)
            m1->crashflag = 1;
    }
}




