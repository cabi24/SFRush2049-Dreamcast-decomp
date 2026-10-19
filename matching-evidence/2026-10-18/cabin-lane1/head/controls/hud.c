/* HUD lifecycle prefix. MAT3 semantics reconciled with DC 8C03478C..4C2A.
 * Remaining callbacks are not implemented in this bounded candidate.
 * DC RemoveHUD has no CleanupOverlay call at 8C034AB4..AB8.
 */
#include "hud_types.h"
#include "hud_data_externs.h"

unsigned char *cStrCreditNames[14][11] = {
    { hud_credit_text_00_00, hud_credit_text_00_01, hud_credit_text_00_02, hud_credit_text_00_03, 0, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_01_00, hud_credit_text_01_01, hud_credit_text_01_02, hud_credit_text_01_03, hud_credit_text_01_04, hud_credit_text_01_05, hud_credit_text_01_06, 0, 0, 0, 0 },
    { hud_credit_text_02_00, hud_credit_text_02_01, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_03_00, hud_credit_text_03_01, hud_credit_text_03_02, hud_credit_text_03_03, 0, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_04_00, hud_credit_text_04_01, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_05_00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_06_00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_07_00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_08_00, hud_credit_text_08_01, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_09_00, hud_credit_text_09_01, hud_credit_text_09_02, hud_credit_text_09_03, hud_credit_text_09_04, 0, 0, 0, 0, 0, 0 },
    { hud_credit_text_10_00, hud_credit_text_10_01, hud_credit_text_10_02, hud_credit_text_10_03, hud_credit_text_10_04, hud_credit_text_10_05, 0, 0, 0, 0, 0 },
    { hud_credit_text_11_00, hud_credit_text_11_01, hud_credit_text_11_02, hud_credit_text_11_03, hud_credit_text_11_04, hud_credit_text_11_05, hud_credit_text_11_06, 0, 0, 0, 0 },
    { hud_credit_text_12_00, hud_credit_text_12_01, hud_credit_text_12_02, hud_credit_text_12_03, hud_credit_text_12_04, hud_credit_text_12_05, 0, 0, 0, 0, 0 },
    { hud_credit_text_13_00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const tBlitDef RaceMultiBlit[90] = {
    { hud_tex_SPEEDBG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animSpeed, 16 },
    { hud_tex_SPEEDBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animSpeed, 32 },
    { hud_tex_SPEEDBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animSpeed, 33 },
    { hud_tex_SPEEDBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animSpeed, 34 },
    { hud_tex_SPEEDBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animSpeed, 35 },
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animSpeedText, 0 },
    { hud_tex_TACHOMETER_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animTach, 0 },
    { hud_tex_TACHOMETER_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animTach, 1 },
    { hud_tex_TACHOMETER_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animTach, 16 },
    { hud_tex_TACHOMETER_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animTach, 17 },
    { hud_tex_TACHOMETER_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animTach, 32 },
    { hud_tex_TACHOMETER_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animTach, 33 },
    { hud_tex_TACHOMETER_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animTach, 48 },
    { hud_tex_TACHOMETER_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animTach, 49 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 0 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 1 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 2 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 3 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 16 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 17 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 18 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 19 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 32 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 33 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 34 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 35 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 48 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 49 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 50 },
    { hud_tex_ODOMETER, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animOdometer, 51 },
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animGear, 0 },
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animCountdownTimer, 0 },
    { hud_tex_ELAPSEBG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animElapsedTimer, 1 },
    { hud_tex_ELAPSEBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animElapsedTimer, 2 },
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animElapsedTimerText, 0 },
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animPlace, 0 },
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animLapCount, 0 },
    { hud_tex_ELAPSEBG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animLapTime, 16 },
    { hud_tex_ELAPSEBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animLapTime, 32 },
    { hud_tex_ELAPSEBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animLapTime, 33 },
    { hud_tex_ELAPSEBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animLapTime, 34 },
    { hud_tex_ELAPSEBG2, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animLapTime, 35 },
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animLapTimeText, 0 },
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animWrongWay, 0 },
    { hud_tex_RADAR_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 96, (int (*)(_tBlit *))animRadar, 0 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 0 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 1 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 2 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 3 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 4 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 5 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 6 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 7 },
    { hud_tex_RADAR_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 96, (int (*)(_tBlit *))animRadar, 1 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 256 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 257 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 258 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 259 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 260 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 261 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 262 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 263 },
    { hud_tex_RADAR_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 96, (int (*)(_tBlit *))animRadar, 2 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 512 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 513 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 514 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 515 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 516 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 517 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 518 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 519 },
    { hud_tex_RADAR_LG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 96, (int (*)(_tBlit *))animRadar, 3 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 768 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 769 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 770 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 771 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 772 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 773 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 774 },
    { hud_tex_RADARBLIP, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animRadarBlip, 775 },
    { hud_tex_MAP_LG, 220, 70, -1, -1, -1, -1, -1, -1, 0, 128, (int (*)(_tBlit *))animTrackMap, 16 },
    { hud_tex_FLAG, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animFlag, 0 },
    { hud_tex_MAPDOT, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animDot, 0 },
    { hud_tex_MAPDOT, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animDot, 1 },
    { hud_tex_MAPDOT, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animDot, 2 },
    { hud_tex_MAPDOT, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animDot, 3 },
    { hud_tex_MAPDOT, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animDot, 4 },
    { hud_tex_MAPDOT, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animDot, 5 },
    { hud_tex_MAPDOT, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animDot, 6 },
    { hud_tex_MAPDOT, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animDot, 7 },
};

