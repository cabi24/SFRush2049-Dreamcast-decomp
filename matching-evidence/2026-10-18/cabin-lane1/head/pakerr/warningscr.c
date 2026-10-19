#include "warningscr_types.h"

static signed char gPakScr = 0;
static signed char byte_8C0D19E1 = 0;

static const tWarningBlitDef WarningBlit[1] = {
    (void *)-1,0,0,0,0,-1,-1,-1,-1,0,0,func_8C093780,0
};

void updateWarningScreen(void)
{
    static _tBlit *sBlit = 0;
    int numPaks;
    int i;

    if (!sBlit) {
        gPakScr = 0;
        byte_8C0BF760 = 0;
    }
    if (!long_8C150464) {
        for (i = 0; i < 4; i++) {
            if (gControllerData[i].attached) break;
        }
        if (i < 4) {
            numPaks = 0;
            for (i = 0; i < 8; i++) {
                if (PS_PakInserted(i) && !PS_PakDamaged(i) && !PS_PakRumble(i)) {
                    numPaks++;
                    if (PS_GetFreeBlocks(i) >= PS_GetBlocks(0x810) && PS_GetFreeFiles(i) >= 1) goto done;
                }
            }
            byte_8C0D19E1 = numPaks;
            if (byte_8C0D19E1 > 0 || !byte_8C0BF760) {
                if (!sBlit) {
                    sBlit = NewMultiBlit(0,0,WarningBlit,1);
                    SetControllerActive(-1,1);
                    SetControllerStickEdges(-1,1);
                }
                if (!(11 & gActiveEdges)) return;
                PlayStartABSound();
            }
        }
    }
done:
    if (sBlit) {
        RemoveBlit(sBlit);
        sBlit = 0;
    }
    gBlocksNeeded = PS_GetBlocks(0x810);
    setGameState(gPakScr ? 0x4000000 : 4);
}

int func_8C093780(_tBlit *blit)
{
    int y;
    int fontHeight;
    int offset;
    char str[256];
    HAL_SetTextDepth(0.0f);
    TB_SetJust(1,1);
    HAL_SetFont(11);
    TB_SetTextColor(1);
    sprintf(str,"%s\n\n%s1%s %s %d%s",
        byte_8C0D19E1 > 0 ? gText.single[0x28c/4] : gText.single[0x288/4],
        gText.single[0x290/4],gText.single[0x180/4],gText.single[0x188/4],
        PS_GetBlocks(0x810),gText.single[0x178/4]);
    TB_DrawTextScrollBox(160,90,300,220,-1,0,str);
    fontHeight = TB_GetFontLineHeight();
    y = 200 - fontHeight * 3;
    HAL_SetFont(gPakScr ? 11 : 10);
    TB_SetTextColor(gPakScr ? 1 : 22);
    offset = !gPakScr;
    TB_DrawText(160,y - offset,gText.single[0x3d4/4]);
    TB_SetJust(0,3);
    HAL_SetTextDepth(-1.0f);
    return 1;
}
