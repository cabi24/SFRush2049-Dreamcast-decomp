#pragma inline(OncePerFrame)
#include "game_types.h"
const float gravityArray[4]={1.0f,1.15f,1.3f,1.5f};
static const tBlitDef inactiveBlitDef[1]={{0,0,0,320,220,-1,-1,-1,-1,0,128,func_8C0291A6,0}};
signed char gRestartGame=0;
signed char gGamePointPlayed=0;
unsigned char gFogColors[27][3] = {
    150, 150, 190, 150, 150, 190, 140, 140, 140, 0, 0, 0, 140, 140, 140, 0, 0, 0, 50, 50, 50, 150,
    150, 190, 150, 150, 190, 0, 0, 0, 150, 150, 190, 150, 150, 190, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 150, 150, 190, 50, 50, 50, 0, 0, 0, 0, 0, 0, 150, 0, 0, 0, 150, 0, 0, 0, 190, 150, 150,
    0, 255, 255, 255, 150, 100, 0, 180, 0, 100
};
int playerTextColor[4] = { 7, 5, 14, 12 };
tRGBA flashColors[2] = { 224, 224, 0, 255, 0, 0, 0, 255 };

enum_kNumRaceTypes_8 gRace;
enum_kNumStuntModes gStuntMode;
enum_kNumStuntFlows gStuntFlow;
int gStuntModeMins;
int gStuntModePoints;
int gStuntModeDeaths;
enum_kNumBattleModes gBattleMode;
int gBattleModeMins;
int gBattleModePoints;
signed char gCoastFlag;
signed char gOutOfTime;
signed char gEndGame;
signed char gRetireGame;
signed char gDifficulty;
signed char gGhostSelect;
signed short gNumDrones;
signed short gNumLaps;
signed char gDeathMode;
signed char gBackwards;
signed char gFogColor;
unsigned int gFrames;

_tBlit *inactiveBlit=0;

int GameLoop(void)
{
 int renderFlag=0;
 static signed char sInited=0;
 static signed char pauseAllowed=0;
 if(!sInited) {(*(volatile unsigned int *)&gFrames)=0;sInited=1;}
 OncePerFrame();
 if(game_InactiveSeconds()>300.0f && !(gGameState&0x4000000f)) {
  if(!inactiveBlit)inactiveBlit=NewMultiBlit(0,0,inactiveBlitDef,1);
 }else if(inactiveBlit) {RemoveBlit(inactiveBlit);inactiveBlit=0;}
 updateGameState();
 if(gGameState&0x7c03fffe)updateFrontEnd();
 if(gGameState&0x600000) {if(pauseAllowed)updateSelectPauseScreen();}else pauseAllowed=0;
 if(!(gGameState&0x200000)||pauseAllowed)func_8C0279EA();
 if(gGameState&0x600000)pauseAllowed=1;
 if((gGameState&0x600000)&&gPauseState) {UpdateGraphics(1,1);return 1;}
 if(gGameState&0x3fc0000)_UpdateGame();
 if(gNewGameState==gGameState) {func_8C02901E();UpdateGraphics(1,1);renderFlag=1;}
 (*(volatile unsigned int *)&gFrames)++;return renderFlag;
}
void OncePerFrame(void)
{UpdateFrameTimer();UpdateRushControls();HAL_CallOncePerFrameFuncs();}
void func_8C0279EA(void)
{
 static signed char seen[4]={0,0,0,0};
 int i; signed char choice1,choice2;
 if(!game_ControllerBypass()) {
  for(i=0;i<gNumWindows;i++) {
   if(gControllerData[i].attached)seen[i]=1;
   if(seen[i]&&!gControllerData[i].attached) {
    seen[i]=0;PS_ErrorFunc(i*2,10,0,0,&choice1,&choice2,0);
   }
  }
 }
}
void UpdateFrame(void)
{
 func_8C0279EA();func_8C02901E();UpdateGraphics(0,0);EndGraphics();checkForExit(1);
 if(gExitGame)ExitSystem();StartGraphics();OncePerFrame();
}

