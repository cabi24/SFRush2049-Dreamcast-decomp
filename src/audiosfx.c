/* Dreamcast audiosfx unit. MAT3 command-queue semantics and DC Audio64 evidence.
 * Anonymous entry labels are retained until root accepts identity metadata.
 */
#include "..\include\audiosfx_private.h"

signed char func_8C016508(float volume);

void func_8C015EF0(DcSfxBank *bank, int lock)
{
    unsigned int size;
    unsigned int allocationSize;
    unsigned int available;
    unsigned int i;
    void *buffer;
    size=0;
    if (!bank->loaded) {
        if (!func_8C0A0EE0(bank->name, &size)) { }
        allocationSize = (size + 2047) & ~2047;
        buffer = AllocateMemoryHeap(0, allocationSize);
        if (!func_8C0A0F2A(bank->name, buffer, allocationSize)) {
            if (gExitGame) func_8C039718();
        }
    recheck:
            if (!func_8C0A18E6(&available)) { }
        if (gAudBankEvictionEnabled && available < size) {
            for (i = 0; i < 256; i++) {
                if (gSfxInstances[i].used &&
                    gSfxInstances[i].soundId >= gAudLoadedBanks[gAudNumLoadedBanks-1]->first &&
                    gSfxInstances[i].soundId <= gAudLoadedBanks[gAudNumLoadedBanks-1]->last)
                    break;
            }
            if (i==256) {
                func_8C016128(gAudLoadedBanks[--gAudNumLoadedBanks]);
                goto recheck;
            }
        }
        if (!func_8C0A191C(&bank->handle, available, 32, 2, 0)) { }
        if (!func_8C0A0E48(bank->handle, buffer, size)) { }
        DeallocateMemory(buffer);
        bank->loaded = 1;
        bank->locked = lock;
        gAudLoadedBanks[gAudNumLoadedBanks++] = bank;
    }
}

void func_8C016044(void)
{
    int time;
    int i;
    DcSfxBank **loaded;
    time = func_8C06AEC8() - 1.0f;
    loaded=gAudLoadedBanks;
    while (gAudNumLoadedBanks) {
        if (loaded[gAudNumLoadedBanks-1]->locked) break;
        for (i=0; i<256; i++) {
            if (gSfxInstances[i].used && gSfxInstances[i].started &&
                func_8C0A2EEA(&gSfxInstances[i].handle) &&
                gSfxInstances[i].soundId >= gAudLoadedBanks[gAudNumLoadedBanks-1]->first &&
                gSfxInstances[i].soundId <= gAudLoadedBanks[gAudNumLoadedBanks-1]->last) {
                if (func_8C06AF30(time, 1.0f)) {
                    time = func_8C06AEC8();
                }
                break;
            }
        }
        if (i>=256) func_8C016128(loaded[--gAudNumLoadedBanks]);
    }
}

void func_8C016128(DcSfxBank *bank)
{
    unsigned int available;
    if (!func_8C0A1BB4(bank->handle)) { }
    if (!func_8C0A18E6(&available)) { }
    bank->loaded=0;
}

DcSfxBank *func_8C016174(int soundId)
{
    int i;
    DcSfxBank *bank;
    for (i=0,bank=gAudBankTable; i<9; i++,bank++) {
        if (soundId>=bank->first && soundId<=bank->last) break;
    }
    if (i>=9) { }
    if (!bank->loaded && gAudBankEvictionEnabled) func_8C015EF0(bank,0);
    return bank;
}

int func_8C0161B2(DcAudioHandle *handle)
{
    unsigned int i;
    int result;
    DcAudioHandle old;
    if (!func_8C0A2BF0(handle)) return 0;
    for (i=0; i<gAudNumActiveResources; i++) {
        if (gAudActiveResources[i]==handle->resource_id) {
            memcpy(&old,handle,sizeof(old));
            result=func_8C0161B2(handle);
            func_8C0A29BE(&old);
            return result;
        }
    }
    return 1;
}

int func_8C016210(tSfxVoice *sound,float volume,float pan)
{
    DcSfxBank *bank;
    int result;
    bank=func_8C016174(sound->soundId);
    result=!func_8C0A2A60(bank->handle,sound->soundId-bank->first,&sound->handle);
    if (result) return 0;
    if (!func_8C0161B2(&sound->handle)) return 0;
    result=!func_8C0A2DB2(&sound->handle,func_8C016508(volume*gSfxMasterVol*(sound->soundId==153?1.0f:0.80000002f)));
    if (result) func_8C0A29BE(&sound->handle);
    if (result) return 0;
    result=!func_8C0A2E1C(&sound->handle,(unsigned int)(((1.0f-pan)/2.0f)*127.0f));
    if (result) func_8C0A29BE(&sound->handle);
    if (result) return 0;
    result=!func_8C0A2C26(&sound->handle,&sound->basePitch);
    if (result) return 0;
    result=!func_8C0A3118(&sound->handle);
    if (result) return 0;
    sound->started=1;
    return 1;
}

