/* Bounded Dreamcast car-screen helper reconstruction; unsupported renderer remains external. */
#include "carscr_types.h"
/* Target-backed menu table; each record has3nonsemantic trailing bytes excluded in map. */
#define CAR_BUTTON(t) {t,-1,0.0f,{{{0.0f}}},0}
#define CAR_BUTTON_ROW { CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(0), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(1), CAR_BUTTON(2), CAR_BUTTON(3), CAR_BUTTON(4), CAR_BUTTON(6), CAR_BUTTON(4), CAR_BUTTON(6) }
static tCarScreenMenuButton buttonInfo[4][40] = {CAR_BUTTON_ROW,CAR_BUTTON_ROW,CAR_BUTTON_ROW,CAR_BUTTON_ROW};
void clearCarComponents(int p)
{
    int i;
    if (p > 0) return;
    for (i = 0; i < 11; i++) {
        if (carscrComponents8C0CC3D4[p][i].idx != -1) {
            ZOID_RemoveObject(carscrComponents8C0CC3D4[p][i].idx);
            carscrComponents8C0CC3D4[p][i].idx = -1;
            carscrComponents8C0CC3D4[p][i].level = -1;
        }
    }
    if (carscrPartFrame8C0CC380 != -1) {
        ZOID_RemoveObject((signed short)carscrPartFrame8C0CC380);
        carscrPartFrame8C0CC380 = -1;
    }
}

int animText(void)
{
    int i;
    tPlayerProfile **player;
    for (i = 0; i < gNumPlayers; i++) {
        if (carscrPlayers8C006000[i].save != &carscrJustPlayers8C15044C[i]) {
            player = carscrPlayerFiles8C15045C.head;
            while (player) {
                if (carscrPlayers8C006000[i].save == player) break;
                player = ((tCarScreenProfileLink *)*player)->next;
            }
            if (!player || PS_PakChanged((*(*player)->fileh)->pak)) return 1;
        }
        if (carscrButtonsInited8C19F604[i] == 1) carscrAnimButtonText8C077230(i);

    }
    return 1;
}

void calcValidOptions(int p)
{
    int i;
    carscrNumOptions8C19FAB4[p] = 0;
    for (i = 0; i < 17; i++) {
        if (validOption(p, i)) carscrOptions8C19FAC4[p][carscrNumOptions8C19FAB4[p]++] = i;
    }
}

int validOption(int p, int opt)
{
    int optionValid = 1;
    if ((opt == 11 || opt == 12) && !gEnableTireScaling) optionValid = 0;
    if (opt == 6 && (gRace == 2 || gRace == 6)) optionValid = 0;
    if (opt == 7 && gRace != 6 && !carscrOption7Available8C0BEEB6) optionValid = 0;
    if (opt == 8 && gRace != 6) optionValid = 0;
    if ((opt == 14 || opt == 15 || opt == 16) && gRace != 2) optionValid = 0;
    if (opt == 10 || opt == 13) optionValid = 0;
    return optionValid;
}

void calcStats(int p)
{
    signed short i;
    for (i = 0; i < 4; i++) {
        carscrStats8C19FBD4[p][i] = 1.0f;
        carscrStats8C19FBD4[p][i] *= carscrTransStats8C0CC680[carscrTrans8C0BBA04[p+1][carscrSelectionRows8C19FA3C[p][0]]][i];
        carscrStats8C19FBD4[p][i] *= carscrHandlingStats8C0CC700[carscrHandling8C0BBBC8[p+1][carscrSelectionRows8C19FA3C[p][0]]][i];
        carscrStats8C19FBD4[p][i] *= carscrEngineStats8C0CC730[carscrEngines8C0BBC09[p+1][carscrSelectionRows8C19FA3C[p][0]]][i];
        carscrStats8C19FBD4[p][i] *= carscrTireStats8C0CC7C0[carscrTires8C0BBD24[p+1][carscrSelectionRows8C19FA3C[p][0]]][i];
        carscrStats8C19FBD4[p][i] *= carscrFrameStats8C0CC810[carscrFrame8C0BBDB8[p+1][carscrSelectionRows8C19FA3C[p][0]]][i];
    }
}

void releaseGridMap(void)
{
    if (carscrGridLOD8C19FA14.displayList) {
        carscrShowGridMap8C0786E8(0);
        DeallocateMemory(carscrGridLOD8C19FA14.displayList);
    }
    carscrGridLOD8C19FA14.displayList = 0;
    carscrGridCarIdx8C0CC65C = -1;
}

void uninitButtons(signed char p)
{
    int buttonIdx;
    for (buttonIdx = 0; buttonIdx < 40; buttonIdx++) {
        if (buttonInfo[p][buttonIdx].objIdx != -1) {
            ZOID_RemoveObject((signed short)buttonInfo[p][buttonIdx].objIdx);
            buttonInfo[p][buttonIdx].objIdx = -1;
        }
    }
    carscrButtonsInited8C19F604[p] = 0;
}

tCarScreenGhost **getNextGhost(tCarScreenGhost **currGhost, signed char whichWay)
{
    tCarScreenGhost **nextGhost;
    if (!currGhost) {
        if (whichWay > 0) {
            currGhost = carscrGhostFiles8C150480.head;
            if (!currGhost) currGhost = carscrLimboGhosts8C19FA7C;
        } else currGhost = carscrLimboGhosts8C19FA7C + 2;
    }
    while (currGhost && *currGhost) {
        if (whichWay > 0) nextGhost = (*currGhost)->next;
        else nextGhost = (*currGhost)->prev;
        if ((*currGhost)->track == gTrackNum && (*currGhost)->backward == gBackwards &&
            currGhost != carscrGhosts8C15046C[1] && currGhost != carscrGhosts8C15046C[2] && currGhost != carscrGhosts8C15046C[3] &&
            (!(*currGhost)->fileh || carscrReadGhostData8C04DED2(currGhost))) {
            if ((*currGhost)->datah) break;
        }
        if (!nextGhost) {
            if (currGhost == carscrLimboGhosts8C19FA7C) {
                if (whichWay > 0) nextGhost = carscrLimboGhosts8C19FA7C + 1;
                else nextGhost = carscrGhostFiles8C150480.tail;
            } else if (currGhost == carscrLimboGhosts8C19FA7C + 1) {
                if (whichWay > 0) nextGhost = carscrLimboGhosts8C19FA7C + 2;
                else nextGhost = carscrLimboGhosts8C19FA7C;
            } else if (currGhost == carscrLimboGhosts8C19FA7C + 2) {
                if (whichWay < 0) nextGhost = carscrLimboGhosts8C19FA7C + 1;
            } else if (whichWay > 0) nextGhost = carscrLimboGhosts8C19FA7C;
        }
        currGhost = nextGhost;
    }
    if (currGhost && !*currGhost) currGhost = 0;
    return currGhost;
}
