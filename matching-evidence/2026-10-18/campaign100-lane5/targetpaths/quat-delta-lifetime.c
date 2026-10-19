/* Dreamcast target-path candidate from MAT3 semantics and DC target evidence.
   Match claims are limited to the private fixed-address receipt. */
#include "..\include\targetpaths_private.h"

void _PlacePathTargetPos(tTarget *t);
void _PlacePathTargetQuat(tTarget *t);
void _PlaceGhostRacePath(tTarget *t);
void _ResetPath(tTarget *t, int pt);
void _PlayStartSound(tTarget *t);
void _PlayStopSound(tTarget *t);
void _PlayMoveSound(tTarget *t);

/* Runtime pointer table/count remain external; unused MAT3 gTargetPaths has no DC storage proof here. */

void func_8C056058(void)
{
    int i;
    for (i = 0; i < 30; i++) CollisionTargetPtrs[i] = 0;
    numCollisionTargetPtrs = 0;
}

void GetTargetVelocity(int collIdx, float *vel)
{
    tBasicTargetPath *pPtr;
    tTargetStructPath *dPtr;
    int i;
    vel[0] = 0.0f;
    vel[1] = 0.0f;
    vel[2] = 0.0f;
    for (i = 0; i < numCollisionTargetPtrs; i++) {
        dPtr = CollisionTargetPtrs[i]->data;
        pPtr = dPtr->pathData;
        if (pPtr->collIdx == collIdx) {
            if (dPtr->flags & 8) {
                vel[0] = -(pPtr->pts[dPtr->ptOn].norm[0] * dPtr->curVel);
                vel[1] = -(pPtr->pts[dPtr->ptOn].norm[1] * dPtr->curVel);
                vel[2] = -(pPtr->pts[dPtr->ptOn].norm[2] * dPtr->curVel);
            } else {
                vel[0] = pPtr->pts[dPtr->ptOn].norm[0] * dPtr->curVel;
                vel[1] = pPtr->pts[dPtr->ptOn].norm[1] * dPtr->curVel;
                vel[2] = pPtr->pts[dPtr->ptOn].norm[2] * dPtr->curVel;
            }
            return;
        }
    }
}

void _PlacePathTargetPos(tTarget *t)
{
    float v1[3];
    float delta[3];
    float dist;
    float dT;
    float dV;
    int p0;
    int p1;
    int i;
    tTargetStructPath *dPtr;
    tBasicTargetPath *pPtr;
    anon_pos_norm_44 *pt;
    dPtr = t->data;
    pPtr = dPtr->pathData;
    p0 = dPtr->ptOn;
    pt = &pPtr->pts[p0];
    if (p0 >= pPtr->numPts - 1 && !(pPtr->flags & 2) && (pPtr->flags & 0x80)) return;
    if (pt->flags & 0x10000008) {
        if (dPtr->flags & 8) dT = pt->timeToNext - dPtr->timeElapsed;
        else dT = dPtr->timeElapsed;
        dist = pt->distToNext * (dT / pt->timeToNext);
    } else {
        p1 = p0 + 1;
        if (p1 >= pPtr->numPts && (pPtr->flags & 2)) p1 = 0;
        dV = pPtr->pts[p1].speed - pt->speed;
        if (dPtr->flags & 8) dT = pt->timeToNext - dPtr->timeElapsed;
        else dT = dPtr->timeElapsed;
        dV = (dV * dT * dT) / (2.0f * pt->timeToNext);
        /* Retain the source comparison visible at target5625E. */
        if (pt->speed > pPtr->pts[p1].speed) dist = pt->speed * dT + dV;
        else dist = pt->speed * dT + dV;
    }
    v1[0] = pt->norm[0];
    v1[1] = pt->norm[1];
    v1[2] = pt->norm[2];
    dPtr->lastPos[0] = t->mat.mat3.pos[0];
    dPtr->lastPos[1] = t->mat.mat3.pos[1];
    dPtr->lastPos[2] = t->mat.mat3.pos[2];
    v1[0] *= dist;
    v1[1] *= dist;
    v1[2] *= dist;
    for (i = 0; i < 3; i++) v1[i] = (int)(v1[i] * 32.0f) * 0.03125f;
    t->mat.mat3.pos[0] = pt->pos[0] + v1[0];
    t->mat.mat3.pos[1] = pt->pos[1] + v1[1];
    t->mat.mat3.pos[2] = pt->pos[2] + v1[2];
    v1[0] = t->mat.mat3.pos[0] - dPtr->lastPos[0];
    v1[1] = t->mat.mat3.pos[1] - dPtr->lastPos[1];
    v1[2] = t->mat.mat3.pos[2] - dPtr->lastPos[2];
    dPtr->curVel = sqrtf(v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2]) / gFrameSeconds;
    if (pPtr->flags & 0x20) {
        int move = 1;
        if (gRace == kObstacle) {
            delta[0] = t->mat.mat3.pos[0] - gGameCar[0].dr_pos[0];
            delta[1] = t->mat.mat3.pos[1] - gGameCar[0].dr_pos[1];
            delta[2] = t->mat.mat3.pos[2] - gGameCar[0].dr_pos[2];
            if (delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2] > 202500.0f) move = 0;
        }
        if (move) MoveCollisionObject(pPtr->collIdx, v1);
    }
}

