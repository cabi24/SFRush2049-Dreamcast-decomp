/* Private Dreamcast pakerr reconstruction. Target proof in evidence/layout-proof.json. */
#include "pakerr_types.h"
enum { kPS_ErrUnknown=14,kPS_ErrDevice=13,kPS_ErrCorruptFile=12,kPS_ErrRemoved=11,kPS_ErrRecognize=10,kPS_ErrBadData=9,kPS_ErrExist=8,kPS_ErrFail=7,kPS_ErrFull=6,kPS_ErrComm=5,kPS_ErrInconsistent=4,kPS_ErrNewPak=3,kPS_ErrNoPak=2,kPS_ErrNoCont=1,kPS_ErrNone=0 };
/* Externally read/reset by warning screen; semantic state, not padding. */
signed char byte_8C0BF760=0;
int _AnimDim(_tBlit *blt);
void _ControlChoice(int pak, int id)
{
    if (gPakError[pak][id].edges & 0xc00) {
        PlayLRSound(gPakError[pak][id].edges);
        gPakError[pak][id].choice1 = !gPakError[pak][id].choice1;
    }
    if (gPakError[pak][id].edges & 10) {
        AudPlayEffect(0, 0, 1, 0);
        gPakError[pak][id].done = 1;
    }
}

void _ControlPakErrNQA(int pak, int id)
{
    if (gPakError[pak][id].edges & 15) {
        PlayStartABSound(gPakError[pak][id].edges);
        gPakError[pak][id].done = 1;
    }
}

void _ControlPakErrTimeout(int pak, int id)
{
    if (gPakError[pak][id].timeout < gFrameCounter)
        gPakError[pak][id].done = 1;
}

void _DrawPakHeading(int pak,int id)
{
 unsigned char str[32];
 HAL_SetFont(11); TB_SetTextColor(1);
 sprintf((char *)str,"%s %s",gText.single[167],gPS_PakName[pak]);
 TB_DrawText(TB_CenterText(str,gPakError[pak][id].left+gPakError[pak][id].width/2),gPakError[pak][id].top,str);
}

void _DrawChoice(int pak, int id, const unsigned char *str, const unsigned char *str1, const unsigned char *str2, int choice1)
{
    int x, y, h;
    TB_SetTextColor(1);
    HAL_SetFont(11);
    TB_SetJust(1, 1);
    TB_DrawTextScrollBox(gPakError[pak][id].left + gPakError[pak][id].width / 2,
                        gPakError[pak][id].top + 20 + (gPakError[pak][id].height - 40) / 2,
                        gPakError[pak][id].width, gPakError[pak][id].height - 40,
                        -1, 0, (void *)str);
    h = TB_GetFontLineHeight();
    x = gPakError[pak][id].left + gPakError[pak][id].width / 2;
    y = gPakError[pak][id].top + gPakError[pak][id].height - h * 3;
    HAL_SetFont(choice1 ? 11 : 10);
    TB_SetTextColor(choice1 ? 1 : 22);
    TB_DrawText(x, y - (choice1 == 0) + h, gText.single[245]);
    HAL_SetFont(choice1 ? 10 : 11);
    TB_SetTextColor(choice1 ? 22 : 1);
    TB_DrawText(x, y - (choice1 ? 1 : 0), gText.taba[gText.tab[26]]);
    TB_SetJust(0, 3);
}

void _DrawPakErrNoController(int pak, int id)
{
    unsigned char str[256];
    HAL_SetFont(11);
    TB_SetTextColor(1);
    TB_Strcpy(str, gText.single[109]);
    TB_Strcat(str, "\n\n");
    TB_Strcat(str, gText.single[110]);
    TB_Strcat(str, gText.single[112]);
    TB_SetJust(1, 1);
    TB_DrawTextScrollBox(gPakError[pak][id].left + gPakError[pak][id].width / 2,
                        gPakError[pak][id].top + 20 + (gPakError[pak][id].height - 20) / 2,
                        gPakError[pak][id].width, gPakError[pak][id].height - 20,
                        -1, 0, str);
    TB_SetJust(0, 3);
    HAL_SetFont(11);
}