const tBlitDef HudMultiBlit[5] = {
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))updateFrame, 0 },
    { hud_tex_CROSSBONES, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animBones, 0 },
    { hud_tex_CROSSBONES, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animBones, 1 },
    { hud_tex_CROSSBONES, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animBones, 2 },
    { hud_tex_CROSSBONES, 0, 0, -1, -1, -1, -1, -1, -1, 0, 255, (int (*)(_tBlit *))animBones, 3 },
};

const tBlitDef ResultsMultiBlit[1] = {
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animResults, 0 },
};

const tBlitDef TwoPlayerMultiBlit[1] = {
    { hud_tex_MULTI_LOGO, 240, 0, -1, -1, -1, -1, -1, -1, 1, 255, 0, 0 },
};

const tBlitDef ThreePlayerMultiBlit[1] = {
    { hud_tex_MULTI_LOGO, 160, 110, 320, 220, 0, 219, 160, 479, 1, 255, 0, 0 },
};

const tBlitDef OverlayMultiBlit[1] = {
    { (const char *)-1, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, (int (*)(_tBlit *))animCreditText, 0 },
};

/* HUD placement prefix, D8C0BE470..BE740. Unknown next128 bytes omitted. */
int whichCam[4] = {
    1, 2, 4, 8
};

int speedPos[4][4][2] = {
    30, 4, 0, 0, 0, 0, 0, 0, 30, 4, 30, 114, 0, 0, 0, 0, 30, 4, 290, 4, 30, 114, 0, 0, 30, 4, 290, 4, 30, 114, 290, 114
};

int odometerPos[4][4][2] = {
    30, 32, 0, 0, 0, 0, 0, 0, 30, 30, 30, 140, 0, 0, 0, 0, 30, 30, 290, 30, 30, 140, 0, 0, 30, 30, 290, 30, 30, 140, 290, 140
};

int tachPos[4][4][2] = {
    90, 4, 0, 0, 0, 0, 0, 0, 90, 4, 90, 114, 0, 0, 0, 0, 70, 6, 240, 6, 70, 116, 0, 0, 70, 6, 240, 6, 70, 116, 240, 116
};

int gearPos[4][4][2] = {
    96, 14, 0, 0, 0, 0, 0, 0, 92, 10, 92, 120, 0, 0, 0, 0, 82, 10, 264, 10, 82, 120, 0, 0, 82, 10, 264, 10, 82, 120, 264, 120
};