void _UpdateGame(void)
{
    static signed char startCountdown = 0; /* explicit `= 0`: emitted at this point, in the target .sbss order */
    int i;
    signed char allSolid;
    int p;
    int numDeaths;
    if (gGameState & 0x40000) {
        for (i = 0; i < gNumPlayers; i++) gPakInserted[i] = (*gPlayers[i].save)->fileh != 0;
        gSkipToGameState = 1;
        SetOptions();
        _CalcPolePositions();
        gUseCatchup = gCatchup;
        setGameState(0x80000);
        goto preplay;
    } else if (gGameState & 0x80000) {
    preplay:
        if (PrePlay(1, 0)) {
            startCountdown = 1;
            setGameState(0x200000);
            ClearRushControls();
            goto countdown;
        } else {
            SaveServer(0, 1);
            goto done;
        }
    } else if (gGameState & 0x200000) {
    countdown:
        if (gRestartGame) goto done;
        CarSetup();
        Update_MDrive();
        TextureAnimation();
        CheckTargetCollision(model[0].slot);
        if (startCountdown) {
            SetCountdownTimer(3.5f);
            init_model_task(3.5f * gRetraceRate);
            startCountdown = 0;
        }
        CountDown();
        _CheckButtons();
    } else if (gGameState & 0x100000) {
        if (gRace == kPractice) SetCountdownTimer(1800.0f);
        else if (gRace == kStunt) {
            if (gStuntMode == 0) SetCountdownTimer(gStuntModeMins * 60);
            else SetCountdownTimer(600.0f);
        } else if (gRace == kObstacle) SetCountdownTimer(300.0f);
        else if (gRace == kBattle) {
            if (gBattleMode == 1) SetCountdownTimer(gBattleModeMins * 60);
            else SetCountdownTimer(1200.0f);
        } else SetCountdownTimer(CalcRaceTime());
        PlayGameSound(84, 0, 1, 0);
        StartGameMusic();
        InitCPTime();
        setGameState(0x400000);
    } else if (gGameState & 0x400000) {
        PlayGame();
        _CheckButtons();
        UpdateStationaryEmitters();
        if (gRetireGame) {
            FindWinner();
            setGameState(0x1000000);
            GhostEnd();
            RemoveHUD();
            goto done;
        }
        if (gCoastFlag) {
            allSolid = 1;
            for (p = 0; p < gNumPlayers; p++) {
                if (!gDeathMode && !AreWeSolid(p)) allSolid = 0;
            }
            if (are_all_cars_stopped() && allSolid) {
                if (gRace == kStunt) FindWinner();
                setGameState(0x1000000);
                GhostEnd();
                RemoveHUD();
            }
            if (!CountdownTimeOut() && gRace != kStunt && gRace != kObstacle && gRace != kBattle) {
                gCoastFlag = 0; gOutOfTime = 0;
            }
        } else if (gRace == kStunt) {
            switch (gStuntMode) {
            case 0:
                if (CountdownTimeOut()) { gCoastFlag = 1; gOutOfTime = 1; }
                break;
            case 1:
                for (i = 0; i < gNumPlayers; i++) {
                    if (gStuntPlayer[i].totalPoints >= gStuntModePoints) { gCoastFlag = 1; gOutOfTime = 1; }
                }
                break;
            case 2:
                numDeaths = 0;
                for (i = 0; i < gNumPlayers; i++) {
                    if (gGameCar[i].player->numCrashes >= gStuntModeDeaths) { gGameCar[i].dead = 1; numDeaths++; }
                }
                if (numDeaths == gNumPlayers) { gCoastFlag = 1; gOutOfTime = 1; }
                break;
            }
        } else if (gRace == kBattle) {
            switch (gBattleMode) {
            case 1:
                if (CountdownTimeOut()) { FindWinner(); gCoastFlag = 1; gOutOfTime = 1; }
                break;
            case 0:
                memset(gTeamPoints, 0, 4);
                for (i = 0; i < gNumPlayers; i++) gTeamPoints[gCarTeam[i]] += gGameCar[i].battle.Points;
                for (p = 0; p < 4; p++) {
                    if (!gGamePointPlayed && gTeamPoints[p] == gBattleModePoints - 1) {
                        gGamePointPlayed = 1;
                        PlayGameSound(54, 0, 2, 0);
                    }
                    if (gTeamPoints[p] >= gBattleModePoints) {
                        FindWinner(); gCoastFlag = 1; gOutOfTime = 1;
                    }
                }
                break;
            }
        } else if (gRace == kOneRace || gRace == kPractice) {
            for (i = 0; i < gNumPlayers; i++) if (!gGameCar[gPlayers[i].slot].place_locked) goto done;
            gAPlayerReallyFinished = 0;
            for (i = 0; i < gNumPlayers; i++) if (gNumLaps == model[i].laps) gAPlayerReallyFinished = 1;
            if (!gCoastFlag) {
                if (gDeathMode) gDeathWaitEnd = 0.0f;
                gCoastFlag = 1;
                FindWinner();
                AudSetMusicVol(0.0f, 3.0f);
            }
        } else {
            if (CountdownTimeOut()) {
                if (!gCoastFlag) {
                    gCoastFlag = 1;
                    FindWinner();
                    AudSetMusicVol(0.0f, 3.0f);
                }
                for (i = 0; i < gNumPlayers; i++) {
                    if (gNumLaps != model[gPlayers[i].slot].laps) {
                        if (gRace != kGhost || !gGhosts[gPlayers[i].slot] || (*(*gGhosts[gPlayers[i].slot])->header)->flag < 0)
                            PlayGameSound(87, 0, 1, 0);
                        gOutOfTime = 1;
                        break;
                    }
                }
            }
        }
    } else if (gGameState & 0x1000000) {
        _EndGame();
    } else if (gGameState & 0x2000000) updateHighScoreScreen();
    else if (gGameState & 0x800000) updateGameOverScreen();
    done:
    if (gGameState & 0x600000) {
        for (i = 0; i < gNumWindows; i++) WorldFogDensity(i, gCamera[i].mat.uvs[3]);
    }
}

