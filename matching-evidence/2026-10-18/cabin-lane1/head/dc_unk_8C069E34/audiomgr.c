/* Private native SHC audiomgr candidate; no flags/assembly/original-byte injection.
 * AudPlaySequence remains an external until native stdarg source is established.
 * No claim is made for missing, moved, fill, natural padding or BSS bytes.
 */
#include "audiomgr_types.h"
#define LOCK() do {if(!gAudioLockCount)gSavedAudioMask=SetInterruptMask(15);gAudioLockCount++;}while(0)
#define UNLOCK() do {gAudioLockCount--;if(!gAudioLockCount)SetInterruptMask(gSavedAudioMask);}while(0)
void AudioInit(unsigned char numVoices, unsigned char maxSfx, unsigned char maxMusic, int numListeners, int numEmitters, float footUnits, float frontDistance, int doppler);
void AudioShutdown(void);
void _PeriodicMaintenance(void);
void _ProcessCommand(tAudioMsg *cmd);
tAudioMsg *_GetCmd(void);
tFxSequence *_GetFxTag(void);
void _FreeFxTag(tFxSequence *tag);
void _PlayFxTag(tFxSequence *tag);
void _InterruptFxTag(tFxSequence *tag);
tFxSequence *_ConvertTag(int tag);
void AudPlaySong(int song, int loop);
void AudKillSong(void);
int AudPlayEffect(int fxid, int subId, int replace, unsigned char priority);
int AudPlayEffectVP(int fxid, int subId, int replace, unsigned char priority, float volume, float lrpan, float fbpan);
int AudEffectPlaying(int tag);
void _AudChangeEffect(int tag, float volume, float lrpan, float fbpan, float pitch);
void AudChangeEffectVol(int tag, float volume);
void AudChangeEffectLRPan(int tag,float pan);
void AudChangeEffectFBPan(int tag,float pan);
void AudChangeEffectPitch(int tag, float pitch);
void AudStopEffect(int tag);
void AudStopAllEffects(void);
void AudPauseAllEffects(void);
void AudResumeAllEffects(void);
int _WeightedPriority(tFxSequence *tag);
void _InsertSorted(tFxSequence *tag);
int _OrderFx(void * el1, void * el2);
void _UpdateFx(void * element, int elNum);
void _ProcessPriorities(void);
void _AudSetMasterVol(float volume, float time, int music, int sfx);
void AudSetFxVol(float volume, float time);
void AudSetMusicVol(float volume, float time);
void AudMono(int mono);
anon_ll_free_3c *_NewListener(void);
void _FreeListener(anon_ll_free_3c *lis);
void _ValidateListener(anon_ll_free_3c *lis);
tSfxEmitter *_NewEmitter(void);
void _FreeEmitter(tSfxEmitter *em);
void _ValidateEmitter(tSfxEmitter *em);
void AudInit3D(int numListeners, int numEmitters, float footUnits, float frontDistance, int doppler);
void AudShutdown3D(void);
int AudAddListener(float *pos, float *dir, float *look, float *up);
void AudRemoveListener(int lis);
void AudUpdateListener(int lis, float *pos, float *dir, float *look, float *up);
int AudEmitEffect(float *pos, float *dir, float fadeDistance, float fadeCompression, float maxVol, float minVol, int fxid, int subId, int replace, unsigned char priority);
void AudStopEmitter(int tag);
int AudEmitterPlaying(int tag);
void AudUpdateEmitter(int tag,float *pos,float *dir,float maxVol,float pitch);
void _ProcessEmitters(void);
void _UpdateEmitter(tSfxEmitter *em, float *vol, float *lrpan, float *fbpan, float *pitch);
anon_ll_id_3c *_NewSequence(void);
void _FreeSequence(anon_ll_id_3c *s);
anon_ll_id_3c *_ConvertSequence(int tag);
void AudStopSequence(int tag);
void _StopAllSequences(void);
void _ProcessSequences(void);
int gAudioLockCount=0; /* BAD44 pool3DD0,3E50 */
signed char gInited=0; /* BAD48 pool3AF0 */
int gSong=-1; /* BAD4C pool3E60 */
tFxSequence *gFxTags=0; /* BAD50 pool3C64 */
anon_te_tag_c *gSortedFxTags=0; /* BAD54 pool3C80 */
int gNumStartCmds=0,gNumStopCmds=0; /* BAD58/5C pool3C94/90 */
unsigned int gNextFxId=1,gNextSeqId=1; /* BAD60/64 pool43D0/5DDC */
anon_ll_free_3c *gListeners=0; /* BAD68 pool50E0 */
tSfxEmitter *gEmitters=0; /* BAD6C pool537C */
int gNumFxTags=0,gNumListeners=0,gNumEmitters=0; /* BAD70/74/78,3B04/50E4/5380 */
anon_ll_id_3c *gSequences=0; /* BAD7C pool3CAC */
signed char gStarted=0; /* BAD80 pool3AFC */
int gNumFxPlaying=0,gNumFxPending=0,gNumFxInterrupted=0; /* BAD84/88/8C,4E6C/70/74 */
const char gEffectLoop[311] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
};

