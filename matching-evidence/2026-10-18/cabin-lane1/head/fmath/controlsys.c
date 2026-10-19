#include "controlsys_types.h"

const unsigned int gControlTable[19] = {
    8,4,2,1,0x800,0x400,0x200,0x100,0x20000,0x10000,0x10,0x20,0x40,0x80,
    0x1000,0x2000,0x4000,0x8000,0
};

tPadInfo gControllerData[4];
float gContStick[4][2];
unsigned int gContLevels[4],gContEdges[4],gAutoEdges[4];
unsigned int gActiveEdges,gActiveLevels,gActiveAutoEdges;
float gRealStick[4][2];
unsigned int gRealLevels[4],gRealEdges[4];
signed char gSoftReset;
static int gSavedInterruptMask;
static volatile signed char gContStickRange[4][2];
static volatile unsigned int gNewEdges[4],gNewLevels[4];
static volatile signed char gNewStick[4][2];
static signed char gActiveControllers[4],gStickEdges[4],gHardStickEdges[4];
static int gAutoEdgeTimes[4][32];

static signed char gInitControls = 0;
static unsigned int gLastKeyTime = 0;
static signed char gResetLastKeyTime = 1;
static unsigned char gIgnoreCount = 0;

static void _InitSemaphore(void);
static void _GetSemaphore(void);
static void _ReleaseSemaphore(void);
#pragma inline(_InitSemaphore)
#pragma inline(_GetSemaphore)
#pragma inline(_ReleaseSemaphore)

void InitControlSys(void)
{
    static int sRegistered = 0;
    int i;
    int mask;
    if (!gInitControls) {
        SetControllerActive(-1,1);
        SetControllerStickEdges(-1,1);
        memset(gControllerData,0,sizeof(gControllerData));
        for (i=0;i<4;i++) gHardStickEdges[i]=0;
    }
    gInitControls=1;
    for (i=0;i<4;i++) gContStickRange[i][0]=gContStickRange[i][1]=110;
    _InitSemaphore();
    if (!sRegistered) {
        mask=SetInterruptMask(15);
        RegisterRetraceCallback(0x900,ModelThread,0x58,0);
        SetInterruptMask(mask);
        sRegistered=1;
    }
    UpdateControls();
}

void ClearControls(void)
{
    int i; /* register only: not in the DWARF */

    _GetSemaphore();
    gActiveEdges = gActiveAutoEdges = gActiveLevels = 0;
    for (i = 0; i < 4; i++) {
        gContStick[i][0] = gContStick[i][1] = 0.0f;
        gContEdges[i] = gContLevels[i] = gAutoEdges[i] = gNewEdges[i] = 0;
    }
    _ReleaseSemaphore();
}

void UpdateControls(void)
{
    unsigned int i; /* r19 */
    unsigned int j; /* r3 */
    float x; /* f0 */
    float y; /* f0 */

    _GetSemaphore();
    if (gResetLastKeyTime) {
        gResetLastKeyTime = 0;
        gLastKeyTime = gFrameCounter;
    }
    gActiveEdges = gActiveAutoEdges = gActiveLevels = 0;
    if (gIgnoreCount) {
        gIgnoreCount--;
        for (i = 0; i < 4; i++) {
            gNewEdges[i] = 0;
            gContStick[i][0] = gContStick[i][1] = 0.0f;
            gContEdges[i] = gContLevels[i] = gAutoEdges[i] = 0;
        }
        _ReleaseSemaphore();
        return;
    }
    for (i = 0; i < 4; i++) {
        x = _PotRange(gNewStick[i][0], gContStickRange[i][0], 1.0f);
        y = _PotRange(gNewStick[i][1], gContStickRange[i][1], 1.0f);
        gContStick[i][0] = x;
        gContStick[i][1] = y;
        gContLevels[i] = gNewLevels[i];
        gContEdges[i] = gNewEdges[i];
        gNewEdges[i] = 0;
        gAutoEdges[i] = 0;
        if (gContLevels[i]) {
            gLastKeyTime = gFrameCounter;
        }
        if (gActiveControllers[i]) {
            gActiveEdges |= gContEdges[i];
            gActiveLevels |= gContLevels[i];
        }
        j = 0;
        goto CheckAutoEdge;
NextAutoEdge:
        {
            if (j) {
                if (gContLevels[i] & (1 << j)) {
                    if (!gAutoEdgeTimes[i][j] || gFrameCounter - gAutoEdgeTimes[i][j] > (int)(gRetraceRate * 0.200000005f)) {
                        gAutoEdgeTimes[i][j] = gFrameCounter;
                        gAutoEdges[i] |= 1 << j;
                    }
                } else {
                    gAutoEdgeTimes[i][j] = 0;
                }
            }
            j++;
        }
CheckAutoEdge:
        if (j < 32) goto NextAutoEdge;
        if (gActiveControllers[i]) {
            gActiveAutoEdges |= gAutoEdges[i];
        }
    }
    _ReleaseSemaphore();
}

