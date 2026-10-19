#include "skids_types.h"
extern volatile float gSeconds;
extern tSkidCar gGameCar[];
extern tSkidModel model[];
extern tSkidCamera gCamera[];
extern unsigned int gGameState;
extern unsigned short gTexList[];
extern float gDummyVtx[4][3];
extern tSkidSurface *surf;
extern tRGBA gShadowColor;
extern void InitStatLL(tStatList *, void *, int, int, char);
extern void *GetStatLLE(tStatList *);
extern void FreeStatLLE(tStatList *, void *);
extern void ZOID_RemovePoly(tPoly *);
extern void ZOID_UpdatePoly(tPoly *, int, float (*)[3], unsigned short, tRGBA *, unsigned short, int);
extern tPoly *ZOID_NewPoly(int, float (*)[3], unsigned short, tRGBA *, unsigned short, int);
extern void direction(float *,float *);
extern void *memset(void *,int,unsigned int);
extern float _builtin_sqrtf(float);
#define sqrtf(x) _builtin_sqrtf(x)

void _UpdateSkid(tNewSkid *, signed short, unsigned int, tRGBA *);
tSkid *_GetSkid(tRGBA *);
void _StartSkid(tNewSkid *, signed short, unsigned int, tRGBA *);
void _StopSkid(tNewSkid *);
void _ContinueSkid(tNewSkid *, signed short, unsigned int, tRGBA *);

tRGBA gDirtColor = { 80, 56, 24, 192 };
static tStatList gSkidList;
static tNewSkid gNewSkid[6][4];
static tSkid gSkidArray[100];

static void _ReleaseSkid(tSkid *s);
#pragma inline(_ReleaseSkid)
static void _ReleaseSkid(tSkid *s)
{
    ZOID_RemovePoly(s->poly);
    FreeStatLLE(&gSkidList, s);
}

void _UpdateSkid(tNewSkid *ns, signed short slot, unsigned int tire, tRGBA *color)
{
    tSkidCar *car = &gGameCar[slot];
    tSkidModel *m = &model[slot];
    tSkid *s = ns->skid;
    signed short i;
    float orth[3];
    float dir[3];
    float wid;
    orth[0] = car->dr_tirepos[tire | 1][0] - car->dr_tirepos[tire & 2][0];
    orth[1] = car->dr_tirepos[tire | 1][1] - car->dr_tirepos[tire & 2][1];
    orth[2] = car->dr_tirepos[tire | 1][2] - car->dr_tirepos[tire & 2][2];
    orth[1] += m->reckon.suscomp[tire | 1] - m->reckon.suscomp[tire & 2];
    direction(orth, dir);
    for (i = 0; i < 3; i++) {
        wid = 0.75f * dir[i];
        ns->end[i] = car->dr_tirepos[tire][i];
        ns->vert[2][i] = ns->end[i] - wid;
        ns->vert[3][i] = ns->end[i] + wid;
    }
    ns->vert[2][1] += 0.5f;
    ns->vert[3][1] += 0.5f;
    s->lastTime = gSeconds;
    s->color.c[0] = color->c[0];
    s->color.c[1] = color->c[1];
    s->color.c[2] = color->c[2];
    s->color.c[3] = 192;
    ZOID_UpdatePoly(s->poly, 4, ns->vert, 0, &s->color, 0, 0);
}

tSkid *_GetSkid(tRGBA *color)
{
    unsigned int i;
    tSkid *s;
    tSkid *fars;
    float d;
    float dsq;
    float fardsq;
    s = GetStatLLE(&gSkidList);
    if (!s) {
        fardsq = -1.0f;
        for (s = fars = gSkidList.head; s->ll.next; s = s->ll.next) {
            dsq = 0.0f;
            for (i = 0; i < 3; i++) {
                d = s->pos[i] - gCamera[0].pos[i];
                dsq += d * d;
            }
            if (dsq > fardsq) {
                fardsq = dsq;
                fars = s;
            }
        }
        _ReleaseSkid(fars);
        s = GetStatLLE(&gSkidList);
    }
    s->active = 0;
    s->color.c[0] = color->c[2];
    s->color.c[1] = color->c[1];
    s->color.c[2] = color->c[0];
    s->color.c[3] = 192;
    s->poly = ZOID_NewPoly(4, gDummyVtx, gTexList[10], color,
                          0x1200 | ((gGameState & 0x100) ? 1 : 15), 1);
    if (!s->poly) {
        FreeStatLLE(&gSkidList, s);
        return 0;
    }
    return s;
}