/* DC 0x8C013A40; semantic MAT3 body with target-backed platform contracts. */
void AudioInit(unsigned char numVoices, unsigned char maxSfx, unsigned char maxMusic, int numListeners, int numEmitters, float footUnits, float frontDistance, int doppler)
{
    int i;
    if (gInited) {
        if (gMaxSfx == maxSfx && gMaxMusic == maxMusic) return;
        AudioShutdown();
    }
    gMaxSfx = maxSfx;
    gMaxMusic = maxMusic;
    for (i = 0; i < 128; i++) gAudioCommandQueue[i].hdr.used = 0;
    if (!gStarted) gStarted=1;
    AudioInitSys(numVoices, maxSfx, maxMusic);
    gNumFxTags = gMaxSfx * 2 < 64 ? gMaxSfx * 2 : 64;
    gNumFxTags = gNumFxTags > gMaxSfx + numEmitters ? gNumFxTags : gMaxSfx + numEmitters;
    gFxTags = AllocateMemoryHeap(0, gNumFxTags * sizeof(*gFxTags));
    PurgeableMemory(gFxTags, 0);
    InitLL(&gFreeFxTags, 0, 1);
    InitLL(&gPlayingFxTags, 0, 1);
    InitLL(&gInterruptedFxTags, 0, 1);
    gSortedFxTags = AllocateMemoryHeap(0, gNumFxTags * sizeof(*gSortedFxTags));
    PurgeableMemory(gSortedFxTags, 0);
    InitBST(&gSortedFxTagTree, _OrderFx);
    gNumStartCmds = gNumStopCmds = 0;
    for (i = 0; i < gNumFxTags; i++) {
        gFxTags[i].id = i;
        InsertLLE(&gFreeFxTags, &gFxTags[i], gFreeFxTags.head);
        gFxTags[i].list = &gFreeFxTags;
    }
    gTagShift = HAL_IntLog2(gNumFxTags);
    if (gNumFxTags > (1 << gTagShift)) gTagShift++;
    gTagMask = (1 << gTagShift) - 1;
    gSequences = AllocateMemoryHeap(0, 6 * sizeof(*gSequences));
    PurgeableMemory(gSequences, 0);
    InitLL(&gFreeSequences, 0, 1);
    InitLL(&gActiveSequences, 0, 1);
    for (i = 0; i < 6; i++) {
        gSequences[i].id = i;
        gSequences[i].free = 1;
        gSequences[i].active = 0;
        InsertLLE(&gFreeSequences, &gSequences[i], gFreeSequences.head);
    }
    AudInit3D(numListeners, numEmitters, footUnits, frontDistance, doppler);
    gInited = 1;
}

/* DC 0x8C013D1C; semantic MAT3 body with target-backed platform contracts. */
void AudioShutdown(void)
{
    if (gInited) {
    gInited = 0;
    AudioShutdownSys();
    DeallocateMemory(gFxTags);
    DeallocateMemory(gSortedFxTags);
    DeallocateMemory(gSequences);
    }
}

/* 8C013D4A..8C013D4E remains an unsourced target gap. */

/* DC 0x8C013D4E; semantic MAT3 body with target-backed platform contracts. */
void _PeriodicMaintenance(void)
{
    if (gInited) { _ProcessEmitters(); _ProcessSequences(); _ProcessPriorities(); }
    LOCK();
    AudioPeriodicMaintenance();
    UNLOCK();
}