int func_8C01631E(tSfxVoice *sound)
{
    return func_8C0A2EEA(&sound->handle);
}

int func_8C01632A(tSfxVoice *sound)
{
    int id;
    int result;
    if (!func_8C0A2EEA(&sound->handle) || sound->handle.resource_id==0x666) {
        result=1;
    } else {
        id=sound->handle.resource_id;
        result=func_8C0A2F10(&sound->handle);
        if (result) gAudActiveResources[gAudNumActiveResources++]=id;
    }
    if (result) {
        FREE_VOICE(sound);
        return 1;
    }
    return 0;
}

int func_8C0163BC(tSfxVoice *sound,float volume)
{
    if (!func_8C0A2DB2(&sound->handle,func_8C016508(volume*gSfxMasterVol*(sound->soundId==153?1.0f:0.80000002f)))) return 0;
    return 1;
}
int func_8C016406(tSfxVoice *sound,float pan)
{
    if (!func_8C0A2E1C(&sound->handle,(unsigned int)(((1.0f-pan)/2.0f)*127.0f))) return 0;
    return 1;
}
int func_8C016454(tSfxVoice *sound,float pitch)
{
    if (!func_8C0A2D2C(&sound->handle,(unsigned int)(sound->basePitch*pitch))) return 0;
    return 1;
}
int func_8C0164AA(int mono)
{
    if (!func_8C09CB80()) return 0;
    func_8C0D84D4(mono?1:0);
    gMono=mono;
    return 1;
}
signed char func_8C016508(float volume)
{
    float complement;
    complement=1.0f-volume;
    volume=-(complement*complement)+1.0f;
    return 127-(int)gAudVolumeCurve[(unsigned char)(volume*255.0f)]/2;
}

int func_8C016538(tSfxVoice **theSound)
{
    unsigned int i;
    *theSound=0;
    for (i=0; i<gNumInstances; i++) {
        if (!gSfxInstances[i].used) {
            memset(&gSfxInstances[i].handle,0,sizeof(DcAudioHandle));
            gSfxInstances[i].used=1;
            gSfxInstances[i].started=0;
            gSfxInstances[i].stop=0;
            gSfxInstances[i].stopped=0;
            gSfxInstances[i].stopAll=0;
            gSfxInstances[i].fail=0;
            gSfxInstances[i].index=(gNextIndex<<8)|i;
            if (gNextIndex >= 0xffffff) gNextIndex=1;
            else gNextIndex++;
            *theSound=&gSfxInstances[i];
            return 1;
        }
    }
    return 0;
}
tSfxCmd *func_8C0165F4(void)
{
    tSfxCmd *cmd;
    cmd=gFreeQ.tail;
    if (!cmd) { }
    cmd->failCounter=0;
    RemoveLLE(&gFreeQ,cmd);
    return cmd;
}
tSfxCmd *func_8C016612(tSfxVoice *sound)
{
    tSfxCmd *c;
    for(c=gCmdQ.tail;c;c=c->dlle.prev) {
        if(c->sound==sound && c->cmd==3) return c;
    }
    return 0;
}
/* Accepted map calls this _FreeCmd; target behavior is MAT3's _SendCmd. */
void _FreeCmd(tSfxCmd *cmd)
{
    tSfxCmd *c;
    tSfxCmd *next;
    if(cmd->cmd!=4) {
      if(cmd->cmd==2 && !cmd->sound->started) {
        for(c=gCmdQ.tail;c;c=next) {
            next=c->dlle.prev;
            if(c->sound==cmd->sound) {
                if(c->cmd) { }
                RELEASE_CMD(c);
            }
        }
        FREE_VOICE(cmd->sound);
        InsertLLE(&gFreeQ,cmd,gFreeQ.head);
        return;
      }
      if(cmd->sound->stop && cmd->cmd!=2) { }
    }
    InsertLLE(&gCmdQ,cmd,gCmdQ.head);
}