int GetControllerActive(int which)
{
    int i; /* r0 */

    if (which == -1) {
        for (i = 0; i < 4; i++) {
            if (!gActiveControllers[i]) {
                return 0;
            }
        }
        return 1;
    }
    return gActiveControllers[which];
}

void SetControllerActive(int which, int active)
{
    int i;
    if (which == -1) {
        for (i=0;i<4;i++) gActiveControllers[i]=active;
    } else if (which < 4) {
        gActiveControllers[which] = active;
    }
}

int GetControllerStickEdges(int which)
{
    int i; /* r0 */

    if (which == -1) {
        for (i = 0; i < 4; i++) {
            if (!gStickEdges[i]) {
                return 0;
            }
        }
        return 1;
    }
    return gStickEdges[which];
}

void SetControllerStickEdges(int which, int active)
{
    int i;
    if (which == -1) {
        for (i=0;i<4;i++) gStickEdges[i]=active;
    } else if (which < 4) {
        gStickEdges[which] = active;
    }
}

int ControllerUpdate(void)
{
    tDCPad *per;
    int i;
    int j;
    int ports[4]={0,6,12,18};
    sdk_func_8C0D7686();
    for (i=0;i<4;i++) {
        per=sdk_func_8C0D778C(ports[i]);
        if ((((volatile tPadInfo *)gControllerData)[i].attached && !per->info->type) ||
            (!((volatile tPadInfo *)gControllerData)[i].attached && per->info->type)) {
        }
        ((volatile tPadInfo *)gControllerData)[i].availButtons=0;
        for (j=0;j<19;j++) {
            if (per->support & gControlTable[j]) ((volatile tPadInfo *)gControllerData)[i].availButtons |= 1<<j;
        }
        ((volatile tPadInfo *)gControllerData)[i].incompatible=(((volatile tPadInfo *)gControllerData)[i].availButtons & 0x3f67)!=0x3f67;
        ((volatile tPadInfo *)gControllerData)[i].attached=per->info->type!=0;
        if (!((volatile tPadInfo *)gControllerData)[i].attached) {
            per->on=0;
            per->l=per->r=per->x1=per->y1=per->x2=per->y2=0;
        } else {
            gHardStickEdges[i]=per->info->type==1 &&
                !strncmp(per->info->product_name,"Racing Controller",strlen("Racing Controller"));
        }
        ((volatile tPadInfo *)gControllerData)[i].buttons=0;
        for (j=0;j<19;j++) {
            if (per->on & gControlTable[j]) ((volatile tPadInfo *)gControllerData)[i].buttons |= 1<<j;
        }
        ((volatile tPadInfo *)gControllerData)[i].stick1x=per->x1;
        ((volatile tPadInfo *)gControllerData)[i].stick1y=per->y1 < -127 ? 127 : -per->y1;
        ((volatile tPadInfo *)gControllerData)[i].stick2x=(signed char)per->x2;
        ((volatile tPadInfo *)gControllerData)[i].stick2y=per->y2 < -127 ? 127 : -per->y2;
        ((volatile tPadInfo *)gControllerData)[i].analog1=per->l;
        ((volatile tPadInfo *)gControllerData)[i].analog2=per->r;
        if (!gStickEdges[i] && gHardStickEdges[i] && (((volatile tPadInfo *)gControllerData)[i].buttons &0x3000)) {
            ((volatile tPadInfo *)gControllerData)[i].buttons &=0xffffcfff;
        }
    }
    ControlIRQ();
    return 1;
}

