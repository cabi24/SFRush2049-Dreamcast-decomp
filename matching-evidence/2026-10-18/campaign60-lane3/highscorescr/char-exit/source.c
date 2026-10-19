#include "highscorescr_types.h"

static void _InitScreen(void);
static void _UninitScreen(void);

static int fixword(char *,char *);
unsigned char *gDefaultNames[58] = {
    (unsigned char *)"MICROSERF", (unsigned char *)"YEAH BABY!", (unsigned char *)"EVAN J KELLY",
    (unsigned char *)"LITTLE BOO", (unsigned char *)"(-: TINA :-)", (unsigned char *)"EL",
    (unsigned char *)"ED LOGG", (unsigned char *)"GEL", (unsigned char *)"CAL",
    (unsigned char *)"RENE", (unsigned char *)"MERRILL", (unsigned char *)"REESER",
    (unsigned char *)"REESE", (unsigned char *)"DADDY-O", (unsigned char *)"MASON D",
    (unsigned char *)"SALTED SLUG", (unsigned char *)"GIO", (unsigned char *)"HANSON",
    (unsigned char *)"ROCKNROLL", (unsigned char *)"TODZILLA", (unsigned char *)"NED",
    (unsigned char *)"PEPSCHARMS@A", (unsigned char *)"HARVEY", (unsigned char *)"MAYA",
    (unsigned char *)"GREGMA", (unsigned char *)"CARA", (unsigned char *)"CHEPS",
    (unsigned char *)"JOSE E.", (unsigned char *)"BARRY L", (unsigned char *)"CHRIS L",
    (unsigned char *)"MICHAEL L", (unsigned char *)"JAMIE L", (unsigned char *)"THE JACKAL",
    (unsigned char *)"CPT AD'LSENT", (unsigned char *)"SCOTCHMO", (unsigned char *)"BIG UNIT",
    (unsigned char *)"ROTTWEILER", (unsigned char *)"SNIFF SNIFF", (unsigned char *)"KRUSER",
    (unsigned char *)"BIGTYMER JOF", (unsigned char *)"PIRU-DAMU-T", (unsigned char *)"SRAYER",
    (unsigned char *)"ISLND_KIDD", (unsigned char *)"RAVEN", (unsigned char *)"AXE_23",
    (unsigned char *)"DARTHRAIDER!", (unsigned char *)"STEELE", (unsigned char *)"MACHINE",
    (unsigned char *)"MEGABUSIVE", (unsigned char *)"CYRUS JACK", (unsigned char *)"MOUTH4WAR",
    (unsigned char *)"JAZWEL", (unsigned char *)"DSKWEEZ", (unsigned char *)"ISLANDXANGEL",
    (unsigned char *)"ORENGEJUSE", (unsigned char *)"SW3ET ROMEO", (unsigned char *)"GOOBLEMAN",
    (unsigned char *)"SHAWN P."
};
static signed char inited = 0;
static _tBlit *blit = 0;
static signed char gTable = 0;
static dcHighProfile **gLastPlayer[4] = { (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff };
static anon_x_y_c gPlayerRects[4][4] = {
    80, 55, 160, 110, 160, 110, 0, 0, 320, 220, 160, 110, 0, 0, 320, 220, 160, 110, 0, 0, 320, 220,
    160, 110, 80, 0, 160, 110, 160, 55, 80, 110, 160, 110, 160, 165, 0, 0, 320, 220, 160, 110, 0,
    0, 320, 220, 160, 110, 0, 0, 160, 110, 80, 55, 160, 0, 160, 110, 240, 55, 0, 110, 160, 110, 80,
    165, 0, 0, 320, 220, 160, 110, 0, 0, 160, 110, 80, 55, 160, 0, 160, 110, 240, 55, 0, 110, 160,
    110, 80, 165, 160, 110, 160, 110, 240, 165
};

void updateHighScoreScreen(void)
{
    signed char exit; /* target boolean */
    int p; /* r27 */

    if (!inited) {
        _InitScreen();
    }
    if (gSoftReset) {
        _SetHighScoreState(0);
        return;
    }
    switch (gHighScoreState) {
    case 1:
        ControlUnlocked();
        break;
    case 2:
        ControlCircuitCode();
        break;
    case 3:
        exit = 1;
        for (p = 0; p < gNumPlayers; p++) {
            if (gDrawName[p] == 1) {
                ControlName(p);
                exit = 0;
            } else if (namePanes[p]) {
                RemovePane(namePanes[p]);
                namePanes[p] = 0;
            }
        }
        if (exit == 1) {
            if (gRace == 2 && gGhostToSave) {
                TB_Strcpy((*gGhostToSave).name, gScoreName[0]);
                UnloadWaveAll(gTrackNum + 0x10f);
                gSavedGhost = CompressGhost(gGhostToSave, (unsigned char *)&gBlocksNeeded);
                gGhostToSave = 0;
                if (gSaveGhostFile && gSaveGhostFiles) {
                    _SetHighScoreState(4);
                } else {
                    SaveGhost(gGhostPak, &gGhostPlayer, gSavedGhost, 1);
                    gSavedGhost = 0;
                    _SetHighScoreState(5);
                }
            } else {
                _SetHighScoreState(5);
            }
        }
        break;
    case 4:
        SaveServer(0, 0);
        ControlGhost();
        break;
    case 5:
        ControlScores();
        break;
    }
}

static void _InitScreen(void)
{
    int p; /* r15 */
    int stuffUnlocked; /* stack */
    int oldCircuitTrack; /* r15 */
    unsigned char ranks[6]; /* stack */

    gSavedGhost = 0;
    stuffUnlocked = 0;
    if (gRace == 2) {
        gSaveGhostFile = (*gPlayers[0].save)->fileh != 0; /* stored second in the target, but written first: the other order blocks hoisting the *save loads (possible alias) */
        gGhostPlayer = &gRealGhostPlayer;
        memcpy(gGhostPlayer, *gPlayers[0].save, sizeof(dcHighProfile));
        gGhostPlayer->ll.prev = 0;
        gGhostPlayer->ll.next = 0;
        gGhostPlayer->fileh = 0;
        gGhostPlayer->datah = 0;
    }
    AudKillSong();
    if (gOptions[14] <= 18) {
        AudPlaySong(18, 1);
        AudSetMusicVol(gOptions[12] / 10.0f, 0.05000000111758709f);
    }
    memcpy(gPrevUnlockedTracks, gUnlockedTracks, sizeof(gPrevUnlockedTracks));
    memcpy(gPrevUnlockedCars, gUnlockedCars, sizeof(gPrevUnlockedCars));
    memcpy(gPrevUnlockedCircuits, gUnlockedCircuits, sizeof(gPrevUnlockedCircuits));
    memcpy(gPrevUnlockedTrans, gUnlockedTrans, sizeof(gPrevUnlockedTrans));
    memcpy(gPrevUnlockedEngines, gUnlockedEngines, sizeof(gPrevUnlockedEngines));
    memcpy(gPrevUnlockedTires, gUnlockedTires, sizeof(gPrevUnlockedTires));
    memcpy(gPrevUnlockedFrames, gUnlockedFrames, sizeof(gPrevUnlockedFrames));
    RecordGameOver();
    gHighScoreState = 0;
    inited = 1;
    circuitCode = 0;
    if (gRace == 3 && gGameCar[gCircuitPlayer].place_locked) {
        for (p = 0; p < 6; p++) {
            ranks[p] = gGameCar[p].place;
        }
        oldCircuitTrack = gCircuitTrack;
        UpdateCircuit(ranks, gGameCar[gCircuitPlayer].score);
        gCircuitTrack = oldCircuitTrack;
        if (!(*gPlayers[gCircuitPlayer].save)->fileh && gCircuitTrack < gCircuitSize - 1) {
            circuitCode = 1;
            GetCircuitCode(gCircuitCode);
        }
    }
    CalcUnlockedStuff();
    for (p = 0; p < gNumPlayers; p++) {
        gStuffUnlocked[p] = memcmp(gPrevUnlockedTracks[p], gUnlockedTracks[p], 19) ||
                            memcmp(gPrevUnlockedCars[p], gUnlockedCars[p], 13) ||
                            memcmp(gPrevUnlockedCircuits[p], gUnlockedCircuits[p], 4) ||
                            memcmp(gPrevUnlockedTrans[p], gUnlockedTrans[p], 8) ||
                            memcmp(gPrevUnlockedEngines[p], gUnlockedEngines[p], 9) ||
                            memcmp(gPrevUnlockedTires[p], gUnlockedTires[p], 5) ||
                            memcmp(gPrevUnlockedFrames[p], gUnlockedFrames[p], 6);
        if (gStuffUnlocked[p]) {
            stuffUnlocked = 1;
        }
        gDrawName[p] = 0;
    }
    gThisTrack = gTrackNum + (gBackwards ? 6 : 0);
    if (gRace == 0 || gRace == 2 || gRace == 3) {
        _RankScores();
    }
    ClearRushControls();
    SaveServer(1, 0);
    SaveFlush(0);
    if (stuffUnlocked) {
        _SetHighScoreState(1);
    } else if (circuitCode) {
        _SetHighScoreState(2);
    } else if (gRace == 0 || gRace == 3 || gRace == 2) {
        _SetHighScoreState(3);
    } else {
        _UninitScreen();
    }
}

static void _UninitScreen(void)
{
    int i; /* r28 */
    int p; /* r28 */

    if (gRace == 2 && gSavedGhost) {
        PS_LockFile((*gSavedGhost)->fileh, 0);
    }
    if (codePane) {
        RemovePane(codePane);
        codePane = 0;
    }
    for (p = 0; p < gNumPlayers; p++) {
        if (namePanes[p]) {
            RemovePane(namePanes[p]);
            namePanes[p] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (scorePanes[i]) {
            RemovePane(scorePanes[i]);
            scorePanes[i] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (ghostPanes[i]) {
            RemovePane(ghostPanes[i]);
            ghostPanes[i] = 0;
        }
    }
    inited = 0;
    setGameState(0x800000);
}

int censor(char *wp)
{
    char **op; /* r31 */
    char retval; /* r30 */
    char *const obscenities[22] = {
        "ASSHOLE", "ASSWHOLE", "FUCK", "PHUK", "PHUCK", "FUK", "SHIT", "CUM", "PISS", "CUNT", "PUSSY",
        "TWAT", "COCK", "NIGGER", "NIGGA", "FAGGOT", "FAG", "CHINK", "GOOK", "BEANER", "WETBACK", ""
    };

    retval = 0;
    for (op = (char **)obscenities; **op; op++) {
        retval |= fixword(*op, wp);
    }
    return retval;
}

static int fixword(char *op, char *wp)
{
    static char *op1; /* .sbss op1$1504 */
    static char *sp; /* .sbss sp$1505 */
    static signed short cnt; /* .sbss cnt$1506 */

    op1 = op;
    for (;;) {
        if (!*wp) {
            return 0;
        }
        if (*wp == *op) {
            break;
        }
        wp++;
    }
    sp = wp;
    while (*wp == *op) {
        wp++;
    }
    while (*op && *op == op[1]) {
        op++;
    }
    for (;;) {
        while (*++op == ' ') {
        }
        if (!*op) {
            break;
        }
        cnt = 0;
        for (;;) {
            while (*wp && *wp <= ' ') {
                wp++;
            }
            if (*wp == *op) {
                break;
            }
            if (!*wp || !cnt) {
                return fixword(op1, sp + 1);
            }
            cnt--;
            wp++;
        }
        while (*++wp == *op && *op != op[1]) {
        }
    }
    while (sp < wp) {
        *sp++ = '!';
    }
    fixword(op1, wp);
    return 1;
}

/* DrawName local initialized objects; target pools033D90/394/38C. */
static unsigned char del[4] = "DEL";
static unsigned char end[4] = "END";
static unsigned char spc[4] = "SPC";