int PrePlay(int useHud, int cancel)
{
    static signed char inited = 0; /* explicit `= 0`: emitted at this point, in the target .sbss order */
    int i, slot, rrr;
    if (cancel) { inited = 0; return 0; }
    if (!inited) {
        inited = 1;
        SetOptions();
        EnableGameSounds(!(gGameState & 8) && !(gGameState & 0x2000000));
        if (gGameState & 0x2000000) {
            /* Reference setup semantics; DC candidate uses ordinary short lvalues. */
            num_our_drones = 0;
            num_drones = 0;
            num_humans = 0;
            for (i = 0; i < gNumPlayers; i++) {
                slot = gPlayers[i].slot;
                gLink[slot].owner -= 6;
                gGameCar[gPlayers[i].slot].dead = 0;
                gGameCar[gPlayers[i].slot].place_locked = 0;
                gGameCar[gPlayers[i].slot].player = 0;
                model[gPlayers[i].slot].drone_type = 1;
                model[gPlayers[i].slot].we_control = 1;
                model[gPlayers[i].slot].autotrans = 1;
                model[gPlayers[i].slot].appearance = 0;
                gGameCar[gPlayers[i].slot].appearance = 0;
                SetupCar(gPlayers[i].slot, model[gPlayers[i].slot].body_type, 1);
                drones[num_drones++] = i;
                our_drones[num_our_drones++] = i;
            }
            for (; i < gNumActiveCars; i++) {
                gGameCar[i].dead = 0;
                gGameCar[i].place_locked = 0;
                model[i].appearance = 0;
                gGameCar[i].appearance = 0;
                SetupCar(i, model[i].body_type, 1);
                drones[num_drones++] = i;
                our_drones[num_our_drones++] = i;
            }
            sched_index = -1;
            sched_to_use = num_our_drones;
            new_sched_to_use = num_our_drones;
            cur_drone = 0;
            _CalcPolePositions();
            InitCPS();
            InitDrones();
            gCoastFlag = 0;
            gRetireGame = 0;
            gGravity = 32.200001f;
            for (i = 0; i < gNumActiveCars; i++) Init_MDrive(0, model[i].slot);
            ShowHUD();
            inited = 0;
            return 1;
        }
        HFOV = 90.0f;
        if (!(gGameState & 8)) AudKillSong();
        if (gRestartGame) { ZOID_Reset(); InitBlits(); }
        else if (!(gGameState & 8)) InitWaves();
        munge_gLink_data();
        GhostStart();
        ResetHighScores();
    }
    if (gFogColor < 19) gFogColor = gTrackNum;
    if (!(gGameState & 8)) {
        HFOV = 90.0f;
        ZOID_ResetWindows();
        gNumWindows = gRace == kGhost ? (short)1 : gNumPlayers; ZOID_SplitScreen(gNumWindows);
    }
    if (gRestartGame) ReloadTrack();
    else {
        rrr = LoadTrack(gTrackNum, 1, 0);
        if (!rrr) return 0;
    }
    InitTextureAnimation();
    gRestartGame = 0;
    for (i = 0; i < gNumWindows; i++) {
        ZOID_SetFog(0, 0, gFogColors[gFogColor][0], gFogColors[gFogColor][1], gFogColors[gFogColor][2], 255, i);
        WorldFogDensity(i, gCamera[i].mat.uvs[3]);
    }
    if (!(gGameState & 8)) AudStopAllEffects();
    InitCPS();
    init_cars();
    for (i = 0; i < 6; i++) carsetup_flag[i] = 0;
    InitDrones();
    gCoastFlag = 0; gRetireGame = 0; gOutOfTime = 0;
    gGravity = 32.200001f * gravityArray[gGravityScale];
    for (i = 0; i < gNumActiveCars; i++) {
        slot = model[i].slot;
        if (gLink[slot].owner == 0 || gLink[slot].owner == 6) Init_MDrive(0, slot);
    }
    StartEnvironment();
    init_views();
    if (useHud) ShowHUD();
    for (i = 0; i < gNumPlayers; i++) {
        gPlayers[i].numCrashes = 0;
        gGameCar[i].battle.health = 800;
        gGameCar[i].battle.Points = 0;
        gGameCar[i].battle.reloadingTime = 0.0f;
        if (gRace == kBattle) InitBattlePlayer((signed short)i);
        gGamePointPlayed = 0;
    }
    if (!(gGameState & 8)) ClearRushControls();
    StartListeners();
    if (gRace == kBattle) { InitBattlePoints(); ShotInit(); }
    if (gRace == kStunt) InitStunt();
    if (!(gGameState & 8)) PS_RumbleGo();
    inited = 0;
    gUpsideDownTrack = gGameCheatStateBytes[3];
    return 1;
}