int countdownTimerPos[4][2] = {
    160, 4, 272, 54, 240, 109, 160, 70
};

int elapsedTimerPos[4][2] = {
    272, 142, 272, 142, 240, 197, 160, 134
};

int placePos[4][4][2] = {
    280, 4, 0, 0, 0, 0, 0, 0, 210, 4, 210, 114, 0, 0, 0, 0, 136, 4, 184, 4, 136, 114, 0, 0, 136, 4, 184, 4, 136, 196, 184, 196
};

void ShowHUD(void)
{
    int init = 0;
    int x, y;
    static float delay;
    static float nextSwitch;
    if (!HudBlit1) {
        int slot;
        for (slot = 0; slot < 4; slot++) {
            gCheckPtTimeout[slot] = 0.0f;
            gLapTime[slot] = 0.0f;
            gElapsedTime[slot] = 0.0f;
            gPlayerCoin[slot][0].idx = -1;
            gPlayerCoin[slot][1].idx = -1;
        }
        if (gGameState & 0x2000000)
            HudBlit1 = NewMultiBlit(0, 0, OverlayMultiBlit, 1);
        else if (gGameState & 8)
            HudBlit1 = NewMultiBlit(0, 0, OverlayMultiBlit, 1);
        else {
            if (gNumWindows == 2)
                multiPlayerBlit = NewMultiBlit(0, 0, TwoPlayerMultiBlit, 1);
            else if (gNumWindows == 3)
                multiPlayerBlit = NewMultiBlit(0, 0, ThreePlayerMultiBlit, 1);
            if (gRace == kBattle || gRace == kStunt)
                ShowHUDExtra();
            else
                HudBlit2 = NewMultiBlit(0, 0, RaceMultiBlit, 90);
            HudBlit1 = NewMultiBlit(0, 0, HudMultiBlit, 5);
        }
        markers_valid = 0;
        init = 1;
    }
    if ((gGameState & 8) && gShowCredits) {
        if (init) {
            gCredit = -1;
            SetCountdownTimer(90.0f);
            delay = GetCountdownTimeRemaining() / 14.0f;
            nextSwitch = gSeconds;
        }
        if (gSeconds >= nextSwitch) {
            nextSwitch += delay;
            gCredit++;
            gCorner = Random(4.0f);
            HAL_SetFont(11);
            gCreditWidth = HAL_GetTextWidth(gText.taba[gText.tab[0] + gCredit], -1);
            for (gNumCreditNames = 0; cStrCreditNames[gCredit][gNumCreditNames]; gNumCreditNames++) {
                if (HAL_GetTextWidth(cStrCreditNames[gCredit][gNumCreditNames], -1) > gCreditWidth)
                    gCreditWidth = HAL_GetTextWidth(cStrCreditNames[gCredit][gNumCreditNames], -1);
            }
            switch (gCorner) {
            case 0: x = 20; y = 20; break;
            case 1: x = 280 - gCreditWidth; y = 20; break;
            case 2: x = 20; y = 145 - 10 * gNumCreditNames; break;
            default: x = 280 - gCreditWidth; y = 145 - 10 * gNumCreditNames; break;
            }
            RemovePanes(0);
            InitPane(x, y, x + gCreditWidth + 20, y + 10 * gNumCreditNames + 32, 192, 0, 0, 0);
        }
    }
}




void RemoveHUD(void)
{
    int player;

    RemovePanes(1);
    if (HudBlit1) {
        RemoveBlit(HudBlit1);
        HudBlit1 = 0;
    }
    if (HudBlit2) {
        RemoveBlit(HudBlit2);
        HudBlit2 = 0;
    }
    if (ResultBlit) {
        RemoveBlit(ResultBlit);
        ResultBlit = 0;
    }
    for (player = 0; player < gNumWindows; player++)
        resultPane[player] = 0;
    resultPane[4] = 0;
    for (player = 0; player < gNumWindows; player++) {
        if (gPlayerCoin[player][0].idx != -1) {
            ZOID_RemoveObject(gPlayerCoin[player][0].idx);
            gPlayerCoin[player][0].idx = -1;
        }
        if (gPlayerCoin[player][1].idx != -1) {
            ZOID_RemoveObject(gPlayerCoin[player][1].idx);
            gPlayerCoin[player][1].idx = -1;
        }
    }
    if (gRace == kBattle || gRace == kStunt)
        RemoveHUDExtra();
}

