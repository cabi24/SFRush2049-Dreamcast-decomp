/* Trackscr real DC menu bodies; six renderer-specific bodies remain externally mapped. */
#include "..\include\trackscr_private.h"
char *buttonNames[9] = {"BUTTON_S1G1","BUTTON_S1G1","BUTTON_S2G1","CURSORG1","AB_BUTTONG1","ARROW_BGG1","ARROW_FGG1","BAR_BGG1","BAR_FGG1"};
tTrackButton buttonInfo[48] = {
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {0,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {1,-1,0.0f},
 {2,-1,0.0f},
 {3,-1,0.0f},
 {4,-1,0.0f},
 {6,-1,0.0f},
 {4,-1,0.0f},
 {6,-1,0.0f}
};
int buttonsInited=0;
int buttonsJustInited=0;
const tBlitDef mblit[18] = {
    "SLIDER", 0, 0, -1, -1, -1, -1, -1, -1, 0, 0xfe, (void *)&animNewSlider, 0xe0000, "SLIDER", 0,
    0, -1, -1, -1, -1, -1, -1, 0, 0xfe, (void *)&animNewSlider, 0xe0001, "SLIDER", 0, 0, -1, -1,
    -1, -1, -1, -1, 0, 0xfe, (void *)&animNewSlider, 0xf0000, "SLIDER", 0, 0, -1, -1, -1, -1, -1,
    -1, 0, 0xfe, (void *)&animNewSlider, 0xf0001, "SLIDER", 0, 0, -1, -1, -1, -1, -1, -1, 0, 0xfe,
    (void *)&animNewSlider, 0x120000, "SLIDER", 0, 0, -1, -1, -1, -1, -1, -1, 0, 0xfe,
    (void *)&animNewSlider, 0x120001, "SLIDER", 0, 0, -1, -1, -1, -1, -1, -1, 0, 0xfe,
    (void *)&animNewSlider, 0x130000, "SLIDER", 0, 0, -1, -1, -1, -1, -1, -1, 0, 0xfe,
    (void *)&animNewSlider, 0x130001, "ARROW_SML_LR_BG", 0, 0, -1, -1, -1, -1, -1, -1, 0x11, 0xff,
    (void *)&animArrows, 0, "ARROW_SML_LR_FG", 0, 0, -1, -1, -1, -1, -1, -1, 0x10, 0xfe,
    (void *)&animArrows, 0x1001, "ARROW_SML_LR_BG", 0, 0, -1, -1, -1, -1, -1, -1, 0x11, 0xff,
    (void *)&animArrows, 0x2010, "ARROW_SML_LR_FG", 0, 0, -1, -1, -1, -1, -1, -1, 0x10, 0xfe,
    (void *)&animArrows, 0x3011, "ARROW_SML_UD_BG", 0, 0, -1, -1, -1, -1, -1, -1, 0x11, 0xff,
    (void *)&animArrows, 0x4020, "ARROW_SML_UD_FG", 0, 0, -1, -1, -1, -1, -1, -1, 0x10, 0xfe,
    (void *)&animArrows, 0x5021, "ARROW_SML_UD_BG", 0, 0, -1, -1, -1, -1, -1, -1, 0x11, 0xff,
    (void *)&animArrows, 0x6030, "ARROW_SML_UD_FG", 0, 0, -1, -1, -1, -1, -1, -1, 0x10, 0xfe,
    (void *)&animArrows, 0x7031, "RUSHFLAGS", 150, 82, -1, -1, -1, -1, -1, -1, 0, 0xff,
    (void *)&animFan, 0x6400, (void *)0xffffffff, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0,
    (void *)&animText, 0
};



void updateTrackScreen(void)
{
    int i;
    int j;
    tPlayerProfile **player;
    int add;
    if (gSkipToGameState != 1) {
        if (gSkipToGameState != gGameState) { setGameState(0x100); return; }
        gSkipToGameState = 1;
    }
    for (i = 0; i < gNumPlayers; i++) {
        if (gPlayers[i].save != &gJustPlayers[i]) {
            player = gPlayerFiles.head;
            while (player) {
                if (gPlayers[i].save == player) break;
                player = ((tTrackProfileHead *)*player)->next;
            }
            if (!player || func_8C06D41A(((tTrackFilePak *)*(*player)->fileh)->pak)) { _UninitScreen(); setGameState(0x20); return; }
        }
    }
    if (!inited) _InitScreen();
    showTrackSelect();
    if (gSoftReset) { _UninitScreen(); setGameState(4); return; }
    if (gActiveEdges & 0xB) {
        PlayStartABSound(gActiveEdges); setGameState(0x100); _UninitScreen();
        for (j = 0; j < gNumPlayers; j++)
            for (i = 20; i < 40; i++) SavePlayerOption(gPlayers[j].save,i,gOptions[i]);
        return;
    }
    if (gActiveEdges & 4) {
        AudPlayEffect(1,0,1,0); _UninitScreen();
        if (gRace == kCircuit) setGameState(0x4000);
        else setGameState(0x40);
        return;
    }
    if (gActiveLevels & 0x3080) {
        add = (gActiveAutoEdges & 0x1000) ? -1 : (gActiveAutoEdges & 0x2000) != 0;
        if (gRace == kCircuit && (gCircuitTrack > 0 || gOpts[gActiveOption] != 20)) goto controls_done;
        if (add == 0 && !gActiveZ) goto controls_done;
        if (gRace == kGhost && gOpts[gActiveOption] == 2) goto controls_done;
        PlayLRSound(gActiveAutoEdges);
        if (gMirrorControls > 1) add = -add;
        switch (gOpts[gActiveOption]) {
        case 0:
            if (add) {
                gTrack += add;
                if (gTrack < 0) gTrack = gNumTracks-1;
                else if (gTrack >= gNumTracks) gTrack = 0;
                do {
                    gTrackNum += add;
                    if (gTrackNum < 0) gTrackNum = 18;
                    else if (gTrackNum >= 19) gTrackNum = 0;
                } while (!TrackValid(gTrackNum));
                for (i = 20; i < 40; i++) gOptions[i] = GetPlayerOption(gPlayers[0].save,i);
            }
            break;
        case 1:
            if (add) {
                gOptions[24] += add;
                if (gOptions[24] < 0) gOptions[24] = 1;
                else if (gOptions[24] > 1) gOptions[24] = 0;
            } else gOptions[24] = 0;
            break;
        case 2:
            if (add) {
                gOptions[20] += add;
                if (gOptions[20] < 1) gOptions[20] = 8;
                else if (gOptions[20] > 8) gOptions[20] = 1;
            } else gOptions[20] = 3;
            break;
        case 3:
            if (add) {
                gOptions[31] += add;
                if (gOptions[31] < 0) gOptions[31] = 2;
                else if (gOptions[31] > 2) gOptions[31] = 0;
            } else gOptions[31] = 0;
            break;
        case 4:
            if (add) {
                if (!gOptions[32]) gOptions[32] = 1;
                else gOptions[32] = 0;
            } else gOptions[32] = 0;
            break;
        case 5:
            if (add) {
                gOptions[33] += add;
                if (gOptions[33] <= 0) gOptions[33] = 100;
                else if (gOptions[33] > 100) gOptions[33] = 1;
            } else gOptions[33] = 69;
            break;
        case 6:
            if (add) {
                gOptions[34] += add;
                if (gOptions[34] <= 0) gOptions[34] = 20;
                else if (gOptions[34] > 20) gOptions[34] = 1;
            } else gOptions[34] = 5;
            break;
        case 7:
            if (add) {
                gOptions[35] += add;
                if (gOptions[35] <= 0) gOptions[35] = 50;
                else if (gOptions[35] > 50) gOptions[35] = 1;
            } else gOptions[35] = 30;
            break;
        case 8:
            if (add) {
                if (!gOptions[36]) gOptions[36] = 1;
                else gOptions[36] = 0;
            } else gOptions[36] = 0;
            break;
        case 9:
            if (add) {
                gOptions[37] += 5*add;
                if (gOptions[37] < 5) gOptions[37] = 50;
                else if (gOptions[37] > 50) gOptions[37] = 5;
            } else gOptions[37] = 10;
            break;
        case 10:
            if (add) {
                gOptions[38] += add;
                if (gOptions[38] <= 0) gOptions[38] = 20;
                else if (gOptions[38] > 20) gOptions[38] = 1;
            } else gOptions[38] = 8;
            break;
        case 11:
            if (add) {
                gOptions[39] ^= 1;
            } else gOptions[39] = 0;
            break;
        case 12:
            if (add) {
                gOptions[26] ^= 1;
            } else gOptions[26] = 0;
            clearTrackMap(); buildTrackMap(); pathPos = 1.0f-pathPos;
            break;
        case 13:
            if (add) {
                if ((gPlayers[0].levels & 0x3c000) == 0x3c000) gOptions[27] = 2;
                else gOptions[27] = !gOptions[27];
            } else gOptions[27] = 0;
            clearTrackMap(); buildTrackMap();
            break;
        case 14:
            if (add) {
                gOptions[28] += add;
                if (gOptions[28] < 0) gOptions[28] = 3;
                else if (gOptions[28] > 3) gOptions[28] = 0;
            } else gOptions[28] = 0;
            break;
        case 15:
            if (add) {
                gOptions[29] += add;
                if (gOptions[29] < 0) gOptions[29] = 4;
                else if (gOptions[29] > 4) gOptions[29] = 0;
            } else gOptions[29] = 0;
            break;
        case 16:
            if (add) {
                gOptions[22] += add;
                if (gOptions[22] < 0) gOptions[22] = 3;
                else if (gOptions[22] > 3) gOptions[22] = 0;
            } else gOptions[22] = 1;
            break;
        case 17:
            if (add) {
                gOptions[21] += add;
                if (gOptions[21] > 5) gOptions[21] = 0;
                else if (gOptions[21] < 0) gOptions[21] = 5;
            } else gOptions[21] = 5;
            break;
        case 18:
            if (add) {
                gOptions[23] += add;
                if (gOptions[23] < 0) gOptions[23] = 5;
                else if (gOptions[23] > 5) gOptions[23] = 0;
            } else gOptions[23] = 2;
            break;
        case 19:
            if (add) {
                gOptions[25] += add;
                if (gOptions[25] < 0) gOptions[25] = 2;
                else if (gOptions[25] > 2) gOptions[25] = 0;
            } else gOptions[25] = 2;
            break;
        case 20:
            if (add) {
                gOptions[30] ^= 1;
            } else gOptions[30] = 0;
            if (gRace == kCircuit) (*(*gPlayers[gCircuitPlayer].save)->datah)->circuit.death = gOptions[30];
            break;
        }
        SetOptions();
        for (i = 20; i < 40; i++) SavePlayerOption(gPlayers[0].save,i,gOptions[i]);
    } else if (gActiveAutoEdges & 0x400) {
        AudPlayEffect(8,0,1,0);
        if (--gActiveOption < 0) gActiveOption = gNumOptions-1;
    } else if (gActiveAutoEdges & 0x800) {
        AudPlayEffect(2,0,1,0);
        if (++gActiveOption > gNumOptions-1) gActiveOption = 0;
    }
    if (gFirstOption > gActiveOption-1) gFirstOption = gActiveOption-1;
    if (gFirstOption < gActiveOption-2) gFirstOption = gActiveOption-2;
    if (gFirstOption > gNumOptions-4) gFirstOption = gNumOptions-4;
    if (gFirstOption < 0) gFirstOption = 0;
controls_done:
    gActiveOptionEnabled = gRace != kCircuit || (gCircuitTrack == 0 && gOpts[gActiveOption] == 20);
    if (gTrackNum != oldTrack) {
        desiredTrackMap = gTrackNum;
        oldTrack = gTrackNum;
        for (i = 0; i < gNumPlayers; i++) {
            switch (gRace) {
            case kOneRace: case kPractice: case kGhost:
                if (gTrackNum >= 0 && gTrackNum <= 5) SavePlayerTrack(gPlayers[i].save,gRace,gTrackNum+0);
                break;
            case kStunt:
                if (gTrackNum >= 14 && gTrackNum <= 17) SavePlayerTrack(gPlayers[i].save,gRace,gTrackNum-14);
                break;
            case kBattle:
                if (gTrackNum >= 6 && gTrackNum <= 13) SavePlayerTrack(gPlayers[i].save,gRace,gTrackNum-6);
                break;
            }
        }
    }
    animTrackMap();
    animTrackMaps();
    SetFogIntensity(0);
    ZeroCamera(1); UpdateCameras();
}

void _InitScreen(void)
{
    int i;
    int j;
    float pos;
    gActiveOption = 0;
    gFirstOption = 0;
    gActiveZ = 0;
    currentTrackMap = -1;
    gNumOptions = 0;
    for (i = 0; i < 21; i++) if (OptionValid(i)) gOpts[gNumOptions++] = i;
    buildTrackMaps();
    if (gRace == kCircuit) oldTrack = gTrackNum = gCircuitTracks[gCircuitTrack].trackNum;
    else {
        if (gRace == kStunt) oldTrack = gTrackNum = GetPlayerTrack(gPlayers[0].save,gRace)+14;
        else if (gRace == kObstacle) oldTrack = gTrackNum = GetPlayerTrack(gPlayers[0].save,gRace)+18;
        else if (gRace == kBattle) oldTrack = gTrackNum = GetPlayerTrack(gPlayers[0].save,gRace)+6;
        else oldTrack = gTrackNum = GetPlayerTrack(gPlayers[0].save,gRace)+0;
        if (!TrackValid(gTrackNum)) {
            if (gRace == kStunt) oldTrack = gTrackNum = 14;
            else if (gRace == kObstacle) oldTrack = gTrackNum = 18;
            else if (gRace == kBattle) oldTrack = gTrackNum = 6;
            else oldTrack = gTrackNum = 0;
        }
        for (i = 0; i < gNumPlayers; i++) {
            if (gRace == kStunt) SavePlayerTrack(gPlayers[i].save,gRace,gTrackNum-14);
            else if (gRace == kObstacle) SavePlayerTrack(gPlayers[i].save,gRace,gTrackNum-18);
            else if (gRace == kBattle) SavePlayerTrack(gPlayers[i].save,gRace,gTrackNum-6);
            else SavePlayerTrack(gPlayers[i].save,gRace,gTrackNum);
        }
        for (j = 20; j < 40; j++) gOptions[j] = GetPlayerOption(gPlayers[0].save,j);
    }
    SetOptions();
    createBG();
    initButtons();
    blit = (_tBlit_2 *)NewMultiBlit(0,0,mblit,18);
    { int j;
    i = 0;
    for (j = 0; j < 19; j++) {
        if (TrackValid(j)) {
            if (j == gTrackNum) {gTrack = i; break;}
            i++;
        }
    }
    }
    gNumTracks = 19;
    i = 0;
    pos = 0.0f;
    for (j = 0; j < 19; j++) {
        if (TrackValid(j)) {
            gTracks[i].gTrackNum = j;
            gTracks[i].slot = i-gTrack;
            if (gTracks[i].slot < 0) gTracks[i].slot += gNumTracks;
            if (gTrack == i) gTracks[i].targetPos = 0.0f;
            else if (gTrack >= gNumTracks/2) gTracks[i].targetPos = pos - (gNumTracks-gTrack)*100;
            else gTracks[i].targetPos = pos + gTrack*100;
            gTracks[i].pos[0] = gTracks[i].targetPos;
            gTracks[i].pos[1] = -5.0f;
            gTracks[i].pos[2] = 0.0f;
            pos -= 100.0f;
            if (pos < -(gNumTracks*100/2)) pos += gNumTracks*100;
            i++;
        }
    }
    trackInitFogPos[0] = 0.0f;
    trackInitFogPos[1] = yFog[gTracks[gTrack].gTrackNum] - 5.0f;
    trackInitFogPos[2] = 0.0f;
    SetFogIntensity(1);
    BlowFog();
    ZeroCamera(1); UpdateCameras();
    ClearRushControls();
    inited = 1;
    currentTrackMap = -1;
    desiredTrackMap = gTrackNum;
    mapTransition = 0.0f;
}

void _UninitScreen(void)
{
    if (inited) {
        uninitButtons();
        if (blit) { RemoveBlit((_tBlit *)blit); blit = 0; }
        removeBG();
        clearTrackMap();
        clearTrackMaps();
        ZOID_Reset();
        inited = 0;
    }
}

void showTrackSelect(void)
{
    float uv[3][3];
    float speed;
    float frames;
    int i;
    int slot;
    updateButtons();
    gNumOptions = 0;
    for (i = 0; i < 21; i++) if (OptionValid(i)) gOpts[gNumOptions++] = i;
    frames = 30.0f * gFrameSeconds;
    if ((gBackwards && !gMirrorWaves) || (!gBackwards && gMirrorWaves)) {
        trackShowAngle -= 0.01745329238474369f * frames;
        if (trackShowAngle < 0.0f) trackShowAngle += 6.2831854820251465f;
    } else {
        trackShowAngle += 0.01745329238474369f * frames;
        if (trackShowAngle >= 6.2831854820251465f) trackShowAngle -= 6.2831854820251465f;
    }
    speed = frames * ((gTracks[gTrack].targetPos - gTracks[gTrack].pos[0]) / 10.0f);
    for (i = 0; i < gNumTracks; i++) {
        slot = i - gTrack;
        if (slot < 0) slot += gNumTracks;
        if (slot != gTracks[i].slot) {
            if (gTracks[i].slot == (slot-1 < 0 ? gNumTracks-1 : slot-1)) gTracks[i].targetPos -= 100.0f;
            else gTracks[i].targetPos += 100.0f;
            gTracks[i].slot = slot;
        }
        if (speed != 0.0f) {
            gTracks[i].pos[0] += speed;
            if (gTracks[i].pos[0] < -(gNumTracks*100/2)) {
                gTracks[i].pos[0] += gNumTracks*100;
                gTracks[i].targetPos += gNumTracks*100;
            } else if (gTracks[i].pos[0] > gNumTracks*100/2) {
                gTracks[i].pos[0] -= gNumTracks*100;
                gTracks[i].targetPos -= gNumTracks*100;
            }
        }
    }
    trackShowFogY += frames * ((yFog[gTracks[gTrack].gTrackNum] - 5.0f - trackShowFogY)/10.0f);
    CopyUV(unitmatrix,uv);
    YawUV(trackShowAngle,uv);
    ScaleUVXYZ(uv,uv,2.0f,1.0f,2.5f);
}

int animNewSlider(_tBlit_2 *blt)
{
    int type;
    int opt;
    signed short screenPos[2];
    type = blt->animID & 15;
    opt = blt->animID >> 16;
    if (Hidden(blt,(buttonInfo[opt+21].angle > 0.6981316804885864f && buttonInfo[opt+21].angle < 2.4434609413146973f) || buttonInfo[opt+21].alpha == 0)) return 1;
    ZOID_ProjectWindow(0,buttonInfo[opt+21].mat.mat3.pos,&gCamera[0].mat,0,screenPos);
    blt->x = (screenPos[0] - blt->width/2) / 2;
    blt->y = screenPos[1] / 2.0f - (blt->height/4/2) / 2.0f;
    blt->top = 0;
    blt->bot = blt->height/4 - 1;
    blt->left = 0;
    blt->right = blt->width-1;
    blt->alpha = buttonInfo[opt+21].alpha;
    if (blt->alpha == 255) blt->alpha = 254;
    if (buttonInfo[opt+21].angle > 1.5707963705062866f) {
        blt->top += blt->height/2;
        blt->bot += blt->height/2;
    }
    if (type == 1) {
        blt->top += blt->height/4;
        blt->bot += blt->height/4;
        switch (opt) {
        case 14: blt->right = (blt->width-1)*gFogIntensity/3; break;
        case 15: blt->right = (blt->width-1)*gWind/4; break;
        case 18: blt->right = (blt->width-1)*gDifficulty/5; break;
        case 19: blt->right = (blt->width-1)*gCatchup/2; break;
        }
    }
    UpdateBlit((_tBlit *)blt);
    return 1;
}

int animArrows(_tBlit_2 *blt)
{
    static const signed short arrowPos[2][4][2] = { 94, 177, 106, 177, 106, 177, 106, 216, 16, 151, 304, 151, 160, 135, 160, 167 };
    signed short fg, dir, size, tex;
    fg = blt->animID & 15;
    dir = (blt->animID >> 4) & 15;
    size = (blt->animID >> 8) & 15;
    tex = (blt->animID >> 12) & 15;
    if (Hidden(blt,buttonsInited == 1)) return 1;
    if (size == 1 && Hidden(blt,dir == 2)) return 1;
    if (!size) {
        if (dir == 2) { if (Hidden(blt,gFirstOption == 0)) return 1; }
        else if (dir == 3) { if (Hidden(blt,gNumOptions-gFirstOption <= 4)) return 1; }
    }
    RenameBlit((_tBlit *)blt,trackArrowNames[tex][(signed short)(gActiveOptionEnabled || dir >= 2)],1);
    if (fg) blt->alpha = TB_GetFlashTextCycle() * 255.0f;
    blt->x = arrowPos[size][dir][0] - (blt->width/2)/2;
    blt->y = arrowPos[size][dir][1] - blt->height / 2.0f / 2.0f;
    if (!size) {
        HAL_SetFont(11);
        if (!dir) blt->x -= HAL_GetTextWidth(gText.taba[gText.tab[1]+gOpts[gActiveOption]],-1);
        if (!dir || dir == 1) blt->y += (gActiveOption-gFirstOption) * 13;
    }
    if (dir == 1) blt->flip = 1;
    else if (dir == 3) gBlitInstanceList[blt->blidx].flags |= 8;
    UpdateBlit((_tBlit *)blt);
    return 1;
}

int animFan(_tBlit_2 *blt)
{
    int frame;
    frame = (blt->animID >> 8) & 0xfff;
    if (Hidden(blt, !OptionValid(15))) return 1;
    blt->top = gWind * (blt->width / 16);
    blt->bot = blt->top + blt->width / 16 - 1;
    frame += gFrameSeconds * (gWind == 2 ? 4500.0f : 3000.0f);
    frame %= 800;
    blt->left = (frame / 100) * (blt->width / 8);
    blt->right = blt->left + blt->width / 8 - 1;
    blt->animID = (blt->animID & ~0xfff00) | (frame << 8);
    UpdateBlit((_tBlit *)blt);
    { tTrackBlitInstance *inst = &gBlitInstanceList[blt->blidx]; inst->flags |= 1; }
    return 1;
}

int animText(void)
{
    int i;
    tPlayerProfile **player;
    for (i = 0; i < gNumPlayers; i++) {
        if (gPlayers[i].save != &gJustPlayers[i]) {
            player = gPlayerFiles.head;
            while (player) {
                if (gPlayers[i].save == player) break;
                player = ((tTrackProfileHead *)*player)->next;
            }
            if (!player) return 1;
        }
    }
    if (buttonsInited == 1) animButtonText();
    return 1;
}

void animButtonText(void)
{
    int i;
    int x;
    int y;
    unsigned char str1[64];
    unsigned char *str;
    signed short screenPos[2];
    int item;
    HAL_SetTextDepth(0.0f);
    TB_SetJust(0,1);
    ZOID_ProjectWindow(0,buttonInfo[42].mat.mat3.pos,&gCamera[0].mat,0,screenPos);
    x = screenPos[0]/2;
    y = (screenPos[1]-4)/2.0f;
    HAL_SetFont(13);
    TB_SetTextColor(1);
    TB_Sprintf(str1,gText.single[45]);
    TB_DrawText(TB_CenterText(str1,x),y,str1);
    HAL_SetFont(11);
    for (i = 0; i < gNumOptions; i++) {
        item = gOpts[i];
        if ((buttonInfo[item].angle <= 0.6981316804885864f || buttonInfo[item].angle >= 2.4434609413146973f) && buttonInfo[item].alpha) {
            HAL_SetTextTranslucency(buttonInfo[item].alpha);
            ZOID_ProjectWindow(0,buttonInfo[item].mat.mat3.pos,&gCamera[0].mat,0,screenPos);
            x = screenPos[0]/2 - 35.0f*gButtonScale;
            y = (screenPos[1]-4)/2.0f;
            if (buttonInfo[item].angle < 1.5707963705062866f) {
                TB_FlashTextColors(flashColors[0],flashColors[1]); TB_SetTextColor(1);
            } else {
                TB_FlashTextColors(flashColors[0],flashColors[1]); TB_SetTextColor(22);
            }
            TB_DrawText(x,y,gText.taba[gText.tab[1]+gOpts[i]]);
        }
        if ((buttonInfo[item+21].angle <= 0.6981316804885864f || buttonInfo[item+21].angle >= 2.4434609413146973f) && buttonInfo[item+21].alpha) {
            HAL_SetTextTranslucency(buttonInfo[item+21].alpha);
            ZOID_ProjectWindow(0,buttonInfo[item+21].mat.mat3.pos,&gCamera[0].mat,0,screenPos);
            x = screenPos[0]/2 - 35.0f*gButtonScale;
            y = (screenPos[1]-4)/2.0f;
            if (buttonInfo[item+21].angle < 1.5707963705062866f) {
                TB_FlashTextColors(flashColors[0],flashColors[1]); TB_SetTextColor(1);
            } else {
                TB_FlashTextColors(flashColors[0],flashColors[1]); TB_SetTextColor(22);
            }
            switch (item) {
            case 0: TB_DrawText(x,y,gTrackTitles[gTrackNum]); break;
            case 1: TB_DrawText(x,y,gText.taba[gText.tab[36]+gGhostSelect]); break;
            case 2: sprintf((char *)str1,"%d",gNumLaps); TB_DrawText(x,y,str1); break;
            case 3:
                if (gOptions[31] == 0) str = gText.single[219];
                else if (gOptions[31] == 1) str = gText.single[140];
                else str = gText.single[12];
                TB_DrawText(x,y,str); break;
            case 4: TB_DrawText(x,y,gText.taba[gText.tab[37]+(gOptions[32] != 0)]); break;
            case 5: sprintf((char *)str1,"%d",gOptions[33]); TB_DrawText(x,y,str1); break;
            case 6: sprintf((char *)str1,"%d",gOptions[34]); TB_DrawText(x,y,str1); break;
            case 7: sprintf((char *)str1,"%d",gOptions[35]); TB_DrawText(x,y,str1); break;
            case 8: TB_DrawText(x,y,gOptions[36] == 1 ? gText.single[219] : gText.single[140]); break;
            case 9: sprintf((char *)str1,"%d",gOptions[37]); TB_DrawText(x,y,str1); break;
            case 10: sprintf((char *)str1,"%d",gOptions[38]); TB_DrawText(x,y,str1); break;
            case 11: TB_DrawText(x,y,gText.taba[gText.tab[17]+(gOptions[39] != 0)]); break;
            case 12: TB_DrawText(x,y,gText.taba[gText.tab[17]+(gBackwards != 0)]); break;
            case 13:
                if (gRace == kCircuit) TB_DrawText(x,y,gText.taba[gText.tab[17]+(gCircuitTracks[gCircuitTrack].mirror != 0)]);
                else if (gOptions[27] == 2) TB_DrawText(x,y,gText.taba[gText.tab[19]+2]);
                else TB_DrawText(x,y,gText.taba[gText.tab[17]+(gOptions[27] != 0)]);
                break;
            case 17: sprintf((char *)str1,"%d",gNumDrones); TB_DrawText(x,y,str1); break;
            case 20: TB_DrawText(x,y,gText.taba[gText.tab[17]+(gDeathMode != 0)]); break;
            }
        }
    }
    TB_SetJust(0,3);
    HAL_SetTextTranslucency(-1.0f);
    HAL_SetTextDepth(-1.0f);
}

int TrackValid(int track)
{
    if (!trackReady[track]) return 0;
    if (!IsTrackUnlocked(track)) return 0;
    if (gRace == kStunt) return track >= 14 && track <= 17;
    if (gRace == kObstacle) return track >= 18 && track <= 18;
    if (gRace == kBattle) return track >= 6 && track <= 13;
    return track >= 0 && track <= 5;
}

int OptionValid(int opt)
{
    int valid;
    valid = validOption[gRace][opt];
    if (gRace == kOneRace && opt == 17) valid = gNumPlayers < 3;
    if (gRace == kStunt) {
        if (opt == 5) valid = gOptions[31] == 1;
        if (opt == 6) valid = gOptions[31] == 0;
        if (opt == 7) valid = gOptions[31] == 2;
    }
    if (gRace == kBattle) {
        if (opt == 9) valid = gOptions[36] == 0;
        if (opt == 11) valid = gOptions[36] == 0;
        if (opt == 10) valid = gOptions[36] == 1;
    }
    if (opt == 19 && gNumPlayers == 1) valid = 0;
    if (opt == 12) valid = gTrackNum >= 0 && gTrackNum <= 5;
    return valid;
}

void SetFogIntensity(int now)
{
    int red, green, blue;
    if (gFogColor < 19) gFogColor = gTrackNum;
    red = gFogColors[gFogColor][0];
    green = gFogColors[gFogColor][1];
    blue = gFogColors[gFogColor][2];
    if (now) {
        trackFogColor[0].c[0] = red;
        trackFogColor[0].c[1] = green;
        trackFogColor[0].c[2] = blue;
    } else {
        trackFogColor[0].c[0] += (red - trackFogColor[0].c[0]) / 10;
        trackFogColor[0].c[1] += (green - trackFogColor[0].c[1]) / 10;
        trackFogColor[0].c[2] += (blue - trackFogColor[0].c[2]) / 10;
    }
    trackFogColor[0].c[3] = gFogIntensity * 192 / 3;
    if (trackFogPoly) ZOID_UpdatePoly(trackFogPoly, 0, 0, 0, trackFogColor, 0, 0);
}

void BlowFog(void)
{
    tTrackFogTile *tile = &trackFogTile;
    int s;
    int speed;
    speed = gWind ? gWind * 5 : 1;
    s = tile->uls + speed;
    if (s < 0) s += 2048;
    if (s >= 2048) s -= 2048;
    tile->uls = s;
    tile->ult = 0;
}

void unloadTrackScreen(void)
{
    if (trackScreenPoly) { ZOID_RemovePoly(trackScreenPoly); trackScreenPoly = 0; }
    if (trackFogPoly) { ZOID_RemovePoly(trackFogPoly); trackFogPoly = 0; }
}

void clearTrackMap(void)
{
    unloadTrackScreen();
    if (trackMapObj != -1) { ZOID_RemoveObject(trackMapObj); trackMapObj = -1; }
}

void buildTrackMaps(void)
{
    if (gRace == kStunt) buildStuntTrackMaps();
    else if (gRace == kBattle) buildBattleTrackMaps();
    else if (gRace != kObstacle) buildRaceTrackMaps();
    trackMapsBuilt = 1;
    pathPos = 0.0f;
}

void buildStuntTrackMaps(void)
{
    int saveTrackNum;
    saveTrackNum = gTrackNum;
    for (gTrackNum = 14; gTrackNum <= 17; gTrackNum++) {
        if (IsTrackUnlocked(gTrackNum) && trackMap[gTrackNum].objIdx == -1) {
            trackMap[gTrackNum].objIdx = MBOX_NewObject(MBOX_FindObject_WORK(trackMapObjNames[gTrackNum],0,gNumModels-1,1), &trackMap[gTrackNum].mat, -1, 0x2000);
            ZOID_SetColor(trackMap[gTrackNum].objIdx, &trackColors[gTrackNum]);
            ScaleUV(unitmatrix, trackMap[gTrackNum].mat.mat3.uvs, 1.0f);
            WPitchUV(-1.5707963705062866f, trackMap[gTrackNum].mat.mat3.uvs);
            trackMap[gTrackNum].mat.mat3.pos[0] = trackStuntMapPos[0];
            trackMap[gTrackNum].mat.mat3.pos[1] = trackStuntMapPos[1];
            trackMap[gTrackNum].mat.mat3.pos[2] = trackStuntMapPos[2];
        }
    }
    gTrackNum = saveTrackNum;
}

void buildBattleTrackMaps(void)
{
    int saveTrackNum;
    saveTrackNum = gTrackNum;
    for (gTrackNum = 6; gTrackNum <= 13; gTrackNum++) {
        if (IsTrackUnlocked(gTrackNum) && trackMap[gTrackNum].objIdx == -1) {
            trackMap[gTrackNum].objIdx = MBOX_NewObject(MBOX_FindObject_WORK(trackMapObjNames[gTrackNum],0,gNumModels-1,1), &trackMap[gTrackNum].mat, -1, 0x2000);
            ZOID_SetColor(trackMap[gTrackNum].objIdx, &trackColors[gTrackNum]);
            ScaleUV(unitmatrix, trackMap[gTrackNum].mat.mat3.uvs, 1.0f);
            WPitchUV(-1.5707963705062866f, trackMap[gTrackNum].mat.mat3.uvs);
            trackMap[gTrackNum].mat.mat3.pos[0] = trackBattleMapPos[0];
            trackMap[gTrackNum].mat.mat3.pos[1] = trackBattleMapPos[1];
            trackMap[gTrackNum].mat.mat3.pos[2] = trackBattleMapPos[2];
        }
    }
    gTrackNum = saveTrackNum;
}

void loadPathData(void)
{
    if (gTrackNum >= 0 && gTrackNum <= 5) {
        pathWaveIdx = LoadWave(gTrackNum + 0x122, 0, 0, 0, 0);
        gPathWave = GetWaveInstanceAddr(pathWaveIdx);
        ReadCheckpointData();
        ReadPathData();
    } else pathWaveIdx = -1;
}

void unloadPathData(void)
{
    if (pathWaveIdx != -1) UnloadWaveInstance(pathWaveIdx, 1);
    pathWaveIdx = -1;
}

void initButtons(void)
{
    tRGBA buttonColor = {255,255,255,255};
    tTexture *tex;
    unsigned short texIdx;
    int buttonIdx;
    for (buttonIdx = 0; buttonIdx < 48; buttonIdx++) {
        if (buttonInfo[buttonIdx].objIdx == -1) {
            memcpy(buttonInfo[buttonIdx].mat.mat3.uvs,unitmatrix,36);
            buttonInfo[buttonIdx].objIdx = MBOX_NewObject(MBOX_FindObject_WORK(buttonNames[buttonInfo[buttonIdx].buttonType],0,gNumModels-1,1),&buttonInfo[buttonIdx].mat,-1,(buttonInfo[buttonIdx].buttonType == 0 || buttonInfo[buttonIdx].buttonType == 1 || buttonInfo[buttonIdx].buttonType == 2) ? 0x42000 : 0);
            ZOID_SetColor(buttonInfo[buttonIdx].objIdx,&buttonColor);
        }
    }
    tex = MBOX_FindTexture_WORK("BUTTON",&texIdx,0,gNumModels-1,1);
    ZOID_SetTexture(buttonInfo[42].objIdx,tex,-1);
    memcpy(buttonInfo[42].mat.mat3.uvs,unitmatrix,36);
    WPitchUV(-1.5707963705062866f,buttonInfo[42].mat.mat3.uvs);
    buttonInfo[42].mat.mat3.pos[0] = 0.0f;
    buttonInfo[42].mat.mat3.pos[1] = 115.0f;
    buttonInfo[42].mat.mat3.pos[2] = 200.0f;
    tex = MBOX_FindTexture_WORK("CURSOR",&texIdx,0,gNumModels-1,1);
    ZOID_SetTexture(buttonInfo[43].objIdx,tex,-1);
    tex = MBOX_FindTexture_WORK("A1",&texIdx,0,gNumModels-1,1);
    ZOID_SetTexture(buttonInfo[44].objIdx,tex,-1);
    memcpy(buttonInfo[44].mat.mat3.uvs,unitmatrix,36);
    WPitchUV(-1.5707963705062866f,buttonInfo[44].mat.mat3.uvs);
    buttonInfo[44].mat.mat3.pos[0] = 75.1f;
    buttonInfo[44].mat.mat3.pos[1] = 59.2000008f;
    buttonInfo[44].mat.mat3.pos[2] = 100.0f;
    ScaleUV(buttonInfo[44].mat.mat3.uvs,buttonInfo[44].mat.mat3.uvs,0.5f);
    memcpy(buttonInfo[45].mat.mat3.uvs,unitmatrix,36);
    WPitchUV(-1.5707963705062866f,buttonInfo[45].mat.mat3.uvs);
    WRollUV(3.1415927410125732f,buttonInfo[45].mat.mat3.uvs);
    buttonInfo[45].mat.mat3.pos[0] = 87.1f;
    buttonInfo[45].mat.mat3.pos[1] = 59.2000008f;
    buttonInfo[45].mat.mat3.pos[2] = 100.0f;
    tex = MBOX_FindTexture_WORK("B1",&texIdx,0,gNumModels-1,1);
    ZOID_SetTexture(buttonInfo[46].objIdx,tex,-1);
    memcpy(buttonInfo[46].mat.mat3.uvs,unitmatrix,36);
    WPitchUV(-1.5707963705062866f,buttonInfo[46].mat.mat3.uvs);
    buttonInfo[46].mat.mat3.pos[0] = -75.1f;
    buttonInfo[46].mat.mat3.pos[1] = 59.2000008f;
    buttonInfo[46].mat.mat3.pos[2] = 100.0f;
    ScaleUV(buttonInfo[46].mat.mat3.uvs,buttonInfo[46].mat.mat3.uvs,0.5f);
    memcpy(buttonInfo[47].mat.mat3.uvs,unitmatrix,36);
    WPitchUV(-1.5707963705062866f,buttonInfo[47].mat.mat3.uvs);
    buttonInfo[47].mat.mat3.pos[0] = -87.1f;
    buttonInfo[47].mat.mat3.pos[1] = 59.2000008f;
    buttonInfo[47].mat.mat3.pos[2] = 100.0f;
    buttonsInited = 1;
    buttonsJustInited = 1;
    ZeroCameras();
}

void uninitButtons(void)
{
    int buttonIdx;
    for (buttonIdx = 0; buttonIdx < 48; buttonIdx++) {
        if (buttonInfo[buttonIdx].objIdx != -1) {
            if (gGameState & 0x7c03fffe) ZOID_RemoveObject(buttonInfo[buttonIdx].objIdx);
            buttonInfo[buttonIdx].objIdx = -1;
        }
    }
    buttonsInited = 0;
}

void updateButtons(void)
{
    int buttonIdx;
    float prevPos[3];
    float buttonY;
    float selectedPos;
    signed char clearScrollSpeed = 1;
    tTexture *tex;
    unsigned short texIdx;
    tRGBA buttonColor = {255,255,255,255};
    if (buttonsInited) {
        if (gButtonScale * ((gNumOptions-1)*gButtonSpace) > maxY-minY)
            selectedPos = maxY - gActiveOption*(maxY-minY)/(gNumOptions-1);
        else selectedPos = maxY - gButtonScale*(gActiveOption*gButtonSpace);
        memcpy(prevPos,buttonInfo[43].mat.mat3.pos,12);
        buttonInfo[43].angle += 6.2831854820251465f*gFrameSeconds;
        if (buttonInfo[43].angle > 6.2831854820251465f) buttonInfo[43].angle -= 6.2831854820251465f;
        buttonInfo[43].mat.mat3.pos[0] = buttonX*cursorZ/buttonZ + -180.0f*gButtonScale;
        buttonInfo[43].mat.mat3.pos[1] = selectedPos*cursorZ/buttonZ;
        buttonInfo[43].mat.mat3.pos[2] = cursorZ;
        if (!buttonsJustInited) {
            if (60.0*fabs(buttonInfo[43].mat.mat3.pos[1]-prevPos[1])/10.0 > trackCursorSpeed) {
                trackCursorSpeed = 60.0*fabs(buttonInfo[43].mat.mat3.pos[1]-prevPos[1])/10.0;
                trackCursorSpeed = trackCursorSpeed < 540.0f*gButtonScale ? trackCursorSpeed : 540.0f*gButtonScale;
                trackCursorSpeed = trackCursorSpeed > 180.0f*gButtonScale ? trackCursorSpeed : 180.0f*gButtonScale;
            }
            if (buttonInfo[43].mat.mat3.pos[1] > prevPos[1]+trackCursorSpeed*gFrameSeconds)
                buttonInfo[43].mat.mat3.pos[1] = prevPos[1]+trackCursorSpeed*gFrameSeconds;
            else if (buttonInfo[43].mat.mat3.pos[1] < prevPos[1]-trackCursorSpeed*gFrameSeconds)
                buttonInfo[43].mat.mat3.pos[1] = prevPos[1]-trackCursorSpeed*gFrameSeconds;
            else trackCursorSpeed = 180.0f*gButtonScale;
        }
        ScaleUV(unitmatrix,buttonInfo[43].mat.mat3.uvs,0.4f*gButtonScale);
        WRollUV(buttonInfo[43].angle,buttonInfo[43].mat.mat3.uvs);
        WPitchUV(1.5707963705062866f,buttonInfo[43].mat.mat3.uvs);
        buttonY = selectedPos + gButtonScale*(gActiveOption*gButtonSpace);
        for (buttonIdx = 0; buttonIdx < 21; buttonIdx++) {
            if (buttonIdx == gOpts[gActiveOption]) {
                if (buttonInfo[buttonIdx].angle < 3.1415927410125732f) buttonInfo[buttonIdx].angle += 12.566370964050293f*gFrameSeconds;
                if (buttonInfo[buttonIdx].angle > 3.1415927410125732f) buttonInfo[buttonIdx].angle = 3.1415927410125732f;
                if (buttonsJustInited) buttonInfo[buttonIdx].angle = 3.1415927410125732f;
            } else {
                if (buttonInfo[buttonIdx].angle > 0.0f) buttonInfo[buttonIdx].angle -= 12.566370964050293f*gFrameSeconds;
                if (buttonInfo[buttonIdx].angle < 0.0f) buttonInfo[buttonIdx].angle = 0.0f;
                if (buttonsJustInited) buttonInfo[buttonIdx].angle = 0.0f;
            }
            if ((gRace != kCircuit || (gCircuitTrack == 0 && buttonIdx == 20)) && (gRace != kGhost || buttonIdx != 2)) buttonInfo[buttonIdx+21].angle = buttonInfo[buttonIdx].angle;
            else buttonInfo[buttonIdx+21].angle = 0.0f;
            memcpy(prevPos,buttonInfo[buttonIdx].mat.mat3.pos,12);
            buttonInfo[buttonIdx].mat.mat3.pos[0] = buttonX + -155.0f*gButtonScale;
            buttonInfo[buttonIdx].mat.mat3.pos[1] = buttonY;
            buttonInfo[buttonIdx].mat.mat3.pos[2] = buttonZ;
            if (!buttonsJustInited) {
                if (60.0*fabs(buttonInfo[buttonIdx].mat.mat3.pos[1]-prevPos[1])/10.0 > trackScrollSpeed) {
                    trackScrollSpeed = 60.0*fabs(buttonInfo[buttonIdx].mat.mat3.pos[1]-prevPos[1])/10.0;
                    trackScrollSpeed = trackScrollSpeed < 540.0f*gButtonScale ? trackScrollSpeed : 540.0f*gButtonScale;
                    trackScrollSpeed = trackScrollSpeed > 180.0f*gButtonScale ? trackScrollSpeed : 180.0f*gButtonScale;
                }
                if (buttonInfo[buttonIdx].mat.mat3.pos[1] > prevPos[1]+trackScrollSpeed*gFrameSeconds) {
                    buttonInfo[buttonIdx].mat.mat3.pos[1] = prevPos[1]+trackScrollSpeed*gFrameSeconds;
                    clearScrollSpeed = 0;
                } else if (buttonInfo[buttonIdx].mat.mat3.pos[1] < prevPos[1]-trackScrollSpeed*gFrameSeconds) {
                    buttonInfo[buttonIdx].mat.mat3.pos[1] = prevPos[1]-trackScrollSpeed*gFrameSeconds;
                    clearScrollSpeed = 0;
                }
            }
            buttonInfo[buttonIdx+21].mat.mat3.pos[0] = buttonX + 50.0f*gButtonScale;
            buttonInfo[buttonIdx+21].mat.mat3.pos[1] = buttonInfo[buttonIdx].mat.mat3.pos[1];
            buttonInfo[buttonIdx+21].mat.mat3.pos[2] = buttonInfo[buttonIdx].mat.mat3.pos[2];
            ScaleUV(unitmatrix,buttonInfo[buttonIdx].mat.mat3.uvs,gButtonScale);
            ScaleUV(unitmatrix,buttonInfo[buttonIdx+21].mat.mat3.uvs,gButtonScale);
            WPitchUV(3.1415927410125732f + (buttonInfo[buttonIdx].angle - 1.5707963705062866f),buttonInfo[buttonIdx].mat.mat3.uvs);
            WPitchUV(3.1415927410125732f + (buttonInfo[buttonIdx+21].angle - 1.5707963705062866f),buttonInfo[buttonIdx+21].mat.mat3.uvs);
            if (buttonInfo[buttonIdx].angle > 1.5707963705062866f) tex = MBOX_FindTexture_WORK("BUTTON_SELECT",&texIdx,0,gNumModels-1,1);
            else tex = MBOX_FindTexture_WORK("BUTTON",&texIdx,0,gNumModels-1,1);
            ZOID_SetTexture(buttonInfo[buttonIdx].objIdx,tex,-1);
            if (buttonInfo[buttonIdx+21].angle > 1.5707963705062866f) tex = MBOX_FindTexture_WORK("BUTTON_SELECT",&texIdx,0,gNumModels-1,1);
            else tex = MBOX_FindTexture_WORK("BUTTON",&texIdx,0,gNumModels-1,1);
            ZOID_SetTexture(buttonInfo[buttonIdx+21].objIdx,tex,-1);
            if (buttonInfo[buttonIdx].mat.mat3.pos[1] > maxY+gButtonSpace*gButtonScale || buttonInfo[buttonIdx].mat.mat3.pos[1] < minY-gButtonSpace*gButtonScale) buttonInfo[buttonIdx].alpha = 0;
            else if (buttonInfo[buttonIdx].mat.mat3.pos[1] > maxY) buttonInfo[buttonIdx].alpha = 80;
            else if (buttonInfo[buttonIdx].mat.mat3.pos[1] < minY) buttonInfo[buttonIdx].alpha = 80;
            else buttonInfo[buttonIdx].alpha = 255;
            if (!OptionValid(buttonIdx)) buttonInfo[buttonIdx].alpha = 0;
            buttonInfo[buttonIdx+21].alpha = buttonInfo[buttonIdx].alpha;
            buttonColor.c[3] = buttonInfo[buttonIdx].alpha;
            ZOID_SetColor(buttonInfo[buttonIdx].objIdx,&buttonColor);
            ZOID_SetColor(buttonInfo[buttonIdx+21].objIdx,&buttonColor);
            if (!OptionValid(buttonIdx)) {
                MBOX_HideObject(buttonInfo[buttonIdx].objIdx,0,15);
                MBOX_HideObject(buttonInfo[buttonIdx+21].objIdx,0,15);
            } else if (buttonInfo[buttonIdx].alpha == 0) {
                MBOX_HideObject(buttonInfo[buttonIdx].objIdx,0,15);
                MBOX_HideObject(buttonInfo[buttonIdx+21].objIdx,0,15);
                buttonY -= gButtonSpace*gButtonScale;
            } else {
                MBOX_ShowObject(buttonInfo[buttonIdx].objIdx,0,15);
                MBOX_ShowObject(buttonInfo[buttonIdx+21].objIdx,0,15);
                buttonY -= gButtonSpace*gButtonScale;
            }
        }
        if (clearScrollSpeed == 1) trackScrollSpeed = 0.0f;
        buttonsJustInited = 0;
    }
}