void _DrawPakErrNoPak(int pak,int id)
{
 int x,y,h;
 unsigned char str[256];
 int ask;
 ask=!(gGameState & 0x7c03fffe);
 HAL_SetFont(11); TB_SetTextColor(1);
 TB_Strcpy(str,gText.single[114]);
 if (ask) TB_Strcat(str,gText.single[113]);
 else { TB_Strcat(str,gText.single[110]); TB_Strcat(str,gText.single[112]); }
 TB_SetJust(1,1);
 TB_DrawTextScrollBox(gPakError[pak][id].left+gPakError[pak][id].width/2,
 gPakError[pak][id].top+20+(gPakError[pak][id].height-(ask?40:20))/2,
 gPakError[pak][id].width,gPakError[pak][id].height-(ask?40:20),-1,0,str);
 if (ask) {
  h=TB_GetFontLineHeight();
  x=gPakError[pak][id].left+gPakError[pak][id].width/2;
  y=gPakError[pak][id].top+gPakError[pak][id].height-h*3;
  HAL_SetFont(gPakError[pak][id].choice1?11:10);
  TB_SetTextColor(gPakError[pak][id].choice1?1:22);
  TB_DrawText(x,y-(gPakError[pak][id].choice1==0)+h,gText.single[245]);
  HAL_SetFont(gPakError[pak][id].choice1?10:11);
  TB_SetTextColor(gPakError[pak][id].choice1?22:1);
  TB_DrawText(x,y-(gPakError[pak][id].choice1?1:0),gText.taba[gText.tab[26]]);
 }
 TB_SetJust(0,3); HAL_SetFont(11);
}

void _DrawPakErrNQA(int pak, int id, const unsigned char *str)
{
    HAL_SetFont(11);
    TB_SetTextColor(1);
    TB_SetJust(1, 1);
    TB_DrawTextScrollBox(gPakError[pak][id].left + gPakError[pak][id].width / 2,
                        gPakError[pak][id].top + gPakError[pak][id].height / 2,
                        gPakError[pak][id].width, gPakError[pak][id].height,
                        -1, 0, (void *)str);
    TB_SetJust(0, 3);
}

void _DrawPakErrNewPak(int pak,int id)
{
 unsigned char str[256];
 TB_Strcpy(str,gPakError[pak][id].rumble?gText.single[116]:gText.single[117]);
 TB_Strcat(str,"\n\n"); TB_Strcat(str,gText.single[118]);
 if (gPakError[pak][id].name) TB_Strcat(str,gPakError[pak][id].name);
 TB_Strcat(str,"\n\n");TB_Strcat(str,gText.single[110]);TB_Strcat(str,gText.single[245]);
 _DrawPakErrNQA(pak,id,str);
}

void _DrawPakErrInconsistent(int pak, int id)
{
    unsigned char str[256];
    TB_Strcpy(str, gText.single[119]);
    TB_Strcat(str, "\n\n");
    TB_Strcat(str, gText.single[120]);
    _DrawPakErrNQA(pak, id, str);
}

void _DrawPakErrFull(int pak, int id)
{
    unsigned char str[256];
    unsigned char *nl;
    TB_Strcpy(str, gText.single[123]);
    nl=(unsigned char *)"\n\n";
    TB_Strcat(str,nl);
    TB_Strcat(str, gText.single[124]);
    TB_Strcat(str,nl);
    TB_Strcat(str, gText.single[110]);
    TB_Strcat(str, gText.single[112]);
    _DrawPakErrNQA(pak, id, str);
}

void _DrawPakErrBadData(int pak, int id)
{
    unsigned char str[256];
    TB_Strcpy(str, gText.single[125]);
    TB_Strcat(str, "\n\n");
    TB_Strcat(str, gText.single[120]);
    _DrawPakErrNQA(pak, id, str);
}

void _DrawPakErrUnknown(int pak, int id)
{
    unsigned char str[256];
    TB_Strcpy(str, gText.single[121]);
    TB_Strcat(str, "\n\n");
    TB_Strcat(str, gText.single[110]);
    TB_Strcat(str, gText.single[112]);
    _DrawPakErrNQA(pak, id, str);
}