/* DC 0x8C013DE0; semantic MAT3 body with target-backed platform contracts. */
void _ProcessCommand(tAudioMsg *cmd)
{
    tFxSequence *tag;
    tFxSequence *next;
    LOCK();
    switch (cmd->hdr.cmd) {
    case 0:
        if (gSong != cmd->playSong.song && gSong < 0) {
            gSong = cmd->playSong.song;
            AudioSongPlay(gSong, cmd->playSong.loop);
        }
        break;
    case 1:
        if (gSong >= 0) {
            UNLOCK();
            AudioSongStop(gSong);
            LOCK();
            gSong = -1;
        }
        break;
    case 2:
        cmd->playEffect.tag->numCmds--;
        if (cmd->playEffect.tag->state == 0) {
            _FreeFxTag(cmd->playEffect.tag);
            break;
        }
        cmd->playEffect.tag->vol = cmd->playEffect.vol;
        cmd->playEffect.tag->lrpan = cmd->playEffect.lrpan;
        cmd->playEffect.tag->fbpan = 0.0f;
        cmd->playEffect.tag->pitch = 1.0f;
        if (cmd->playEffect.replace != 0) {
            for (tag = gPlayingFxTags.head; tag; tag = tag->ll.next) {
                if (tag != cmd->playEffect.tag && tag->state != 0
                    && tag->fxid == cmd->playEffect.tag->fxid
                    && tag->subId == cmd->playEffect.tag->subId
                    && tag->type == cmd->playEffect.tag->type) break;
            }
            if (!tag) {
                for (tag = gInterruptedFxTags.head; tag; tag = tag->ll.next) {
                    if (tag != cmd->playEffect.tag && tag->state == 1
                        && tag->fxid == cmd->playEffect.tag->fxid
                        && tag->subId == cmd->playEffect.tag->subId
                        && tag->type == cmd->playEffect.tag->type) break;
                }
            }
            if (tag) {
                if (tag->state == 2 || tag->state == 1) {
                    if (cmd->playEffect.replace == 2) {
                        _FreeFxTag(cmd->playEffect.tag);
                        if (tag->state == 1) tag->startTime = -1.0f;
                        break;
                    } else if (tag->state == 2) AudioStopEffect(tag->sysId);
                }
                _FreeFxTag(tag);
            }
        }
        if (cmd->playEffect.fbpan != -2.0f)
            cmd->playEffect.tag->fbpan = cmd->playEffect.fbpan;
        else cmd->playEffect.tag->fbpan = 0.0f;
        cmd->playEffect.tag->state = 3;
        break;
    case 3:
        cmd->startEffect.tag->numCmds--;
        if (cmd->startEffect.tag->state == 0) {
            _FreeFxTag(cmd->startEffect.tag);
            break;
        }
        cmd->startEffect.tag->sysId = AudioPlayEffect(cmd->startEffect.tag->fxid,
            cmd->startEffect.tag->vol, cmd->startEffect.tag->lrpan);
        if (cmd->startEffect.tag->sysId) {
            _PlayFxTag(cmd->startEffect.tag);
            if (cmd->startEffect.tag->fbpan)
                AudioSetEffectFBPan(cmd->startEffect.tag->sysId, cmd->startEffect.tag->fbpan);
            if (cmd->startEffect.tag->pitch != 1.0f)
                AudioSetEffectPitch(cmd->startEffect.tag->sysId, cmd->startEffect.tag->pitch);
            cmd->startEffect.tag->state = 2;
            cmd->startEffect.tag->startTime = GetAudioTime();
        }
        break;
    case 4:
        cmd->changeEffect.tag->numCmds--;
        if (cmd->changeEffect.tag->state == 0) {
            _FreeFxTag(cmd->changeEffect.tag);
            break;
        }
        if (cmd->changeEffect.vol != -2.0f) {
            if (cmd->changeEffect.tag->state == 2)
                AudioSetEffectVolume(cmd->changeEffect.tag->sysId, cmd->changeEffect.vol);
            cmd->changeEffect.tag->vol = cmd->changeEffect.vol;
        }
        if (cmd->changeEffect.lrpan != -2.0f) {
            if (cmd->changeEffect.tag->state == 2)
                AudioSetEffectLRPan(cmd->changeEffect.tag->sysId, cmd->changeEffect.lrpan);
            cmd->changeEffect.tag->lrpan = cmd->changeEffect.lrpan;
        }
        if (cmd->changeEffect.fbpan != -2.0f) {
            if (cmd->changeEffect.tag->state == 2)
                AudioSetEffectFBPan(cmd->changeEffect.tag->sysId, cmd->changeEffect.fbpan);
            cmd->changeEffect.tag->fbpan = cmd->changeEffect.fbpan;
        }
        if (cmd->changeEffect.pitch != -2.0f) {
            if (cmd->changeEffect.tag->state == 2 && !cmd->changeEffect.tag->paused)
                AudioSetEffectPitch(cmd->changeEffect.tag->sysId, cmd->changeEffect.pitch);
            cmd->changeEffect.tag->pitch = cmd->changeEffect.pitch;
        }
        break;
    case 5:
        cmd->intEffect.tag->numCmds--;
        if (cmd->intEffect.tag->state == 0) {
            _FreeFxTag(cmd->intEffect.tag);
            break;
        }
        AudioStopEffect(cmd->intEffect.tag->sysId);
        if (cmd->intEffect.tag->loop) _InterruptFxTag(cmd->intEffect.tag);
        else _FreeFxTag(cmd->intEffect.tag);
        break;
    case 6:
        cmd->stopEffect.tag->numCmds--;
        if (cmd->stopEffect.tag->state == 0) {
            _FreeFxTag(cmd->stopEffect.tag);
            break;
        }
        if (cmd->stopEffect.tag->state == 2) AudioStopEffect(cmd->stopEffect.tag->sysId);
        _FreeFxTag(cmd->stopEffect.tag);
        break;
    case 7:
        _StopAllSequences();
        AudioStopAllEffects();
        for (tag = gPlayingFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            _FreeFxTag(tag);
        }
        for (tag = gInterruptedFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            _FreeFxTag(tag);
        }
        break;
    case 8:
        for (tag = gPlayingFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            if (tag->state == 2) AudioSetEffectPitch(tag->sysId, 0.0f);
            tag->paused = 1;
        }
        for (tag = gInterruptedFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            tag->paused = 1;
        }
        break;
    case 9:
        for (tag = gPlayingFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            if (tag->paused && tag->state == 2) {
                tag->paused = 0;
                AudioSetEffectPitch(tag->sysId, tag->pitch);
            }
        }
        for (tag = gInterruptedFxTags.head; tag; tag = next) {
            next = tag->ll.next;
            tag->paused = 0;
        }
        break;
    case 10:
        AudioSetMasterVolume(cmd->setMasterVol.vol, (unsigned int)(1000.0f * cmd->setMasterVol.time),
            cmd->setMasterVol.music, cmd->setMasterVol.sfx);
        break;
    case 11:
        AudioSetMono(cmd->mono.mono);
        break;
    }
    cmd->hdr.used=0;
    UNLOCK();
}

/* DC 0x8C0142FE; semantic MAT3 body with target-backed platform contracts. */
tAudioMsg *_GetCmd(void)
{
    int i;
    for (i=0; i<128; i++) {
        if (!gAudioCommandQueue[i].hdr.used) {
            gAudioCommandQueue[i].hdr.used=1;
            gAudioCommandQueue[i].hdr.type=-1;
            return &gAudioCommandQueue[i];
        }
    }
    return 0;
}

/* DC 0x8C014334; semantic MAT3 body with target-backed platform contracts. */
tFxSequence *_GetFxTag(void)
{
    tFxSequence *tag = gFreeFxTags.head;
    tag->id = (gNextFxId << gTagShift) | (tag->id & gTagMask);
    if (gNextFxId < (~gTagMask >> (gTagShift + 1))) gNextFxId = gNextFxId + 1;
    else gNextFxId = 1;
    tag->startTime = -1.0f;
    tag->data = 0;
    tag->vol = 0.0f;
    tag->lrpan = 0.0f;
    tag->fbpan = 0.0f;
    tag->pitch = 1.0f;
    RemoveLLE(&gFreeFxTags, tag);
    InsertLLE(&gInterruptedFxTags, tag, gInterruptedFxTags.head);
    tag->list = &gInterruptedFxTags;
    return tag;
}

/* DC 0x8C0143E8; semantic MAT3 body with target-backed platform contracts. */
void _FreeFxTag(tFxSequence *tag)
{
    tag->id &= gTagMask;
    tag->state = 0;
    if (tag->type == 1 && tag->data) {
        _FreeEmitter(tag->data);
        tag->data = 0;
    }
    if (!tag->numCmds) {
        RemoveLLE(tag->list, tag);
        InsertLLE(&gFreeFxTags, tag, gFreeFxTags.head);
        tag->list = &gFreeFxTags;
    }
}

