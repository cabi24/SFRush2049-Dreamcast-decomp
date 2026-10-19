/* worlddam.c - scorch-mark decals left on the road (a ring of 25 polygons).
   Original: D:\rush_ngc\Rush_NGC\GameSource\src\worlddam.c; assembly: build/2049LE/asm/GameSource/src/worlddam.s */
#include "..\include\worlddam_private.h"

static float dummyVtx[4][3] = { 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f };
static tRGBA damColor[5] = {
    255, 255, 255, 255, 255, 255, 255, 51, 255, 255, 255, 102, 255, 255, 255, 153, 255, 255, 255,
    204
};
/* Runtime ring/texture storage stays external; allocation boundary unproven. */

int findDamageSlot(void);

/* newWorldDamage gives up if any edge of the decal is too steep or too long */
#define EDGE_CHECK(a,b) { \
    delta[0]=(a)[0]-(b)[0]; \
    delta[1]=(a)[1]-(b)[1]; \
    delta[2]=(a)[2]-(b)[2]; \
    if (fabs(delta[1])>3.0f) return; \
    edge_length2=delta[0]*delta[0]+delta[1]*delta[1]+delta[2]*delta[2]; \
    if (edge_length2>200.0f) return; \
}

/* 0x8005e7c0, size 0xc0 */
void initWorldDamage(void)
{
    int i; /* r0 */
    DamagePolys.full = 0;
    DamagePolys.oldest = 0;
    MBOX_FindTexture_WORK("SCORCH_MARK", &damTexture, 0, gNumModels - 1, 1);
    for (i = 0; i < 25; i++) {
        DamagePolys.polys[i] = 0;
    }
}

/* 0x8005e880, size 0x15c */
int findDamageSlot(void)
{
    int i; /* r30 */
    int j; /* r28 */
    int fade; /* r3 */
    if (DamagePolys.full) {
        ZOID_HidePoly(DamagePolys.polys[DamagePolys.oldest]);
        i = DamagePolys.oldest;
        ++DamagePolys.oldest;
        if (DamagePolys.oldest >= 25) {
            DamagePolys.oldest = 0;
        }
    } else {
        i = DamagePolys.oldest;
        DamagePolys.polys[i] = ZOID_NewPoly(4, dummyVtx, damTexture, damColor, 0x820f, 1);
        ++DamagePolys.oldest;
        if (DamagePolys.oldest >= 25) {
            DamagePolys.oldest = 0;
            DamagePolys.full = 1;
        }
    }
    for (j = 1; j < 5; j++) {
        fade = i + j;
        if (fade >= 25) {
            fade -= 25;
        }
        if (DamagePolys.polys[fade]) {
            ZOID_UpdatePoly(DamagePolys.polys[fade], 0, 0, 0, &damColor[j], 0, 0);
        }
    }
    return i;
}

/* 0x8005e9dc, size 0x2e8 */
void newWorldDamage(float *pos, int slot)
{
    float newpos[3]; /* stack */
    float uvs[3][3]; /* stack */
    float xyz[4][3]; /* stack */
    float delta[3]; /* target keeps one edge-vector address across all checks */
    float edge_length2; /* target moves squared sum to fr5 before comparison */
    int damslot; /* r0 */
    int i; /* r30 */
    tModelData *m; /* r3 */
    m = &model[slot];
    if (m->airdist[0] > 8.0f || m->airdist[1] > 8.0f || m->airdist[2] > 8.0f || m->airdist[3] > 8.0f) {
        return;
    }
    for (i = 0; i < 4; i++) {
        xyz[i][0] = pos[0];
        xyz[i][1] = pos[1];
        xyz[i][2] = pos[2];
        xyz[i][1] += 3.0f;
        switch (i) {
        case 0:
            xyz[0][0] += -6.0f;
            xyz[0][2] += 6.0f;
            break;
        case 1:
            xyz[1][0] += -6.0f;
            xyz[1][2] += -6.0f;
            break;
        case 2:
            xyz[2][0] += 6.0f;
            xyz[2][2] += -6.0f;
            break;
        case 3:
            xyz[3][0] += 6.0f;
            xyz[3][2] += 6.0f;
            break;
        }
        if (RoadPosition(xyz[i], newpos, uvs)) {
            xyz[i][0] = newpos[0];
            xyz[i][1] = newpos[1];
            xyz[i][2] = newpos[2];
            xyz[i][1] += 0.25f;
        }
    }
    EDGE_CHECK(xyz[0], xyz[1]);
    EDGE_CHECK(xyz[1], xyz[2]);
    EDGE_CHECK(xyz[2], xyz[3]);
    EDGE_CHECK(xyz[3], xyz[0]);
    damslot = findDamageSlot();
    ZOID_UpdatePoly(DamagePolys.polys[damslot], 0, xyz, 0, damColor, 0x60f, 0);
}