void _PlacePathTargetQuat(tTarget *t)
{
    float delta[3];
    float nquat[4];
    float frac;
    float dist2;
    int p0;
    int p1;
    int i;
    tBasicTargetPath *pPtr;
    tTargetStructPath *dPtr;
    anon_pos_norm_44 *pt;
    pPtr = ((tTargetStructPath *)t->data)->pathData;
    dPtr = t->data;
    p0 = dPtr->ptOn;
    pt = &pPtr->pts[p0];
    if (pt->flags & 0x20) goto directQuat;
    if (dPtr->flags & 8) frac = pt->timeToNext - dPtr->timeElapsed;
    else frac = dPtr->timeElapsed;
    dist2 = pt->timeToNext;
    if (dist2 == 0.0f) {
directQuat:
        CreateQuatMat((float *)t->mat.mat3.uvs, pt->quat, 1);
    } else {
        p1 = p0 + 1;
        if (p1 >= pPtr->numPts && (pPtr->flags & 2)) p1 = 0;
        frac /= dist2;
        InterpQuats(frac, pt->quat, pPtr->pts[p1].quat, nquat);
        CreateQuatMat((float *)t->mat.mat3.uvs, nquat, 1);
    }
    if (pPtr->flags & 0x20) {
        int move = 1;
        if (gRace == kObstacle) {
            delta[0] = t->mat.mat3.pos[0] - gGameCar[0].dr_pos[0];
            delta[1] = t->mat.mat3.pos[1] - gGameCar[0].dr_pos[1];
            delta[2] = t->mat.mat3.pos[2] - gGameCar[0].dr_pos[2];
            dist2 = delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];
            if (dist2 > 22500.0f) move = 0;
        }
        if (move) RotateCollisionObject(pPtr->collIdx, t->mat.mat3.pos, dPtr->lastUVS, t->mat.mat3.uvs);
    }
}

void _PlaceGhostRacePath(tTarget *t)
{
    tTargetStructPath *dPtr;
    tBasicTargetPath *pPtr;
    anon_pos_norm_44 *pt;
    int i;
    float origin[3];
    float v1[3];
    float ouvs[3][3];
    dPtr = t->data;
    pPtr = dPtr->pathData;
    for (i = 0; i < pPtr->numPts; i++) {
        pt = &pPtr->pts[i];
        if (pt->flags & 0x01000000) {
            t->mat.mat3.pos[0] = pt->pos[0];
            t->mat.mat3.pos[1] = pt->pos[1];
            t->mat.mat3.pos[2] = pt->pos[2];
            CreateQuatMat((float *)t->mat.mat3.uvs, pt->quat, 1);
        }
    }
    if (pPtr->flags & 0x20) {
        origin[0] = pPtr->pts[0].pos[0];
        origin[1] = pPtr->pts[0].pos[1];
        origin[2] = pPtr->pts[0].pos[2];
        CreateQuatMat((float *)ouvs, pPtr->pts[0].quat, 1);
        TransposeUV(ouvs);
        v1[0] = t->mat.mat3.pos[0] - origin[0];
        v1[1] = t->mat.mat3.pos[1] - origin[1];
        v1[2] = t->mat.mat3.pos[2] - origin[2];
        MoveCollisionObject(pPtr->collIdx, v1);
        RotateCollisionObject(pPtr->collIdx, t->mat.mat3.pos, ouvs, t->mat.mat3.uvs);
    }
}