void ControlIRQ(void)
{
    unsigned int i; /* r20 */
    unsigned int edges; /* r4 */
    unsigned int levels; /* r0 */
    int stickEdge = 88;

    if (!gInitControls) {
        InitControlSys();
    }
    _GetSemaphore();
    for (i = 0; i < 4; i++) {
        if (!((volatile tPadInfo *)gControllerData)[i].attached) {
            gNewStick[i][0] = 0;
            gNewStick[i][1] = 0;
            levels = 0;
            gContStickRange[i][0] = 110;
            gContStickRange[i][1] = 110;
        } else {
            levels = ((volatile tPadInfo *)gControllerData)[i].buttons;
            gNewStick[i][0] = ((volatile tPadInfo *)gControllerData)[i].stick1x;
            gNewStick[i][1] = ((volatile tPadInfo *)gControllerData)[i].stick1y;
            if (gStickEdges[i]) {
                if (gNewStick[i][0] < -88) levels |=0x1000;
                if (gNewStick[i][0] > stickEdge) levels |=0x2000;
                if (gNewStick[i][1] < -88) levels |=0x800;
                if (gNewStick[i][1] > stickEdge) levels |=0x400;
            }
            if (levels &0x40000) gContStickRange[i][0]=gContStickRange[i][1]=110;
            if ((gNewStick[i][0] > gContStickRange[i][0] || gNewStick[i][0] < -gContStickRange[i][0]) &&
                gContStickRange[i][0] < 127) {
                gContStickRange[i][0]++;
            }
            if ((gNewStick[i][1] > gContStickRange[i][1] || gNewStick[i][1] < -gContStickRange[i][1]) &&
                gContStickRange[i][1] < 127) {
                gContStickRange[i][1]++;
            }
        }
        edges = levels ^ gNewLevels[i];
        edges &= levels;
        gNewEdges[i] |= edges;
        gRealEdges[i] = edges;
        gNewLevels[i] = levels;
        gRealLevels[i] = levels;
        gRealStick[i][0] = _PotRange(gNewStick[i][0], gContStickRange[i][0], 1.0f);
        gRealStick[i][1] = _PotRange(gNewStick[i][1], gContStickRange[i][1], 1.0f);
        if (gActiveControllers[i] && (gRealLevels[i]&2) && (gRealLevels[i]&4) &&
            ((gRealLevels[i]&0x20) || !(((volatile tPadInfo *)gControllerData)[i].availButtons&0x20)) &&
            ((gRealLevels[i]&0x40) || !(((volatile tPadInfo *)gControllerData)[i].availButtons&0x40)) &&
            (gRealEdges[i]&1)) gSoftReset=1;
    }
    _ReleaseSemaphore();
}

float _PotRange(int pot, float range, float max)
{
    float res; /* f1 */

    res = pot * max / range;
    if (res < -max) {
        res = -max;
    } else if (res > max) {
        res = max;
    }
    return res;
}

float func_8C01FDA8(void)
{
    return (float)(gFrameCounter-gLastKeyTime)*gRetraceTime;
}

static void _InitSemaphore(void)
{
    static signed char sInited=0;
    if (!sInited) sInited=1;
}

static void _GetSemaphore(void)
{
    gSavedInterruptMask=SetInterruptMask(15);
}

static void _ReleaseSemaphore(void)
{
    SetInterruptMask(gSavedInterruptMask);
}
