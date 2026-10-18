#include "dc_types.h"
/* Private DC audio layouts. Gaps unknown. Target listing and numeric pool proofs in evidence/target.txt. */
#ifndef AUDIO_TYPES_H
#define AUDIO_TYPES_H
typedef struct anon_id_basePitchRpm_1c {
 int id; /* +0:1BCA idlong; specs stride1C at1BC2/1BC6; pair stride38 at1B92 */
 unsigned short basePitchRpm; /* +4:1F5A/1F5C unsigned basePitchRpm+4;1F5E/1F62 pitchRpmRange+6 */
 unsigned short pitchRpmRange; /* +6:1F5A/1F5C unsigned basePitchRpm+4;1F5E/1F62 pitchRpmRange+6 */
 unsigned short lowVolRpm; /* +8:1F5A/1F5C unsigned basePitchRpm+4;1F5E/1F62 pitchRpmRange+6 */
 unsigned short midVolRpm; /* +A:1F5A/1F5C unsigned basePitchRpm+4;1F5E/1F62 pitchRpmRange+6 */
 unsigned short hiVolRpm; /* +C:1F5A/1F5C unsigned basePitchRpm+4;1F5E/1F62 pitchRpmRange+6 */
 /* Natural two-byte alignment gap at+E..10; excluded from data credit. */
 float lowVol; /* +10:1F9E/1FA4 lowVol+10 */
 float midVol; /* +14:vol interpolation+14; see1FE0..2062 */
 float hiVol; /* +18:vol interpolation+18; see1FE0..2062 */
} anon_id_basePitchRpm_1c;
typedef struct anon_sound_vol_14 {
 int sound; /* +0:1534 longstore */
 float vol; /* +4:1538/153A */
 float pitch; /* +8:153C/153E */
 float lrpan; /* +C:1540/1542 */
 float fbpan; /* +10:1544/1548 */
} anon_sound_vol_14;
typedef struct anon_specs_state_54 { anon_id_basePitchRpm_1c *specs; anon_sound_vol_14 state[2]; anon_sound_vol_14 hum,rev; } anon_specs_state_54;
typedef struct anon_dirt_water_3c { anon_sound_vol_14 dirt,water,pave; } anon_dirt_water_3c;
typedef struct anon_peak_peakVec_18 { float peak; float peakVec[3]; float time; int bump; } anon_peak_peakVec_18;




typedef struct tAudioCamera {
 float uvs[3][3]; /* +0:1852/1856 base+0c;1860+18 */
 char pad0[0x74]; /* target stride; see notes */
} tAudioCamera;
typedef struct tAudioLink {
 char pad0[0x8];
 unsigned char owner; /* +8:3916..3926 index*10+8 */
 char pad1[0x1]; /* target stride; see notes */
} tAudioLink;
typedef char check_model[(sizeof(tModelData)==0x808)?1:-1]; /* 1868..1870 index*808 */
typedef char check_spec[(sizeof(anon_id_basePitchRpm_1c)==0x1c)?1:-1];
typedef char check_state[(sizeof(anon_sound_vol_14)==0x14)?1:-1];
typedef char check_engine[(sizeof(anon_specs_state_54)==0x54)?1:-1];
typedef char check_bump[(sizeof(anon_peak_peakVec_18)==0x18)?1:-1];
#define gGameCar ((tCarData *)0x8C004000)
#define gPlayers ((tPadState *)0x8C006000)
extern tModelData model[];
extern tAudioCamera gCamera[4];
extern tAudioLink gLink[13];
extern float unitmatrix[4][3];
extern signed char gCarHorn[5][13],gOptions[41],gTrackNum;
extern signed short gNumPlayers,gCountDown;
extern unsigned int gGameState;
extern int gRace;
extern volatile float gSeconds;
extern volatile unsigned int gFrameCount;
extern float gRetraceRate;
float Random(float);
void BodyVector(float *,float *,float (*)[3]);
int AudAddListener(float *,float *,float *,float *);
void AudUpdateListener(int,float *,float *,float *,float *);
void AudRemoveListener(int);
void AudSetFxVol(float,float);void AudSetMusicVol(float,float);void AudPlaySong(int,int);
int AudPlayEffect(int,int,int,unsigned char);
int AudPlayEffectVP(int,int,int,unsigned char,float,float,float);
int AudEmitEffect(float *,float *,float,float,float,float,int,int,int,unsigned char);
void AudUpdateEmitter(int,float *,float *,float,float);
void AudStopEmitter(int);void AudStopEffect(int);int AudEmitterPlaying(int);
void AudChangeEffectVol(int,float);void AudChangeEffectPitch(int,float);
void _InitSoundState(anon_sound_vol_14 *);void _StopSound(anon_sound_vol_14 *,int);
void StartGameMusic(void);void PauseGameMusic(void);void ResumeGameMusic(void);
#endif