void _ResetPath(tTarget *t, int pt)
{
    int i;
    tBasicTargetPath *pPtr;
    tTargetStructPath *dPtr;
    dPtr = t->data;
    pPtr = dPtr->pathData;
    dPtr->flags = ((pPtr->flags & 1) && pt >= pPtr->numPts - 1) ? 8 : 4;
    dPtr->timeElapsed = 0.0f;
    dPtr->ptOn = pt;
    dPtr->curVel = 0.0f;
    t->mat.mat3.pos[0] = pPtr->pts[dPtr->ptOn].pos[0];
    t->mat.mat3.pos[1] = pPtr->pts[dPtr->ptOn].pos[1];
    t->mat.mat3.pos[2] = pPtr->pts[dPtr->ptOn].pos[2];
    dPtr->lastPos[0] = pPtr->pts[dPtr->ptOn].pos[0];
    dPtr->lastPos[1] = pPtr->pts[dPtr->ptOn].pos[1];
    dPtr->lastPos[2] = pPtr->pts[dPtr->ptOn].pos[2];
    if (pPtr->flags & 0xc0) {
        pPtr->flags |= 0x100;
        pPtr->parent = dPtr;
    }
    if (pPtr->flags & 0x5000) pPtr->flags |= 0x400;
    CreateQuatMat((float *)t->mat.mat3.uvs, pPtr->pts[dPtr->ptOn].quat, 1);
    CopyUV(t->mat.mat3.uvs, dPtr->lastUVS);
    TransposeUV(dPtr->lastUVS);
    for (i = 0; i < 3; i++) {
        t->mat.mat3.uvs[0][i] *= pPtr->pts[dPtr->ptOn].scale[0];
        t->mat.mat3.uvs[1][i] *= pPtr->pts[dPtr->ptOn].scale[1];
        t->mat.mat3.uvs[2][i] *= pPtr->pts[dPtr->ptOn].scale[2];
    }
}

void _StartPathTarget(tTarget *t)
{
    tRGBA clr = {255,0,0,255};
    tVisual *v;
    float max;
    tTargetInfo *tInfo;
    tBasicTargetPath *pPtr;
    tTargetStructPath *dPtr;
    int i;
    t->sndState = 2;
    tInfo = &targetInfo[t->infoIdx];
    dPtr = t->data;
    pPtr = dPtr->pathData;
    if (t->Flags & 1) return;
    dPtr->timeElapsed = 0.0f;
    dPtr->flags = 0;
    if (gRace == kGhost && pPtr->collIdx > 0) {
        _PlaceGhostRacePath(t);
    } else {
        if (pPtr->flags & 0x20) {
            CollisionTargetPtrs[numCollisionTargetPtrs] = t;
            numCollisionTargetPtrs++;
        }
        max = 0.0f;
        for (i = 0; i < pPtr->numPts; i++) {
            if (pPtr->pts[i].speed > max) max = pPtr->pts[i].speed;
        }
        dPtr->maxVel = max;
        for (i = 0; i < pPtr->numPts; i++) {
            if ((pPtr->pts[i].flags & 1) && !(pPtr->pts[i].flags & 0x1000)) {
                _ResetPath(t, i);
                pPtr->pts[i].flags |= 0x1000;
                i = pPtr->numPts;
            }
        }
        if (tInfo->objListIdx == 676) t->frames = 4;
        else if (tInfo->objListIdx == 680) t->frames = 20;
        if ((pPtr->flags & 0x40) && !(pPtr->flags & 0x4000)) {
            pPtr->flags &= ~0x00100000;
            pPtr->flags |= 0x00200400;
            if (!(pPtr->flags & 0x8000)) ZOID_SetObjectDef(t->dlIdx, gObjList[663]);
        }
        v = GrabEnvEntry();
        if (v) {
            v->index = 0;
            v->func = tInfo->animFunc;
            v->data = (unsigned int)t;
            if (tInfo->objListIdx == 676) v->timeStamp = 0.25f;
            else if (tInfo->objListIdx == 680) v->timeStamp = 0.042500001f;
            AddToEnvList(v);
        }
    }
    t->Flags |= 1;
}