void PlayGame(void)
{
    int i;
    signed short slot;
    int camera;
    tModelData *m;
    tCarData *car;
    Update_MDrive();
    if (gRace == kStunt) CheckForStunt();
    else if (gRace == kBattle) { ShotUpdate(); UpdateBattlePowerUps(); }
    else {
        if (gRace == kObstacle) GetResPt();
        else DoDrones();
        CheckCPs();
    }
    UpdateSmoke();
    UpdateArrowVis();
    for (i = 0, m = model, car = gGameCar; i < gNumActiveCars; i++, m++, car++) {
        /* DC restart dispatch has negative and positive byte tests. */
        signed char restart = car->restart;
        slot = m->slot;
        if (restart < 0) {
            camera = car->camera;
            if (!(gGameState & 8) && slot < gNumPlayers) gPlayers[slot].numCrashes++;
            if (camera >= 0) {
                car->saveview = car->view;
                car->view = 4;
                gCamera[camera].fcam.uvs[3][0] = gCamera[camera].mat.uvs[3][0];
                gCamera[camera].fcam.uvs[3][1] = gCamera[camera].mat.uvs[3][1];
                gCamera[camera].fcam.uvs[3][2] = gCamera[camera].mat.uvs[3][2];
            }
            StartBlast(&slot, 1, 1.0f, 0);
            StopCarSounds(m);
            if (m->drone_type == 2) {
                if (gRace != kGhost || !gGhosts[m->slot] || (*(*gGhosts[m->slot])->header)->flag < 0)
                    PlayGameSound(9, m->slot, 1, 1);
            } else EmitGameSound(m->rwr, unitmatrix[3], 400.0f, 0.0f, 1.0f, 0.0f, 9, m->slot, 0, 128);
            if (gRace == kStunt) CrashedStunt(slot);
            car->restart = 0;
        } else if (car->restart > 0) {
            InitSmokeBlast(slot);
            if (car->camera >= 0) {
                car->view = car->saveview;
                SetCameraView(car->camera);
            }
            car->restart = 0;
        }
        UpdateVisuals(slot);
        if (gLink[slot].owner == 0 || gLink[slot].owner == 6) {
            if (gRace != kGhost || (gRace == kGhost && slot == 0)) CheckTargetCollision(slot);
        }
    }
    TextureAnimation();
    DetectDroneAbort();
    if (!gRestartGame) ShowHUD();
}

