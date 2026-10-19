/* DC license screen; root-reviewed ownership transfer preserves original function ID. */
#include "licensescr_types.h"

int animText(void);
static const tBlitDef sofdecBlits[1] = {
    "SOFDEC",32,0,-1,-1,0,479,0,511,0,0xff,0,0
};
static const tBlitDef blits[2] = {
    "MIDWAY",32,23,-1,-1,0,176,0,511,0,0xff,0,0,
    (void *)0xffffffff,0,0,0,0,-1,-1,-1,-1,0,0,(void *)&animText,0
};
static _tBlit *sofdecBlit = 0;
static _tBlit *blit = 0;
static signed char inited = 0;

/* updateLicenseScreen, retaining the original front-end function identity. */
void func_8C07D9F8(void)
{
    if (!pfsInited) {
        func_8C06CADC((void *)&func_8C044584);
        func_8C06CD14(1,0);
        if (!lcdInited) {
            lcdInited = 1;
            func_8C06C2A6();
        }
        pfsInited = 1;
    }
    if (!gSofdecShown) {
        SetCountdownTimer(1.5f);
        sofdecBlit = NewMultiBlit(0,0,sofdecBlits,1);
        gSofdecShown = 1;
    } else {
    if (sofdecBlit) {
        if (!CountdownTimeOut()) return;
        RemoveBlit(sofdecBlit);
        sofdecBlit = 0;
    }
    if (!inited) {
        blit = NewMultiBlit(0,0,blits,2);
        inited = 1;
        SetCountdownTimer(3.0f);
    }
    if (CountdownTimeOut() || (gLicenseShown && (gActiveEdges & 15))) {
        if (blit) {
            RemoveBlit(blit);
            blit = 0;
        }
        inited = 0;
        setGameState(gLicenseShown ? 4 : 0x40000000);
        gLicenseShown = 1;
    }
    }
}

int animText(void)
{
    HAL_SetTextDepth(0.0f);
    HAL_SetFont(0);
    HAL_SetFontFixedWidth(0);
    HAL_SetFontSpacing(2);
    TB_SetJust(1,1);
    TB_SetTextColor(1);
    TB_DrawTextScrollBox(160,165,288,110,-1,0,gText.single[243]);
    if (gContLevels[0] == 102) {
        TB_SetTextColor(14);
        TB_Printf(210,30,"Built %s %s",dcBuildDate,dcBuildTime);
    }
    TB_SetJust(0,3);
    HAL_SetFontFixedWidth(1);
    HAL_SetFontSpacing(-1);
    HAL_SetTextDepth(-1.0f);
    return 1;
}