void SetPakError(int pak, int err, void * name, int data, int left, int top, int width, int height)
{
    static signed char sInited = 0;
    int i, j;
    if (!sInited) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 3; j++) {
                gPakError[i][j].err = kPS_ErrNone;
                gPakError[i][j].done = 1;
            }
        }
        sInited = 1;
    }
    for (i = 0; i < 3; i++) {
        if (gPakError[pak][i].err == kPS_ErrNone)
            break;
    }
    gPakError[pak][i].done = 0;
    gPakError[pak][i].timeout = gFrameCounter + (int)(3.0f * gRetraceRate);
    if (err == kPS_ErrRecognize || err == kPS_ErrRemoved) {
        for (j=0;j<3;j++) {
            if (gPakError[pak][j].err == err) {
                gPakError[pak][j].timeout=gFrameCounter+(int)(3.0f*gRetraceRate);
                gPakError[pak][i].timeout=0;
            }
        }
    }
    gPakError[pak][i].name = name;
    gPakError[pak][i].data = data;
    gPakError[pak][i].choice1 = 1;
    gPakError[pak][i].choice2 = 0;
    gPakError[pak][i].err = err;
    for (j = 0; j < 4; j++) {
        gPakError[pak][i].contActive[j] = GetControllerActive(j);
        gPakError[pak][i].contStick[j] = GetControllerStickEdges(j);
        gPakError[pak][i].contDebug[j] = 0;
    }
    SetControllerActive(-1, 0);
    SetControllerStickEdges(-1, 0);
    if (err == kPS_ErrNoCont) {
        SetControllerActive(-1, 1);
        SetControllerStickEdges(-1, 1);
    } else {
        gPakError[pak][i].cont = pak / 2;
        if (err == kPS_ErrNoPak) {
            SetControllerActive(-1, 1);
            SetControllerStickEdges(-1, 1);
        } else {
            SetControllerActive(gPakError[pak][i].cont, 1);
            SetControllerStickEdges(gPakError[pak][i].cont, 1);
        }
    }
    gPakError[pak][i].rumble = PS_PakRumble(pak);
    gPakError[pak][i].left = left;
    gPakError[pak][i].top = top;
    gPakError[pak][i].width = width;
    gPakError[pak][i].height = height;
}

int PakErrorActive(int pak)
{
 int i;
 for(i=2;i>=0;i--) if(gPakError[pak][i].err!=kPS_ErrNone) return gPakError[pak][i].err;
 return 0;
}

void CancelPakError(int pak)
{
    int i;
    for (i = 2; i >= 0; i--) {
        if (gPakError[pak][i].err != kPS_ErrNone) {
            gPakError[pak][i].err = kPS_ErrNone;
            return;
        }
    }
}