void RemoveMultiplayerHUD(void)
{
    if (multiPlayerBlit) {
        RemoveBlit(multiPlayerBlit);
        multiPlayerBlit = 0;
    }
}

void UpdateHUDPostCamera(void)
{
    if (gGameState == -1 || (!(gGameState & 0x200000) && !(gGameState & 0x400000)))
        return;
    if (gPauseState == 0)
        hudObjAngle += 3.1415928f * gFrameSeconds;
    if (gRace == 0 || gRace == 1 || gRace == 2 || gRace == 3 || gRace == kStunt)
        animCoins(0);
    if (gRace == kBattle || gRace == kStunt)
        UpdateHUDExtraPostCamera();
}

void AddBattlePoint(signed char to, signed char from, signed char p)
{
    if (gRace == kBattle || gRace == kStunt)
        _AddBattlePoint(to, from, p);
}


void CheckResults(void);

int updateFrame(void)
{
    signed short cars_in_order[6];
    signed short index, i, j;
    int slot;
    for (slot = 0; slot < 4; slot++)
        cvt_time(gGameCar[slot].place_locked == 1 ? gGameCar[slot].score : gModelTime,
                 TrackTimeStr[slot], 'f');
    for (i = 0; i < 6; i++)
        dot_order_list[i] = -1;
    for (i = 0; i < gNumActiveCars; i++) {
        index = 0;
        for (j = 0; j < gNumActiveCars; j++) {
            index = model[j].slot;
            if (gGameCar[index].place == i)
                break;
        }
        cars_in_order[i] = index;
    }
    /* volatile read: the target reloads num_humans on every loop test (header keeps it plain) */
    for (j = 0; j < *(volatile signed short *)&num_humans; j++)
        dot_order_list[gNumActiveCars - j - 1] = gPlayers[j].slot;
    for (i = 0; i < gNumActiveCars; i++) {
        if (model[index = cars_in_order[i]].drone_type == 1)
            dot_order_list[gNumActiveCars - 1 - j++] = index;
        if (j == gNumActiveCars)
            break;
    }
    markers_valid = 1;
    CheckResults();
    return 1;
}