/* DC 0x8C01443A; semantic MAT3 body with target-backed platform contracts. */
void _PlayFxTag(tFxSequence *tag)
{
    RemoveLLE(tag->list, tag);
    InsertLLE(&gPlayingFxTags, tag, gPlayingFxTags.head);
    tag->list = &gPlayingFxTags;
}

/* DC 0x8C014466; semantic MAT3 body with target-backed platform contracts. */
void _InterruptFxTag(tFxSequence *tag)
{
    tag->state = 3;
    RemoveLLE(tag->list, tag);
    InsertLLE(&gInterruptedFxTags, tag, gInterruptedFxTags.head);
    tag->list = &gInterruptedFxTags;
}

/* DC 0x8C01448E; semantic MAT3 body with target-backed platform contracts. */
tFxSequence *_ConvertTag(int tag)
{
    if (tag == -1) return 0;
    if (tag != gFxTags[tag & gTagMask].id) return 0;
    return &gFxTags[tag & gTagMask];
}

/* DC 0x8C0144BA; semantic MAT3 body with target-backed platform contracts. */
void AudPlaySong(int song, int loop)
{
    tAudioMsg *cmd;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 0;
    cmd->playSong.song = song;
    cmd->playSong.loop = loop;
    UNLOCK();
    _ProcessCommand(cmd);
}

/* DC 0x8C01450E; semantic MAT3 body with target-backed platform contracts. */
void AudKillSong(void)
{
    tAudioMsg *cmd;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 1;
    UNLOCK();
    _ProcessCommand(cmd);
}

/* DC 0x8C014552; semantic MAT3 body with target-backed platform contracts. */
int AudPlayEffect(int fxid, int subId, int replace, unsigned char priority)
{
    tAudioMsg *cmd;
    int tag;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 2;
    cmd->playEffect.replace = replace;
    cmd->playEffect.tag = _GetFxTag();
    cmd->playEffect.tag->type = 0;
    cmd->playEffect.tag->fxid = fxid;
    cmd->playEffect.tag->subId = subId;
    cmd->playEffect.tag->priority = priority;
    cmd->playEffect.tag->state = 1;
    cmd->playEffect.tag->paused = 0;
    cmd->playEffect.tag->numCmds = 1;
    cmd->playEffect.tag->loop = gEffectLoop[fxid];
    cmd->playEffect.vol = 1.0f;
    cmd->playEffect.lrpan = 0.0f;
    cmd->playEffect.fbpan = -2.0f;
    tag = cmd->playEffect.tag->id;
    UNLOCK();
    _ProcessCommand(cmd);
    return tag;
}

/* DC 0x8C014644; semantic MAT3 body with target-backed platform contracts. */
int AudPlayEffectVP(int fxid, int subId, int replace, unsigned char priority, float volume, float lrpan, float fbpan)
{
    tAudioMsg *cmd;
    int tag;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 2;
    cmd->playEffect.replace = replace;
    cmd->playEffect.tag = _GetFxTag();
    cmd->playEffect.tag->type = 0;
    cmd->playEffect.tag->fxid = fxid;
    cmd->playEffect.tag->subId = subId;
    cmd->playEffect.tag->priority = priority;
    cmd->playEffect.tag->state = 1;
    cmd->playEffect.tag->paused = 0;
    cmd->playEffect.tag->numCmds = 1;
    cmd->playEffect.tag->loop = gEffectLoop[fxid];
    cmd->playEffect.vol = volume < 0.0f ? 0.0f : volume > 1.0f ? 1.0f : volume;
    cmd->playEffect.lrpan = lrpan < -1.0f ? -1.0f : lrpan > 1.0f ? 1.0f : lrpan;
    cmd->playEffect.fbpan = fbpan < -1.0f ? -1.0f : fbpan > 1.0f ? 1.0f : fbpan;
    tag = cmd->playEffect.tag->id;
    UNLOCK();
    _ProcessCommand(cmd);
    return tag;
}

/* DC 0x8C01473A; semantic MAT3 body with target-backed platform contracts. */
int AudEffectPlaying(int tag)
{
    int playing;
    tFxSequence *t;
    LOCK();
    t = _ConvertTag(tag);
    if (t) {
        if (t->state == 1 || t->state == 3) playing = 1;
        else if (t->state != 2) playing = 0;
        else playing = AudioEffectPlaying(t->sysId);
    } else playing = 0;
    UNLOCK();
    return playing;
}

/* DC 0x8C0147AE; semantic MAT3 body with target-backed platform contracts. */
void _AudChangeEffect(int tag, float volume, float lrpan, float fbpan, float pitch)
{
    tAudioMsg *cmd = 0;
    tFxSequence *t = _ConvertTag(tag);
    if (t && ((-2.0f != volume && volume != t->vol) ||
              (-2.0f != lrpan && lrpan != t->lrpan) ||
              (-2.0f != fbpan && fbpan != t->fbpan) ||
              (-2.0f != pitch && pitch != t->pitch))) {
        cmd = _GetCmd();
        cmd->hdr.cmd = 4;
        cmd->changeEffect.tag = t;
        cmd->changeEffect.tag->numCmds++;
        cmd->changeEffect.vol = (-2.0f != volume && volume != t->vol) ? volume : -2.0f;
        cmd->changeEffect.lrpan = (-2.0f != lrpan && lrpan != t->lrpan) ? lrpan : -2.0f;
        cmd->changeEffect.fbpan = (-2.0f != fbpan && fbpan != t->fbpan) ? fbpan : -2.0f;
        cmd->changeEffect.pitch = (-2.0f != pitch && pitch != t->pitch) ? pitch : -2.0f;
    }
    if (cmd) _ProcessCommand(cmd);
}