void AudioSfxInit(int maxSfx)
{
    int i;
    int numCmds;
    InitLL(&gFreeQ,0,1);
    InitLL(&gCmdQ,0,1);
    if(maxSfx>256) { }
    gNumInstances=maxSfx;
    gSfxInstances=AllocateMemoryHeap(0,maxSfx*sizeof(tSfxVoice));
    PurgeableMemory(gSfxInstances,0);
    memset(gSfxInstances,0,gNumInstances*sizeof(tSfxVoice));
    numCmds=maxSfx*3;
    gCmds=AllocateMemoryHeap(0,numCmds*sizeof(tSfxCmd));
    PurgeableMemory(gCmds,0);
    memset(gCmds,0,numCmds*sizeof(tSfxCmd));
    for(i=0;i<numCmds;i++) InsertLLE(&gFreeQ,&gCmds[i],gFreeQ.head);
    gNumChannels=maxSfx;
    for(i=0;i<sizeof(gStoppedChannels)/sizeof(gStoppedChannels[0]);i++) gStoppedChannels[i]=0;
    gStopAllEffects=0;
    gInited=1;
}
void func_8C0167E8(void)
{
    func_8C016C3C();
    while(*(volatile signed char *)&gStopAllEffects) { }
    while(*(void *volatile *)&gCmdQ.head) { }
    gInited=0;
    DeallocateMemory(gSfxInstances);
    DeallocateMemory(gCmds);
}

void AudioSfxServer(void)
{
    tSfxCmd *cmd;
    tSfxCmd *next;
    int i;
    if(!gInited) return;
    gAudNumActiveResources=0;
    gNumChannels+=gStoppedChannels[0];
    for(i=sizeof(gStoppedChannels)/sizeof(gStoppedChannels[0])-1;i>0;i--) gStoppedChannels[i]=gStoppedChannels[i-1];
    gStoppedChannels[0]=0;
    if(gStopAllEffects) {
        for(i=0;i<gNumInstances;i++) {
            if(gSfxInstances[i].used && gSfxInstances[i].stopAll && !gSfxInstances[i].stop) AudioStopEffect(gSfxInstances[i].index);
        }
        if(i==gNumInstances) gStopAllEffects=0;
    }
    for(i=0;i<gNumInstances;i++) {
        if(gSfxInstances[i].used && gSfxInstances[i].started) {
            if(!func_8C01631E(&gSfxInstances[i])) {
                if(gSfxInstances[i].stopped) { }
                for(cmd=gCmdQ.tail;cmd;cmd=next) {
                    next=cmd->dlle.prev;
                    if(cmd->sound==&gSfxInstances[i]) {
                        if(gSfxInstances[i].stopped && cmd->cmd==0) { }
                        RELEASE_CMD(cmd);
                    }
                }
                gStoppedChannels[0]++;
                FREE_VOICE(&gSfxInstances[i]);
            }
        }
    }
    for(cmd=gCmdQ.tail;cmd;cmd=next) {
        next=cmd->dlle.prev;
        if(cmd->sound && cmd->sound->stop && cmd->cmd!=2) {
            RELEASE_CMD(cmd);
            continue;
        }
        if(cmd->sound && cmd->sound->fail) continue;
        switch(cmd->cmd) {
        case 0:
            if(gNumChannels<=0 || !func_8C016210(cmd->sound,cmd->vol,cmd->lrpan)) goto fail;
            gNumChannels--;
            break;
        case 2:
            if(!func_8C01632A(cmd->sound)) goto fail;
            gStoppedChannels[0]++;
            break;
        case 3:
            if(cmd->vol!=-2.0f && !func_8C0163BC(cmd->sound,cmd->vol)) goto fail;
            if(cmd->lrpan!=-2.0f && !func_8C016406(cmd->sound,cmd->lrpan)) goto fail;
            if(cmd->fbpan!=-2.0f) { }
            if(cmd->pitch!=-2.0f && !func_8C016454(cmd->sound,cmd->pitch)) goto fail;
            break;
        case 4:
            if(!func_8C0164AA(cmd->vol)) goto fail;
            break;
        }
        RELEASE_CMD(cmd);
        continue;
    fail:
        cmd->failCounter++;
        if(cmd->sound) cmd->sound->fail=1;
        if(cmd->failCounter>=20) {
            if(cmd->cmd==3 || cmd->cmd==4) {
                if(cmd->cmd==4) { }
            }
            RELEASE_CMD(cmd);
        }
    }
    for(cmd=gCmdQ.tail;cmd;cmd=cmd->dlle.prev) {
        if(cmd->sound) cmd->sound->fail=0;
    }
}

