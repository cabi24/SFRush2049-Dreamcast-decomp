/* HAL_time.c - private Dreamcast once-per-frame callback table. */
#include "HAL_time_types.h"
static signed char gInited = 0; /* targetFDAC=8C0BDF98, byte test/storeFCD8..FCE4 */
static void (*gOPPFuncs[10])(void); /* targetFDB0=8C133ED8;40-byte spanFCE8/FD42/FD80 */
void HAL_InitTime(void)
{
    int i;
    if (gInited) return;
    gInited = 1;
    for (i=0;i<10;i++) gOPPFuncs[i] = NULL;
}
/* Extra DC time getter; original identity unproven. */
float func_8C02FD02(void)
{
    return (float)dcConvertTick(dcReadTick()) / 1000000.0f;
}
void HAL_InstallOncePerFrameFunc(void (*const func)(void))
{
    int i;
    if (!gInited) HAL_InitTime();
    for (i=0;i<10;i++) {
        if (gOPPFuncs[i] == NULL) {
            gOPPFuncs[i] = func;
            break;
        }
    }
}
void HAL_CallOncePerFrameFuncs(void)
{
    int i;
    if (!gInited) HAL_InitTime();
    for (i=0;i<10;i++) {
        if (gOPPFuncs[i]) gOPPFuncs[i]();
    }
}
