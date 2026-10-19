#include "text_types.h"

signed short gLang;
tText gText;
static unsigned char gTextWave;
static int gTextWaveInst;

void _AdjustTextWavePointers(void);

void InitTextWave(void)
{
    static signed char sInited = 0;

    if (!sInited) {
        sInited = 1;
        gTextWave = gLargestTextWave;
        LockWaveInstance(gTextWaveInst = LoadWave(gLargestTextWave, 0, 0, 0, 0));
        _AdjustTextWavePointers();
        LoadTextWave();
    }
}

void LoadTextWave(void)
{
    int i;
    if (gLang != gTextWave) {
        ReplaceWave(gTextWave, gLang);
        gTextWave = gLang;
        _AdjustTextWavePointers();
        for (i = 0; i < 4; i++) {
            sprintf((char *)gJustPlayers[i]->name, "%s %d", gText.single[29], i + 1);
        }
    }
}

void _AdjustTextWavePointers(void)
{
    int i;
    unsigned char *data;
    unsigned int start;
    data = GetWaveInstanceAddr(gTextWaveInst);
    start = (unsigned int)data;
    gText.numSingle = *(unsigned int *)data;
    data += 4;
    gText.single = (unsigned char **)data;
    data += gText.numSingle * 4;
    gText.numTab = *(unsigned int *)data;
    data += 4;
    gText.tab = (unsigned short *)data;
    data += ((gText.numTab + 1) & ~1) * 2;
    gText.numTabA = *(unsigned int *)data;
    data += 4;
    gText.taba = (unsigned char **)data;
    data += gText.numTabA * 4;
    gText.numTab2 = *(unsigned int *)data;
    data += 4;
    gText.tab2 = (unsigned short *)data;
    data += ((gText.numTab2 + 1) & ~1) * 2;
    gText.numTab2a = *(unsigned int *)data;
    data += 4;
    gText.tab2a = (unsigned short *)data;
    data += ((gText.numTab2a + 1) & ~1) * 2;
    gText.numTab2b = *(unsigned int *)data;
    data += 4;
    gText.tab2b = (unsigned char **)data;
    for (i = 0; i < gText.numSingle; i++) {
        gText.single[i] += start;
    }
    for (i = 0; i < gText.numTabA; i++) {
        gText.taba[i] += start;
    }
    for (i = 0; i < gText.numTab2b; i++) {
        gText.tab2b[i] += start;
    }
}