unsigned int AudioPlayEffect(int soundId,float volume,float pan)
{
    tSfxVoice *theSound;
    tSfxCmd *cmd;
    theSound=0;
    if(!gInited) { }
    if(soundId==-1) return 0;
    if(!func_8C016538(&theSound)) return 0;
    cmd=func_8C0165F4();
    cmd->cmd=0;
    cmd->sound=theSound;
    theSound->soundId=soundId;
    cmd->vol=volume;
    cmd->lrpan=pan;
    _FreeCmd(cmd);
    return theSound->index;
}
int AudioEffectPlaying(unsigned int sound)
{
    int result;
    if(!gInited) { }
    if(sound && sound==gSfxInstances[sound&0xff].index) {
        if(gSfxInstances[sound&0xff].started) {
            result=func_8C0A2EEA(&gSfxInstances[sound&0xff].handle);
            return result;
        }
        return 1;
    }
    return 0;
}
void AudioStopEffect(unsigned int sound)
{
    tSfxCmd *cmd;
    if(!gInited) { }
    if(!sound || gSfxInstances[sound&0xff].index!=sound) return;
    cmd=func_8C0165F4();
    cmd->cmd=2;
    cmd->sound=&gSfxInstances[sound&0xff];
    cmd->sound->stop=1;
    cmd->sound->index=0;
    _FreeCmd(cmd);
}
void func_8C016C3C(void)
{
    int i;
    if(!gInited) { }
    for(i=0;i<gNumInstances;i++) {
        if(gSfxInstances[i].used) gSfxInstances[i].stopAll=1;
    }
    gStopAllEffects=1;
}
int func_8C016CA8(void)
{
    int i;
    if(!gInited) { }
    for(i=0;i<gNumInstances;i++) {
        if(gSfxInstances[i].used) return 0;
    }
    return 1;
}

void AudioSetEffectVolume(unsigned int sound,float vol)
{
    tSfxCmd *cmd;
    if(!gInited) { }
    if(!sound || gSfxInstances[sound&0xff].index!=sound) return;
    cmd=func_8C016612(&gSfxInstances[sound&0xff]);
    if(!cmd) {
        cmd=func_8C0165F4();cmd->cmd=3;cmd->sound=&gSfxInstances[sound&0xff];
        cmd->lrpan=-2.0f;cmd->fbpan=-2.0f;cmd->pitch=-2.0f;cmd->vol=vol;
        _FreeCmd(cmd);
    } else cmd->vol=vol;
}
void AudioSetEffectLRPan(unsigned int sound,float pan)
{
    tSfxCmd *cmd;
    if(!sound || gSfxInstances[sound&0xff].index!=sound) return;
    cmd=func_8C016612(&gSfxInstances[sound&0xff]);
    if(!cmd) {
        cmd=func_8C0165F4();cmd->cmd=3;cmd->sound=&gSfxInstances[sound&0xff];
        cmd->vol=-2.0f;cmd->fbpan=-2.0f;cmd->pitch=-2.0f;cmd->lrpan=pan;
        _FreeCmd(cmd);
    } else cmd->lrpan=pan;
}
void AudioSetEffectFBPan(unsigned int sound,float pan)
{
    tSfxCmd *cmd;
    if(!sound || gSfxInstances[sound&0xff].index!=sound) return;
    cmd=func_8C016612(&gSfxInstances[sound&0xff]);
    if(!cmd) {
        cmd=func_8C0165F4();cmd->cmd=3;cmd->sound=&gSfxInstances[sound&0xff];
        cmd->vol=-2.0f;cmd->lrpan=-2.0f;cmd->pitch=-2.0f;cmd->fbpan=pan;
        _FreeCmd(cmd);
    } else cmd->fbpan=pan;
}
void AudioSetEffectPitch(unsigned int sound,float pitch)
{
    tSfxCmd *cmd;
    if(!sound || gSfxInstances[sound&0xff].index!=sound) return;
    cmd=func_8C016612(&gSfxInstances[sound&0xff]);
    if(!cmd) {
        cmd=func_8C0165F4();cmd->cmd=3;cmd->sound=&gSfxInstances[sound&0xff];
        cmd->vol=-2.0f;cmd->lrpan=-2.0f;cmd->fbpan=-2.0f;cmd->pitch=pitch;
        _FreeCmd(cmd);
    } else cmd->pitch=pitch;
}
void func_8C016ED0(int mono)
{
    tSfxCmd *cmd;
    cmd=func_8C0165F4();
    cmd->cmd=4;
    cmd->vol=mono;
    cmd->sound=0;
    _FreeCmd(cmd);
}