void _AnimatePathTarget(tVisual *v, signed short op)
{
    tTarget *t;
    tBasicTargetPath *pPtr;
    tBasicTargetPath *pPtr2;
    tTargetStructPath *dPtr;
    tTargetStructPath *dPtr2;
    int done;
    tTargetInfo *tInfo;
    signed short type;
    signed char moving;
    float dist;
    int pN;
    moving = 0;
    if (!op) goto release;
    if (!(gGameState & 0x007c0000) && !(gGameState & 8)) return;
    if (gPauseState != kNoPause) return;
    t = (tTarget *)v->data;
    dPtr = t->data;
    pPtr = dPtr->pathData;
    if (pPtr->flags & 0x40) {
        if (pPtr->flags & 0x4000) {
            if (!(pPtr->flags & 0x200)) {
                if (!(pPtr->flags & 0x400)) {
                    if (dPtr->flags & 4) {
                        dPtr->flags &= ~4;
                        dPtr->flags |= 8;
                        dPtr->timeElapsed = pPtr->pts[dPtr->ptOn].timeToNext - dPtr->timeElapsed;
                    }
                    if (pPtr->flags & 0x100) _PlayStartSound(t);
                    pPtr->flags &= ~0x100;
                }
            } else {
                if (pPtr->flags & 0x400) {
                    pPtr->flags &= ~0x500;
                    _PlayStartSound(t);
                }
                pPtr->flags &= ~0x200;
            }
        } else {
            if ((pPtr->flags & 0x2000) && (pPtr->flags & 0x100000) && (((tBasicTargetPath *)pPtr->trigPtr)->flags & 0x100)) {
                pPtr->flags &= ~0x100000;
                pPtr->flags |= 0x200400;
                if (!(pPtr->flags & 0x8000)) ZOID_SetObjectDef(t->dlIdx, gObjList[663]);
                _PlayStopSound(t);
                return;
            }
            if ((pPtr->flags & 0x400) && (pPtr->flags & 0x200)) {
                pPtr2 = pPtr->trigPtr;
                if (pPtr2->flags & 0x100) pPtr2->flags &= ~0x100;
                else if (pPtr->flags & 0x1000) {
                    dPtr2 = pPtr2->parent;
                    if (dPtr2->flags & 8) {
                        dPtr2->flags &= ~8;
                        dPtr2->flags |= 4;
                        dPtr2->timeElapsed = pPtr2->pts[dPtr2->ptOn].timeToNext - dPtr2->timeElapsed;
                    } else {
                        dPtr2->flags &= ~4;
                        dPtr2->flags |= 8;
                        dPtr2->timeElapsed = pPtr2->pts[dPtr2->ptOn].timeToNext - dPtr2->timeElapsed;
                    }
                }
                pPtr->flags &= ~0x700;
                if (pPtr->flags & 0x1000) {
                    if (pPtr->flags & 0x200000) {
                        pPtr->flags &= ~0x200000;
                        pPtr->flags |= 0x100000;
                        if (!(pPtr->flags & 0x8000)) ZOID_SetObjectDef(t->dlIdx, gObjList[662]);
                        _PlayStartSound(t);
                    } else {
                        pPtr->flags &= ~0x100000;
                        pPtr->flags |= 0x200000;
                        if (!(pPtr->flags & 0x8000)) ZOID_SetObjectDef(t->dlIdx, gObjList[663]);
                        _PlayStopSound(t);
                    }
                } else if (pPtr->flags & 0x200000) {
                    pPtr->flags &= ~0x200000;
                    pPtr->flags |= 0x100000;
                    if (!(pPtr->flags & 0x8000)) ZOID_SetObjectDef(t->dlIdx, gObjList[662]);
                    _PlayStartSound(t);
                }
            }
        }
    }
    if (pPtr->flags & 0x100) return;
    dPtr->timeElapsed += gFrameSeconds;
    done = 0;
    while (!done) {
        if (dPtr->timeElapsed >= pPtr->pts[dPtr->ptOn].timeToNext) {
            dPtr->timeElapsed -= pPtr->pts[dPtr->ptOn].timeToNext;
            if (dPtr->flags & 8) {
                if (dPtr->ptOn == 0) {
                    if (pPtr->flags & 0x4000) {
                        _ResetPath(t, 0);
                        done = 1;
                        _PlayStopSound(t);
                    } else {
                        dPtr->flags &= ~8;
                        dPtr->flags |= 4;
                        if (pPtr->flags & 0x80) {
                            pPtr->flags |= 0x100;
                            done = 1;
                            dPtr->timeElapsed = 0.0f;
                        }
                    }
                    if (pPtr->flags & 0x20) ResetCollisionObject(pPtr->collIdx);
                } else dPtr->ptOn--;
            } else {
                if (dPtr->ptOn == pPtr->numPts - 1) {
                    if (pPtr->flags & 0x80) {
                        dPtr->flags &= ~4;
                        dPtr->flags |= 8;
                        dPtr->ptOn--;
                    } else {
                        dPtr->ptOn = 0;
                        if (pPtr->flags & 0x20) ResetCollisionObject(pPtr->collIdx);
                    }
                } else {
                    dPtr->ptOn++;
                    if (pPtr->pts[dPtr->ptOn].flags & 0x40) {
                        pPtr->flags |= 0x100;
                        done = 1;
                        dPtr->timeElapsed = 0.0f;
                    } else if (dPtr->ptOn == pPtr->numPts - 1) {
                        if (pPtr->flags & 1) {
                            dPtr->flags &= ~4;
                            dPtr->flags |= 8;
                            dPtr->ptOn--;
                        } else if (pPtr->flags & 0x4000) {
                            dPtr->flags &= ~4;
                            dPtr->flags |= 8;
                            dPtr->ptOn--;
                            dPtr->timeElapsed = 0.0f;
                            pPtr->flags |= 0x100;
                        } else if (!(pPtr->flags & 2)) {
                            _ResetPath(t, 0);
                            if (pPtr->flags & 0x20) ResetCollisionObject(pPtr->collIdx);
                        }
                    }
                }
            }
        } else done = 1;
    }
    if (!(pPtr->pts[dPtr->ptOn].flags & 2)) {
        _PlacePathTargetPos(t);
        moving = 1;
    }
    if (!(pPtr->pts[dPtr->ptOn].flags & 0x10) || !(pPtr->pts[dPtr->ptOn].flags & 4)) {
        _PlacePathTargetQuat(t);
        moving = 1;
    }
    if (moving) {
        if (pPtr->flags & 0x100) _PlayStopSound(t);
        else _PlayMoveSound(t);
    } else _PlayStopSound(t);
    tInfo = &targetInfo[t->infoIdx];
    if (tInfo->objListIdx != -1) {
        v->timeStamp -= gFrameSeconds;
        if (v->timeStamp <= 0.0f) {
            if (tInfo->objListIdx == 676) v->timeStamp = 0.25f;
            else if (tInfo->objListIdx == 680) v->timeStamp = 0.042500001f;
        } else return;
        v->index++;
        if (v->index >= t->frames) v->index = 0;
        if (t->type != (type = t->otype + v->index)) {
            if (!(pPtr->flags & 0x8000)) ZOID_SetObjectDef(t->dlIdx, gObjList[type]);
            t->type = type;
        }
    }
    if (tInfo->collFuncIdx == 4) {
        float vel[3];
        if (dPtr->flags & 8) {
            vel[0] = -(pPtr->pts[dPtr->ptOn].norm[0] * dPtr->curVel);
            vel[1] = -(pPtr->pts[dPtr->ptOn].norm[1] * dPtr->curVel);
            vel[2] = -(pPtr->pts[dPtr->ptOn].norm[2] * dPtr->curVel);
        } else {
            vel[0] = pPtr->pts[dPtr->ptOn].norm[0] * dPtr->curVel;
            vel[1] = pPtr->pts[dPtr->ptOn].norm[1] * dPtr->curVel;
            vel[2] = pPtr->pts[dPtr->ptOn].norm[2] * dPtr->curVel;
        }
        UpdateMiniModelStruct(t->owner, vel, t->mat.mat3.pos, t->mat.mat3.uvs);
    }
    if (!(pPtr->pts[dPtr->ptOn].flags & 0x10)) {
        float sc[3];
        pN = dPtr->ptOn + 1;
        if (pN >= pPtr->numPts) pN = 0;
        dist = ((dPtr->flags & 8) ? pPtr->pts[dPtr->ptOn].timeToNext - dPtr->timeElapsed : dPtr->timeElapsed) / pPtr->pts[dPtr->ptOn].timeToNext;
        {
            int i;
            for (i = 0; i < 3; i++) sc[i] = pPtr->pts[dPtr->ptOn].scale[i] + dist * (pPtr->pts[pN].scale[i] - pPtr->pts[dPtr->ptOn].scale[i]);
        }
        {
            int i;
            for (i = 0; i < 3; i++) {
                t->mat.mat3.uvs[0][i] *= sc[0];
                t->mat.mat3.uvs[1][i] *= sc[1];
                t->mat.mat3.uvs[2][i] *= sc[2];
            }
        }
    }
    if (!(t->Flags & 2)) {
        if (!(pPtr->flags & 0x8000)) ZOID_RemoveObject(t->dlIdx);
        FreeStatLLE(&gTargetList, t);
release:
        ReleaseEnviron(v, 1);
    }
}