void AbortGame(void)
{
    signed short i;
    ShowCountdown(0);
    RemoveHUD();
    RemoveMultiplayerHUD();
    for (i = 0; i < gNumActiveCars; i++) End_MDrive(model[i].slot);
    modelstop();
    StopListeners();
    RemoveEnvirons();
    AudStopAllEffects();
    if (gRestartGame) setGameState(0x40000);
    else {
        GhostEnd();
        UnloadTrack(gTrackNum);
        gNumWindows = 1;
        ZOID_SplitScreen(1);
        setGameState(4);
        updateGameState();
        updateTitleScreen();
    }
    ClearRushControls();
}

void _EndGame(void)
{
    signed short i;
    for (i = 0; i < gNumActiveCars; i++) {
        End_MDrive(model[i].slot);
        gGameCar[i].appearance &= ~8;
        UpdateVisuals(gGameCar[i].slot);
    }
    modelstop();
    StopListeners();
    SetCountdownTimer(4.0f);
    setGameState(0x2000000);
}

void CarSetup(void)
{
    signed short i;
    signed short slot;
    for (i = 0; i < gNumActiveCars; i++) {
        slot = model[i].slot;
        if (model[slot].data_valid) {
            if (!carsetup_flag[slot]) SetupCar(slot, model[slot].body_type, 1);
            carsetup_flag[slot] = 1;
            UpdateVisuals(slot);
        }
    }
}

void _CheckButtons(void)
{
    signed short i;
    for (i = 0; i < gNumPlayers; i++) {
    }
}

void _CalcPolePositions(void)
{
    int i;
    int j;
    int max_drones;
    unsigned char nextSlot;
    anon_track_car_a *lp;
    nextSlot = 0;
    max_drones = gNumDrones;
    if (max_drones > 6 - gNumPlayers) max_drones = 6 - gNumPlayers;
    if (gRace == kStunt || gRace == kObstacle || gRace == kBattle) max_drones = 0;
    gNumCars = gNumPlayers + max_drones;
    for (i = 0, lp = gLink; i < 6; i++, lp++) {
        if (gRace == kCircuit && i < gNumCars && gCircuitTrack > 0) {
            int slot;
            if (i == 0) slot = gCircuitPlayer;
            else slot = i == 1 ? gCircuitPlayer ^ 1 : i;
            lp->polepos = gCircuitCars[slot].totalPlace;
            lp->flags = 0xb0;
            if (i < gNumPlayers) lp->owner = 6;
            else lp->owner = 0;
        } else if (i < gNumPlayers) {
            lp->polepos = max_drones++;
            lp->owner = 6;
            lp->flags = 0xb0;
        } else if (i < gNumCars) {
            if (gRace == kCircuit) {
                for (;;) {
                    lp->polepos = Random(gNumCars);
                    for (j = 0; j < i; j++) if (lp->polepos == gLink[j].polepos) break;
                    if (i == j) break;
                }
            } else lp->polepos = nextSlot++;
            lp->owner = 0;
            lp->flags = 0xb0;
        } else {
            lp->owner = 7;
            lp->flags = 0;
        }
        lp->track = gTrackNum;
    }
}

void func_8C02901E(void)
{
 char controller[8],str[256];
 static char *portLetters="ABCD";
 static int x=320;
 int numPlayers;
 numPlayers=gRace==kGhost?1:gNumPlayers;
 if(game_ControllerBypass()) {x=320;return;}
 for(game_WarningPlayer=0;game_WarningPlayer<numPlayers;game_WarningPlayer++)
  if(!gControllerData[game_WarningPlayer].attached&&PakErrorActive(game_WarningPlayer)!=10)break;
 if(game_WarningPlayer>=numPlayers) {x=320;return;}
 controller[0]=portLetters[game_WarningPlayer];controller[1]=0;
 sprintf(str,"%s%s%s%s%s%d%s",game_Localized->parts[0],controller,game_Localized->parts[1],controller,game_Localized->parts[2],numPlayers,game_Localized->parts[3]);
 HAL_SetFont(11);TB_SetTextColor(22);
 if(!(gGameState&3)) {
  if((gGameState&0x40000000)&&!PakErrorActive(game_WarningPlayer)&&!PakErrorActive(game_WarningPlayer+1))TB_DrawText(x,20,str);
  else if(!(gGameState&4)&&!(gGameState&8))TB_DrawText(x,190,str);
  else TB_DrawText(x,20,str);
 }
 x--;
 if(x<-(int)HAL_GetTextWidth(str,-1))x=320;
}
int func_8C0291A6(_tBlit *blt)
{blt->palette=game_InactiveDescription;UpdateBlit(blt);return 1;}