/* DC 0x8C0148B8; semantic MAT3 body with target-backed platform contracts. */
void AudChangeEffectVol(int tag, float volume)
{
    LOCK();
    if (volume < 0.0f) volume = 0.0f;
    else if (volume > 1.0f) volume = 1.0f;
    _AudChangeEffect(tag, volume, -2.0f, -2.0f, -2.0f);
    UNLOCK();
}

/* DC 0x8C014922; semantic MAT3 body with target-backed platform contracts. */
void AudChangeEffectLRPan(int tag,float pan)
{
    LOCK();
    if(pan < -1.0f)pan=-1.0f; else if(pan>1.0f)pan=1.0f;
    _AudChangeEffect(tag,-2.0f,pan,-2.0f,-2.0f);
    UNLOCK();
}

/* DC 0x8C014992; semantic MAT3 body with target-backed platform contracts. */
void AudChangeEffectFBPan(int tag,float pan)
{
    LOCK();
    if(pan < -1.0f)pan=-1.0f; else if(pan>1.0f)pan=1.0f;
    _AudChangeEffect(tag,-2.0f,-2.0f,pan,-2.0f);
    UNLOCK();
}

/* DC 0x8C014A02; semantic MAT3 body with target-backed platform contracts. */
void AudChangeEffectPitch(int tag, float pitch)
{
    LOCK();
    _AudChangeEffect(tag, -2.0f, -2.0f, -2.0f, pitch);
    UNLOCK();
}

/* DC 0x8C014A68; semantic MAT3 body with target-backed platform contracts. */
void AudStopEffect(int tag)
{
    tAudioMsg *cmd = 0;
    tFxSequence *t;
    LOCK();
    t = _ConvertTag(tag);
    if (t) {
        cmd = _GetCmd();
        cmd->hdr.cmd = 6;
        cmd->stopEffect.tag = t;
        cmd->stopEffect.tag->numCmds++;
    }
    UNLOCK();
    if (cmd) _ProcessCommand(cmd);
}

/* DC 0x8C014ADC; semantic MAT3 body with target-backed platform contracts. */
void AudStopAllEffects(void)
{
    tAudioMsg *cmd;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 7;
    UNLOCK();
    _ProcessCommand(cmd);
}

/* DC 0x8C014B20; semantic MAT3 body with target-backed platform contracts. */
void AudPauseAllEffects(void)
{
    tAudioMsg *cmd;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 8;
    UNLOCK();
    _ProcessCommand(cmd);
}

/* DC 0x8C014B64; semantic MAT3 body with target-backed platform contracts. */
void AudResumeAllEffects(void)
{
    tAudioMsg *cmd;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 9;
    UNLOCK();
    _ProcessCommand(cmd);
}

/* DC 0x8C014BA8; semantic MAT3 body with target-backed platform contracts. */
int _WeightedPriority(tFxSequence *tag)
{
    int priority;
    priority = (int)tag->priority;
    priority += 80.0f * (1.0f - tag->vol);
    if (tag->loop) priority += 8;
    else if (tag->priority && tag->startTime >= 0.0f && tag->state == 2)
        priority += AudioTimeoutPerc(tag->startTime, 0.25f);
    return priority;
}

/* DC 0x8C014C14; semantic MAT3 body with target-backed platform contracts. */
void _InsertSorted(tFxSequence *tag)
{
    anon_te_tag_c *stag;
    stag = &gSortedFxTags[gSortedFxTagTree.numElements];
    stag->tag = tag;
    InsertBSTE(&gSortedFxTagTree, stag);
}

/* DC 0x8C014C34; semantic MAT3 body with target-backed platform contracts. */
int _OrderFx(void * el1, void * el2)
{
    return ((anon_te_tag_c *)el1)->tag->wpriority - ((anon_te_tag_c *)el2)->tag->wpriority;
}

/* DC 0x8C014C68; semantic MAT3 body with target-backed platform contracts. */
void _UpdateFx(void * element, int elNum)
{

    anon_te_tag_c *stag = element;
    tAudioMsg *cmd;
    tFxSequence *tag = stag->tag;
    if (tag->state != 1) {
        if (elNum >= 0.75f * gMaxSfx ||
            (tag->loop && tag->state == 2 && tag->vol <= 0.0f &&
             AudioTimeout(tag->startTime, 0.200000004f))) {
            if (stag->tag->state == 2) {
                cmd = _GetCmd();
                gStopCmds[gNumStopCmds++] = cmd;
                cmd->hdr.cmd = 5;
                cmd->intEffect.tag = stag->tag;
                cmd->intEffect.tag->numCmds++;
            }
        } else if (stag->tag->state != 2 &&
                   (stag->tag->vol > 0.100000002f || (!stag->tag->loop && stag->tag->type == 1))) {
            cmd = _GetCmd();
            gStartCmds[gNumStartCmds++] = cmd;
            cmd->hdr.cmd = 3;
            cmd->startEffect.tag = stag->tag;
            cmd->startEffect.tag->numCmds++;
        }
    }
}