void CheckResults(void)
{
    int playerIdx, x, y;
    int gAPlayerFinished = 0;
    int gNumPlayersFinished = 0;
    for (playerIdx = 0; playerIdx < gNumWindows; playerIdx++) {
        signed short numLaps = gNumLaps;
        if (numLaps == model[playerIdx].laps)
            gAPlayerFinished = 1;
        if (gGameCar[model[playerIdx].slot].place_locked == 1)
            gNumPlayersFinished++;
    }
    if (gNumPlayersFinished && (!gDeathMode || gAPlayerFinished)) {
        for (playerIdx = 0; playerIdx <= 4; playerIdx++) {
            if (!resultPane[playerIdx]) {
                resultPane[playerIdx] = InitPane(-20, 0, -10, 10, 192, 0, 0, 1);
                if (playerIdx < 4)
                    ChangePaneColor(resultPane[playerIdx], playerPaneColors[playerIdx][0], playerPaneColors[playerIdx][1]);
            }
        }
        if (!ResultBlit)
            ResultBlit = NewMultiBlit(0, 0, ResultsMultiBlit, 1);
    }
    HAL_SetFont(10);
    if (gNumWindows > 1 && gNumPlayersFinished == gNumWindows && (!gDeathMode || (gDeathMode && gAPlayerFinished))) {
        if (resultPane[4]) {
            unsigned char str[64] = "";
            int h = TB_GetFontLineHeight();
            int w;
            if (model[model[gRankedPlayer[0]].slot].drone_type == 1)
                TB_Sprintf(str, gText.single[211]);
            else if (ties)
                TB_Sprintf(str, gText.single[212], ties + 1);
            else
                TB_Sprintf(str, gText.single[210], (*gPlayers[gRankedPlayer[0]].save)->name);
            w = HAL_GetTextWidth(str, -1) < 20 ? 20 : HAL_GetTextWidth(str, -1);
            x = 160; y = 110;
            ChangePaneSize(resultPane[4], x - w / 2 - 8, y - h / 2 - 4, x + w / 2 + 8, y + h / 2 + 4);
        }
    }
    for (playerIdx = 0; playerIdx < gNumWindows; playerIdx++) {
        if ((!gDeathMode || gNumLaps == model[playerIdx].laps) && gGameCar[model[playerIdx].slot].place_locked == 1 && resultPane[playerIdx]) {
            unsigned char str1[32] = "", str2[32] = "";
            int w, h = TB_GetFontLineHeight();
            x = wrongWayPos[gNumWindows - 1][playerIdx][0];
            y = wrongWayPos[gNumWindows - 1][playerIdx][1];
            if (gNumWindows == 4 && playerIdx >= 2)
                y += 12;
            if (gRace == kStunt) {
                TB_Sprintf(str1, "%d %s", gStuntPlayer[playerIdx].totalPoints, gText.single[140]);
            } else if (gRace == kObstacle) {
                if (gLap[playerIdx] != 1)
                    TB_Sprintf(str1, "%s", gText.single[225]);
                else {
                    unsigned int min;
                    unsigned int sec = (unsigned int)gGameCar[gPlayers[playerIdx].slot].score;
                    unsigned int thou;
                    min = sec / 60;
                    min %= 100;
                    sec %= 60;
                    thou = (unsigned int)(1000.0f * gGameCar[gPlayers[playerIdx].slot].score);
                    thou %= 1000;
                    TB_Sprintf(str1, "%s %d:%02d.%03d", gText.single[219], min, sec, thou);
                }
            } else if (gRace == kBattle) {
                TB_Sprintf(str1, "%d %s", gGameCar[playerIdx].battle.Points, gText.single[140]);
            } else {
                int place = gGameCar[gPlayers[playerIdx].slot].place;
                TB_Sprintf(str1, gText.single[209], place + 1, gText.taba[gText.tab[57] + place]);
                if (gNumLaps != model[gPlayers[playerIdx].slot].laps)
                    TB_Sprintf(str2, gText.single[218], model[gPlayers[playerIdx].slot].laps, gNumLaps); /* the two extra varargs are the values the compare above already loaded: passing them pins them to r5/r6 at no instruction cost */
                else {
                    unsigned int min;
                    unsigned int sec = (unsigned int)gGameCar[gPlayers[playerIdx].slot].score;
                    unsigned int thou;
                    min = sec / 60;
                    min %= 100;
                    sec %= 60;
                    thou = (unsigned int)(1000.0f * gGameCar[gPlayers[playerIdx].slot].score);
                    thou %= 1000;
                    TB_Sprintf(str2, "%s %d:%02d.%03d", gText.single[219], min, sec, thou);
                }
            }
            w = HAL_GetTextWidth((*gPlayers[playerIdx].save)->name, -1) < 20 ? 20 : HAL_GetTextWidth((*gPlayers[playerIdx].save)->name, -1);
            w = w > HAL_GetTextWidth(str1, -1) ? w : HAL_GetTextWidth(str1, -1);
            w = w > HAL_GetTextWidth(str2, -1) ? w : HAL_GetTextWidth(str2, -1);
            if (TB_Strlen(str1))
                h += TB_GetFontLineHeight();
            if (TB_Strlen(str2))
                h += TB_GetFontLineHeight();
            ChangePaneSize(resultPane[playerIdx], x - w / 2 - 8, y - h / 2 - 4, x + w / 2 + 8, y + h / 2 + 4);
        }
    }
}