void ShowPakError(int pak, signed char *choice1, signed char *choice2, signed char *done, int (*cancelFunc)(int))
{
    int i, j;
    char str[256];
    if (cancelFunc && cancelFunc(pak))
        CancelPakError(pak);
    for (i = 2; i >= 0; i--) {
        if (gPakError[pak][i].err != kPS_ErrNone)
            break;
        if (!gPakError[pak][i].done) {
            gPakError[pak][i].done = 1;
            *done = 1;
            *choice2 = 0;
            *choice1 = 0;
            goto restore;
        }
    }
    if (i < 0)
        return;
    gPakError[pak][i].edges = gActiveEdges;
    gPakError[pak][i].levels = gActiveLevels;
    switch (gPakError[pak][i].err) {
    case kPS_ErrNoCont: _ControlChoice(pak, i); break;
    case kPS_ErrNoPak:
        if (!(gGameState & 0x7c03fffe)) _ControlChoice(pak, i);
        else _ControlPakErrNQA(pak, i);
        break;
    case kPS_ErrNewPak: _ControlPakErrNQA(pak,i); gPakError[pak][i].choice1=0; break;
    case kPS_ErrInconsistent: _ControlPakErrNQA(pak, i); break;
    case kPS_ErrUnknown: _ControlPakErrNQA(pak, i); break;
    case kPS_ErrComm:
    case kPS_ErrFail: _ControlChoice(pak, i); break;
    case kPS_ErrExist: _ControlChoice(pak, i); break;
    case kPS_ErrFull: _ControlPakErrNQA(pak,i); gPakError[pak][i].choice1=0; break;
    case kPS_ErrBadData: _ControlPakErrNQA(pak, i); break;
    case kPS_ErrRecognize: _ControlPakErrTimeout(pak, i); break;
    case kPS_ErrRemoved: _ControlPakErrNQA(pak, i); break;
    case kPS_ErrCorruptFile: _ControlPakErrNQA(pak, i); break;
    case kPS_ErrDevice: gPakError[pak][i].done = 1; break;
    }
    *done = gPakError[pak][i].done;
    if (gPakError[pak][i].done) {
        gPakError[pak][i].err = kPS_ErrNone;
        *choice1 = gPakError[pak][i].choice1;
        *choice2 = gPakError[pak][i].choice2;
        goto restore;
    }
    HAL_SetFont(11);
    TB_SetTextColor(1);
    if (gPakError[pak][i].err != kPS_ErrRecognize) _DrawPakHeading(pak,i);
    switch (gPakError[pak][i].err) {
    case kPS_ErrNoCont: _DrawPakErrNoController(pak, i); break;
    case kPS_ErrNoPak: _DrawPakErrNoPak(pak, i); break;
    case kPS_ErrNewPak: _DrawPakErrNewPak(pak, i); break;
    case kPS_ErrInconsistent: _DrawPakErrInconsistent(pak, i); break;
    case kPS_ErrUnknown: _DrawPakErrUnknown(pak, i); break;
    case kPS_ErrComm:
    case kPS_ErrFail:
        _DrawChoice(pak, i, gText.single[121], gText.taba[gText.tab[27] + 1], gText.single[245], gPakError[pak][i].choice1);
        break;
    case kPS_ErrExist:
        _DrawChoice(pak, i, gText.single[122], gText.taba[(int)gText.tab[25]], gText.taba[(int)gText.tab[25] + 1], gPakError[pak][i].choice1);
        break;
    case kPS_ErrFull: _DrawPakErrFull(pak, i); break;
    case kPS_ErrBadData: _DrawPakErrBadData(pak, i); break;
    case kPS_ErrRecognize:
        sprintf(str, "%s%s%s", gText.single[133], gPS_ContName[pak], gText.single[134]);
        _DrawPakErrNQA(pak, i, (unsigned char *)str);
        break;
    case kPS_ErrRemoved:
        sprintf(str, "%s%s%s\n\n%s%s", gText.single[135], gPS_PakName[pak], gText.single[136], gText.single[110], gText.single[112]);
        _DrawPakErrNQA(pak, i, (unsigned char *)str);
        break;
    case kPS_ErrCorruptFile:
        sprintf(str, "%s\"%s\"%s%s\n\n%s%s", gText.single[137], gPakError[pak][i].name, gText.single[138], gPS_PakName[pak], gText.single[110], gText.single[112]);
        _DrawPakErrNQA(pak, i, (unsigned char *)str);
        break;
    case kPS_ErrDevice: gPakError[pak][i].done = 1; break;
    }
    return;
restore:
    for (j = 0; j < 4; j++) {
        SetControllerActive(j, gPakError[pak][i].contActive[j]);
        SetControllerStickEdges(j, gPakError[pak][i].contStick[j]);
    }
    UpdateRushControls();
}

void PS_ErrorFunc(int pak,int error,void *name,int data,signed char *choice1,signed char *choice2,int (*cancelFunc)(int))
{
 static const tBlitDef dimMultiBlit[1]={{0,0,0,320,220,-1,-1,-1,-1,0,192,_AnimDim,0}};
 signed char done=0;
 _tBlit *dimBlit;
 signed short min[4],max[4];
 signed char red[4],green[4],blue[4],alpha[4];
 int i;
 dimBlit=NewMultiBlit(0,0,dimMultiBlit,1);
 SetPakError(pak,error,name,data,10,10,300,180);
 for(i=0;i<gNumWindows;i++) {
  ZOID_GetFog(&min[i],&max[i],&red[i],&green[i],&blue[i],&alpha[i],i);
  ZOID_SetFog(0,0,0,0,0,255,i);
 }
 while(!done) { UpdateFrame();SaveServer(0,0);ShowPakError(pak,choice1,choice2,&done,cancelFunc); }
 RemoveBlit(dimBlit);
 for(i=0;i<gNumWindows;i++) ZOID_SetFog(min[i],max[i],red[i],green[i],blue[i],alpha[i],i);
}

int _AnimDim(_tBlit *blt)
{
 static unsigned char color[4]={0,0,0,224};
 blt->palette=color; UpdateBlit(blt); blt->animFunc=0; return 1;
}