/* DC 0x8C014D62; semantic MAT3 body with target-backed platform contracts. */
void _ProcessPriorities(void)
{
    tFxSequence *tag, *next;
    int i;
    LOCK();
    ClearBST(&gSortedFxTagTree);
    gNumStopCmds = 0;
    gNumStartCmds = 0;
    for (tag = gPlayingFxTags.head; tag; tag = next) {
        next = tag->ll.next;
        if (!AudioEffectPlaying(tag->sysId)) {
            if (tag->loop) _InterruptFxTag(tag);
            else _FreeFxTag(tag);
        } else {
            tag->wpriority = _WeightedPriority(tag);
            _InsertSorted(tag);
        }
    }
    gNumFxPlaying = gPlayingFxTags.numElements;
    gNumFxInterrupted = 0;
    gNumFxPending = 0;
    for (tag = gInterruptedFxTags.head; tag; tag = next) {
        next = tag->ll.next;
        if (!tag->loop) {
            tag->startTime -= 1.0f;
            if (tag->startTime < -10.0f) {
                _FreeFxTag(tag);
                continue;
            }
        }
        if (tag->state == 1) gNumFxPending++;
        if (tag->state != 1) gNumFxInterrupted++;
        tag->wpriority = _WeightedPriority(tag);
        _InsertSorted(tag);
    }
    WalkBST(&gSortedFxTagTree, _UpdateFx);
    UNLOCK();
    for (i = 0; i < gNumStopCmds; i++) _ProcessCommand(gStopCmds[i]);
    for (i = 0; i < gNumStartCmds; i++) _ProcessCommand(gStartCmds[i]);
}

/* DC 0x8C014EFA; semantic MAT3 body with target-backed platform contracts. */
void _AudSetMasterVol(float volume, float time, int music, int sfx)
{
    tAudioMsg *cmd;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 10;
    cmd->setMasterVol.vol = volume < 0.0f ? 0.0f : volume > 1.0f ? 1.0f : volume;
    cmd->setMasterVol.time = time < 0.0f ? 0.0f : time;
    cmd->setMasterVol.music = music;
    cmd->setMasterVol.sfx = sfx;
    UNLOCK();
    _ProcessCommand(cmd);
}

/* DC 0x8C014F86; semantic MAT3 body with target-backed platform contracts. */
void AudSetFxVol(float volume, float time)
{
    _AudSetMasterVol(volume, time, 0, 1);
}

/* DC 0x8C014F8C; semantic MAT3 body with target-backed platform contracts. */
void AudSetMusicVol(float volume, float time)
{
    _AudSetMasterVol(volume, time, 1, 0);
}

/* DC 0x8C014F92; semantic MAT3 body with target-backed platform contracts. */
void AudMono(int mono)
{
    tAudioMsg *cmd;
    LOCK();
    cmd = _GetCmd();
    cmd->hdr.cmd = 11;
    cmd->mono.mono = mono;
    UNLOCK();
    _ProcessCommand(cmd);
}

/* DC 0x8C014FE0; semantic MAT3 body with target-backed platform contracts. */
anon_ll_free_3c *_NewListener(void)
{
    anon_ll_free_3c *lis = gFreeListeners.head;
    RemoveLLE(&gFreeListeners, lis);
    lis->free = 0;
    return lis;
}

/* DC 0x8C014FFE; semantic MAT3 body with target-backed platform contracts. */
void _FreeListener(anon_ll_free_3c *lis)
{
    if (lis->active) {
        RemoveLLE(&gActiveListeners, lis);
        lis->active = 0;
    }
    InsertLLE(&gFreeListeners, lis, gFreeListeners.head);
    lis->free = 1;
}

/* DC 0x8C01502C; semantic MAT3 body with target-backed platform contracts. */
/* _ValidateListener: target validation remains external; original diagnostic source is unproved. */

/* DC 0x8C01504E; semantic MAT3 body with target-backed platform contracts. */
tSfxEmitter *_NewEmitter(void)
{
    tSfxEmitter *em = gFreeEmitters.head;
    if (!em) return 0;
    RemoveLLE(&gFreeEmitters, em);
    em->free = 0;
    return em;
}

/* DC 0x8C015076; semantic MAT3 body with target-backed platform contracts. */
void _FreeEmitter(tSfxEmitter *em)
{
    if (em->active) {
        RemoveLLE(&gActiveEmitters, em);
        em->active = 0;
    }
    InsertLLE(&gFreeEmitters, em, gFreeEmitters.head);
    em->free = 1;
}

/* DC 0x8C0150F4; semantic MAT3 body with target-backed platform contracts. */
/* _ValidateEmitter: target validation remains external; original diagnostic source is unproved. */

/* DC 0x8C015116; semantic MAT3 body with target-backed platform contracts. */
void AudInit3D(int numListeners, int numEmitters, float footUnits, float frontDistance, int doppler)
{
    int i;
    InitLL(&gFreeListeners, 0, 1);
    InitLL(&gActiveListeners, 0, 1);
    InitLL(&gFreeEmitters, 0, 1);
    InitLL(&gActiveEmitters, 0, 1);
    if (numListeners <= 0 || numEmitters <= 0) return;
    gNumListeners = numListeners;
    gListeners = AllocateMemoryHeap(0, gNumListeners * sizeof(*gListeners));
    PurgeableMemory(gListeners, 0);
    for (i = 0; i < gNumListeners; i++) {
        gListeners[i].free = 1;
        gListeners[i].active = 0;
        InsertLLE(&gFreeListeners, &gListeners[i], gFreeListeners.head);
    }
    gNumEmitters = numEmitters;
    gEmitters = AllocateMemoryHeap(0, gNumEmitters * sizeof(*gEmitters));
    PurgeableMemory(gEmitters, 0);
    for (i = 0; i < gNumEmitters; i++) {
        gEmitters[i].free = 1;
        gEmitters[i].active = 0;
        InsertLLE(&gFreeEmitters, &gEmitters[i], gFreeEmitters.head);
    }
    gFrontDistance = frontDistance;
    gSoundSpeed = 1100.0f * footUnits;
    gCalcDoppler = doppler;
}

/* DC 0x8C015236; semantic MAT3 body with target-backed platform contracts. */
void AudShutdown3D(void)
{
    if(gListeners){DeallocateMemory(gListeners);gListeners=0;}
    if(gEmitters){DeallocateMemory(gEmitters);gEmitters=0;}
}