void _StartSkid(tNewSkid *ns, signed short slot, unsigned int tire, tRGBA *color)
{
    tSkid *s;
    s = _GetSkid(color);
    if (s) {
        ns->skid = s;
        ns->lensq = 0.0f;
        _UpdateSkid(ns, slot, tire, color);
        ns->start[0] = ns->end[0];
        ns->start[1] = ns->end[1];
        ns->start[2] = ns->end[2];
        ns->vert[0][0] = ns->vert[3][0];
        ns->vert[0][1] = ns->vert[3][1];
        ns->vert[0][2] = ns->vert[3][2];
        ns->vert[1][0] = ns->vert[2][0];
        ns->vert[1][1] = ns->vert[2][1];
        ns->vert[1][2] = ns->vert[2][2];
        ZOID_UpdatePoly(s->poly, 4, ns->vert, 0, 0, 0, 0);
    }
}

void _StopSkid(tNewSkid *ns)
{
    tSkid *s = ns->skid;
    unsigned int i;
    for (i=0;i<3;i++) s->pos[i] = 0.5f * (ns->start[i] + ns->end[i]);
    s->active = 1;
    ns->skid = 0;
}

void _ContinueSkid(tNewSkid *ns, signed short slot, unsigned int tire, tRGBA *color)
{
    float lensq;
    float invlen;
    float d[3];
    float v[3];
    float len;
    _UpdateSkid(ns, slot, tire, color);
    d[0] = gGameCar[slot].dr_tirepos[tire][0] - ns->start[0];
    d[1] = gGameCar[slot].dr_tirepos[tire][1] - ns->start[1];
    d[2] = gGameCar[slot].dr_tirepos[tire][2] - ns->start[2];
    lensq = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
    if (ns->lensq > 0.0f) {
        len = sqrtf(lensq);
        v[0] = ns->dir[0] * len;
        v[1] = ns->dir[1] * len;
        v[2] = ns->dir[2] * len;
        d[0] = ns->end[0] - v[0];
        d[1] = ns->end[1] - v[1];
        d[2] = ns->end[2] - v[2];
        len = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
        if (len > 1.0f || lensq < ns->lensq) {
            _StopSkid(ns);
            _StartSkid(ns, slot, tire, color);
        }
    } else if (lensq > 0.100000003f) {
        invlen = 1.0f / sqrtf(lensq);
        ns->dir[0] = d[0] * invlen;
        ns->dir[1] = d[1] * invlen;
        ns->dir[2] = d[2] * invlen;
        ns->lensq = lensq;
    }
}

void func_8C04EE60(void)
{
    tSkid *s;
    while (gSkidList.head) {
        s = gSkidList.head;
        ZOID_RemovePoly(s->poly);
        FreeStatLLE(&gSkidList, s);
    }
    InitStatLL(&gSkidList, gSkidArray, sizeof(tSkid), 100, 1);
    memset(gNewSkid, 0, sizeof(gNewSkid));
}

void UpdateSkids(int slot)
{
    static int appFlags[4] = { 4, 2, 0x400, 0x200 };
    int tire;
    int laston;
    tSkidModel *m = &model[slot];
    tNewSkid *ns;
    tRGBA *color;
    ns = gNewSkid[slot];
    for (tire = 0; tire < 4; tire++, ns++) {
        int on = ((gGameCar[slot].appearance & appFlags[tire]) &&
                  m->resurrect.moving_state == -1 && m->sviscode[tire] != 8) &&
                 !(surf[m->tiresli[tire]].snd_flags & 0x30);
        laston = ns->skid != 0;
        if (m->sviscode[tire] == 0) {
            color = &gShadowColor;
        } else {
            color = &gDirtColor;
        }
        if (!laston && on) _StartSkid(ns, slot, tire, color);
        else if (laston && on) _ContinueSkid(ns, slot, tire, color);
        else if (laston && !on) _StopSkid(ns);
    }
}

void AnimateSkids(void)
{
    tSkid *s;
    tSkid *nexts;
    float t;
    float fadetime;
    unsigned int xlu;
    int numfree;
    numfree = 0;
    for (s = gSkidList.store; s; s = s->ll.next) numfree++;
    fadetime = 2.0f + 2.0f * numfree / 100.0f;
    s = gSkidList.head;
    goto skid_test;
    skid_loop:
    {
        if (s->active) {
            t = (gSeconds - s->lastTime) / fadetime;
            xlu = 255.0f * t + 63.0f;
            if (xlu < 255) {
                s->color.c[3] = 255 - xlu;
                if (s->color.c[3] > 192) s->color.c[3] = 192;
                ZOID_UpdatePoly(s->poly, 4, 0, 0, &s->color, 0, 0);
                s = s->ll.next;
            } else {
                nexts = s->ll.next;
                ZOID_RemovePoly(s->poly);
                FreeStatLLE(&gSkidList, s);
                s = nexts;
            }
        } else {
            s = s->ll.next;
        }
    }
    skid_test:
    if (s) goto skid_loop;
}