void _PlayStartSound(tTarget *t)
{
    AudStopEmitter(t->sound);
    if (targetInfo[t->infoIdx].startSnd != -1)
        t->sound = EmitGameSound(t->mat.mat3.pos, unitmatrix[3], targetInfo[t->infoIdx].fadeDist,
            0.0f, 1.0f, 0.0f, targetInfo[t->infoIdx].startSnd, 0, targetInfo[t->infoIdx].replace, 128);
    else t->sound = -1;
    t->sndState = 0;
}

void _PlayStopSound(tTarget *t)
{
    if (t->sndState == 2) return;
    if (t->sndState == 0 && AudEmitterPlaying(t->sound)) return;
    {
        AudStopEmitter(t->sound);
        if (targetInfo[t->infoIdx].stopSnd != -1)
            t->sound = EmitGameSound(t->mat.mat3.pos, unitmatrix[3], targetInfo[t->infoIdx].fadeDist,
                0.0f, 1.0f, 0.0f, targetInfo[t->infoIdx].stopSnd, 0, targetInfo[t->infoIdx].replace, 128);
        else t->sound = -1;
        t->sndState = 2;
    }
}

void _PlayMoveSound(tTarget *t)
{
    tTargetStructPath *dPtr;
    float ratio;
    int maxVel;
    int curVel;
    if (t->sndState == 1) {
        if (t->sound != -1) {
            if (targetInfo[t->infoIdx].moveSnd == 53) {
                dPtr = t->data;
                maxVel = dPtr->maxVel;
                curVel = dPtr->curVel;
                if (!maxVel) {
                    if (curVel) maxVel = curVel;
                    else maxVel = 1;
                }
                if ((maxVel < 0 && curVel > 0) || (maxVel > 0 && curVel < 0)) ;
                ratio = (float)path_abs(curVel) / (float)path_abs(maxVel);
                ratio = ratio / 2.0f + 0.5f;
                if (ratio < 0.0f || ratio > 1.0f) {
                    if ((int)ratio < 0 || (int)ratio > 1) ;
                    if (ratio < 0.0f) ratio = 0.0f;
                    else ratio = 1.0f;
                }
            } else ratio = 1.0f;
            if (targetInfo[t->infoIdx].moveSnd == 83)
                AudUpdateEmitter(t->sound, t->mat.mat3.pos, unitmatrix[3], 0.80000002f, 1.0f);
            else if (targetInfo[t->infoIdx].moveSnd == 140)
                AudUpdateEmitter(t->sound, t->mat.mat3.pos, unitmatrix[3], 1.0f, 0.75f);
            else AudUpdateEmitter(t->sound, t->mat.mat3.pos, unitmatrix[3], ratio * 0.75f + 0.25f, ratio);
        }
    } else if (t->sndState == 2) _PlayStartSound(t);
    else if (!AudEmitterPlaying(t->sound)) {
        AudStopEmitter(t->sound);
        if (targetInfo[t->infoIdx].moveSnd != -1) {
            t->sound = EmitGameSound(t->mat.mat3.pos, unitmatrix[3], targetInfo[t->infoIdx].fadeDist,
                0.0f, 1.0f, 0.0f, targetInfo[t->infoIdx].moveSnd, 0, targetInfo[t->infoIdx].replace, 128);
            if (targetInfo[t->infoIdx].moveSnd == 53)
                AudUpdateEmitter(t->sound, t->mat.mat3.pos, unitmatrix[3], 0.0f, 1.0f);
        } else t->sound = -1;
        t->sndState = 1;
    }
}

void PauseTargetPaths(tTarget *t)
{
    t->sndState = 2;
    t->sound = -1;
}