/* DC 0x8C015266; semantic MAT3 body with target-backed platform contracts. */
int AudAddListener(float *pos, float *dir, float *look, float *up)
{
    anon_ll_free_3c *lis;
    if (!gListeners) return -1;
    LOCK();
    lis = _NewListener();
    lis->pos[0] = pos[0]; lis->pos[1] = pos[1]; lis->pos[2] = pos[2];
    lis->dir[0] = dir[0]; lis->dir[1] = dir[1]; lis->dir[2] = dir[2];
    lis->look[0] = look[0]; lis->look[1] = look[1]; lis->look[2] = look[2];
    lis->up[0] = up[0]; lis->up[1] = up[1]; lis->up[2] = up[2];
    InsertLLE(&gActiveListeners,lis,gActiveListeners.head);
    lis->active=1;
    UNLOCK();
    return (int)lis;
}

/* DC 0x8C015332; semantic MAT3 body with target-backed platform contracts. */
void AudRemoveListener(int lis)
{
    if (lis == -1) return;
    LOCK();
    _ValidateListener((anon_ll_free_3c *)lis);
    _FreeListener((anon_ll_free_3c *)lis);
    UNLOCK();
}

/* DC 0x8C0153D8; semantic MAT3 body with target-backed platform contracts. */
void AudUpdateListener(int lis, float *pos, float *dir, float *look, float *up)
{
    anon_ll_free_3c *l;
    if (lis == -1) return;
    LOCK();
    _ValidateListener((anon_ll_free_3c *)lis);
    l = (anon_ll_free_3c *)lis;
    l->pos[0] = pos[0]; l->pos[1] = pos[1]; l->pos[2] = pos[2];
    l->dir[0] = dir[0]; l->dir[1] = dir[1]; l->dir[2] = dir[2];
    l->look[0] = look[0]; l->look[1] = look[1]; l->look[2] = look[2];
    l->up[0] = up[0]; l->up[1] = up[1]; l->up[2] = up[2];
    UNLOCK();
}

/* DC 0x8C01548E; semantic MAT3 body with target-backed platform contracts. */
int AudEmitEffect(float *pos, float *dir, float fadeDistance, float fadeCompression, float maxVol, float minVol, int fxid, int subId, int replace, unsigned char priority)
{
    tSfxEmitter *em;
    tAudioMsg *cmd;
    int tag;
    if (!gEmitters) return -1;
    LOCK();
    em = _NewEmitter();
    if (!em) {UNLOCK(); return -1;}
    em->pos[0] = pos[0]; em->pos[1] = pos[1]; em->pos[2] = pos[2];
    em->fadeDistance = fadeDistance;
    em->maxVol = maxVol < 0.0f ? 0.0f : maxVol > 1.0f ? 1.0f : maxVol;
    em->minVol = minVol < 0.0f ? 0.0f : minVol > 1.0f ? 1.0f : minVol;
    em->basePitch = 1.0f;
    cmd = _GetCmd();
    cmd->hdr.cmd = 2;
    cmd->playEffect.replace = replace;
    cmd->playEffect.tag = _GetFxTag();
    cmd->playEffect.tag->type = 1;
    cmd->playEffect.tag->fxid = fxid;
    cmd->playEffect.tag->subId = subId;
    cmd->playEffect.tag->priority = priority;
    cmd->playEffect.tag->state = 1;
    cmd->playEffect.tag->paused = 0;
    cmd->playEffect.tag->numCmds = 1;
    cmd->playEffect.tag->loop = gEffectLoop[fxid];
    cmd->playEffect.vol = 0.0f;
    cmd->playEffect.lrpan = 0.0f;
    cmd->playEffect.fbpan = -2.0f;
    cmd->playEffect.tag->data = em;
    tag = em->sndTag = cmd->playEffect.tag->id;
    UNLOCK();
    _ProcessCommand(cmd);
    LOCK();
    if (!em->free) {InsertLLE(&gActiveEmitters,em,gActiveEmitters.head);em->active=1;}
    UNLOCK();
    return tag;
}

/* DC 0x8C015618; semantic MAT3 body with target-backed platform contracts. */
void AudStopEmitter(int tag)
{
    tFxSequence *t;
    LOCK();
    t = _ConvertTag(tag);
    if (t) {
        _ValidateEmitter(t->data);
        UNLOCK();
        AudStopEffect(tag);
        return;
    }
    UNLOCK();
}

/* DC 0x8C0156CC; semantic MAT3 body with target-backed platform contracts. */
int AudEmitterPlaying(int tag)
{
    tFxSequence *t;
    LOCK();
    t = _ConvertTag(tag);
    if (!t) {UNLOCK(); return 0;}
    _ValidateEmitter(t->data);
    UNLOCK();
    return AudEffectPlaying(tag);
}

/* DC 0x8C01574C; semantic MAT3 body with target-backed platform contracts. */
void AudUpdateEmitter(int tag,float *pos,float *dir,float maxVol,float pitch)
{
    tFxSequence *t;
    tSfxEmitter *em;
    LOCK();
    t = _ConvertTag(tag);
    if (t) {
        em = t->data;
        _ValidateEmitter(em);
        em->pos[0] = pos[0]; em->pos[1] = pos[1]; em->pos[2] = pos[2];
        em->maxVol = maxVol < 0.0f ? 0.0f : maxVol > 1.0f ? 1.0f : maxVol;
        em->basePitch = pitch;
    }
    UNLOCK();
}

/* DC 0x8C0157E8; semantic MAT3 body with target-backed platform contracts. */
void _ProcessEmitters(void)
{
    tSfxEmitter *em, *next;
    tFxSequence *tag;
    float vol, lrpan, fbpan, pitch;
    LOCK();
    for (em = gActiveEmitters.head; em; em = next) {
        next = em->ll.next;
        tag = _ConvertTag(em->sndTag);
        if (tag->state == 2 && !tag->numCmds && !tag->loop && !AudioEffectPlaying(tag->sysId))
            _FreeFxTag(tag);
        else if (!tag->paused) {
            _UpdateEmitter(em, &vol, &lrpan, &fbpan, &pitch);
            _AudChangeEffect(em->sndTag, vol, lrpan, fbpan, pitch);
        }
    }
    UNLOCK();
}

/* DC 0x8C0158D4; semantic MAT3 body with target-backed platform contracts. */
void _UpdateEmitter(tSfxEmitter *em, float *vol, float *lrpan, float *fbpan, float *pitch)
{
    anon_ll_free_3c *lis;
    float dir[3];
    float dist;
    float lrpans[5];
    float fbpans[5];
    float vols[5];
    float lrp, fbp;
    float totalVol = 0.0f;
    int l;
    *pitch = em->basePitch;
    for (lis = gActiveListeners.head, l = 0; lis; lis = lis->ll.next, l++) {
        dir[0] = em->pos[0] - lis->pos[0];
        dir[1] = em->pos[1] - lis->pos[1];
        dir[2] = em->pos[2] - lis->pos[2];
        dist = TB_Vector3Normalize(dir);
        {
            float lookX = lis->look[0];
            float lookY = lis->look[1];
            float lookZ = lis->look[2];
            float upX = lis->up[0];
            float upY = lis->up[1];
            float fadeDistance = em->fadeDistance;
            float upZ = lis->up[2];
            {
                float rightX, rightY, rightZ;
                /* Listener right axis is up cross look. */
                rightX = upY * lookZ - upZ * lookY;
                rightY = upZ * lookX - upX * lookZ;
                rightZ = upX * lookY - upY * lookX;
                lrpans[l] = rightX * dir[0] + rightY * dir[1] + rightZ * dir[2];
                fbpans[l] = lookX * dir[0] + lookY * dir[1] + lookZ * dir[2];
            }
            if (fadeDistance > 0.0f) {
                vols[l] = em->maxVol - (em->maxVol - em->minVol) * (dist / fadeDistance);
                if (vols[l] < 0.0f) vols[l] = 0.0f;
            } else vols[l] = em->maxVol;
        }
        totalVol += vols[l];
        lrpans[l] -= lrpans[l] * vols[l];
    }
    *vol = totalVol < 0.0f ? 0.0f : totalVol > 1.0f ? 1.0f : totalVol;
    lrp = 0.0f;
    fbp = 0.0f;
    for (l = 0; l < gActiveListeners.numElements; l++) {
        if (totalVol) {
            lrp += lrpans[l] * (vols[l] / totalVol);
            fbp += fbpans[l] * (vols[l] / totalVol);
        }
    }
    *lrpan = lrp < -1.0f ? -1.0f : lrp > 1.0f ? 1.0f : lrp;
    *fbpan = fbp < -1.0f ? -1.0f : fbp > 1.0f ? 1.0f : fbp;
}

/* DC 0x8C015B8C; semantic MAT3 body with target-backed platform contracts. */
anon_ll_id_3c *_NewSequence(void)
{
    anon_ll_id_3c *s = gFreeSequences.head;
    RemoveLLE(&gFreeSequences, s);
    s->free = 0;
    s->id = (gNextSeqId << gTagShift) | (s->id & gTagMask);
    if (gNextSeqId < (~gTagMask >> (gTagShift + 1))) gNextSeqId = gNextSeqId + 1;
    else gNextSeqId = 1;
    return s;
}

/* DC 0x8C015BDC; semantic MAT3 body with target-backed platform contracts. */
void _FreeSequence(anon_ll_id_3c *s)
{
    s->id &= gTagMask;
    if (s->active) {
        RemoveLLE(&gActiveSequences, s);
        s->active = 0;
    }
    InsertLLE(&gFreeSequences, s, gFreeSequences.head);
    s->free = 1;
}

/* DC 0x8C015C18; semantic MAT3 body with target-backed platform contracts. */
anon_ll_id_3c *_ConvertSequence(int tag)
{
    if(tag==-1)return 0;
    if(gSequences[tag & gTagMask].id!=tag)return 0;
    return &gSequences[tag & gTagMask];
}

/* DC 0x8C015D58; semantic MAT3 body with target-backed platform contracts. */
void AudStopSequence(int tag)
{
    anon_ll_id_3c *s;
    LOCK();
    s=_ConvertSequence(tag);
    if(s)s->stop=1;
    UNLOCK();
}

/* DC 0x8C015DA2; semantic MAT3 body with target-backed platform contracts. */
void _StopAllSequences(void)
{
    anon_ll_id_3c *s;
    for (s = gActiveSequences.head; s; s = s->ll.next) s->stop = 1;
}

/* DC 0x8C015DBA; semantic MAT3 body with target-backed platform contracts. */
void _ProcessSequences(void)
{
    anon_ll_id_3c *s, *next;
    tFxSequence *tag;
    for (s = gActiveSequences.head; s; s = next) {
        next = s->ll.next;
        if (s->stop || (s->sndTag == -1 && AudioTimeout(s->startTime, s->timeout))) {
            AudStopEffect(s->sndTag);
            _FreeSequence(s);
        } else {
            tag = _ConvertTag(s->sndTag);
            if (tag) {
                if (tag->state == 2 && !tag->numCmds && !AudioEffectPlaying(tag->sysId))
                    _FreeFxTag(tag);
                else continue;
            }
            while (s->ids[s->next] == -1) {
                s->next++;
                if (s->next >= 4) break;
            }
            if (s->next >= 4) _FreeSequence(s);
            else {
                s->sndTag = AudPlayEffect(s->ids[s->next++], s->subId, s->replace, s->priority);
                _ConvertTag(s->sndTag)->data = s;
            }
        }
    }
}
