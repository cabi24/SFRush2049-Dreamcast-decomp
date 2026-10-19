/* Audio64 setup/time/fade/error wrappers, target-derived SH-4 C89.
 * Anonymous map identities retained. Global D28 owned by dc_sound remains external. */
#include "dc_audio64_init_types.h"
extern int func_8C09F2CC(int);
extern void func_8C09EEE6(int);
extern int func_8C09C06C(void);
extern int func_8C010080(int);
extern void func_8C0D9484(int, void (*)(void), int, int);
extern void AudioSfxInit(int);
extern void func_8C069E34(void);
extern void func_8C0A273A(void);
extern unsigned int func_8C0D8726(void);
extern unsigned int func_8C0D8730(unsigned int, unsigned int);
extern unsigned int func_8C0D8736(unsigned int);
extern int func_8C0A18E6(unsigned int *);
extern void AudioSfxServer(void);
extern void func_8C06A1E4(void);
extern tDCAudioErrorPrefix *func_8C09E3A2(int);
extern void func_8C09E946(int);
extern int func_8C09EA0C(void);
extern int func_8C09C354(const char *, unsigned int *);
extern void *AllocateMemoryHeap(int, unsigned int);
extern int func_8C09C456(const char *, void *, unsigned int);
extern int func_8C09EF10(int, void *, unsigned int, int);
extern void DeallocateMemory(void *);
extern tDCAudioErrorPrefix *func_8C0A1832(void);
extern int func_8C0A26C8(void);
extern void func_8C09EE78(void *, unsigned char *, unsigned char *, signed char *);
extern int func_8C09C044(char *, const char *, char *);

/* D48..78; target references B220/B2DC/B0C4/B0B4..E0/AF0C.
 * Natural3-byte alignment after gMono is uncredited. */
tDCAudioErrorPrefix *dcAudioErrorA = 0;
tDCAudioErrorPrefix *dcAudioErrorB = 0;
signed char gMono = 0;
float dcAudioFadeAStart = 0.0f;
float dcAudioFadeADuration = 0.0f;
float dcAudioFadeAFrom = 1.0f;
float dcAudioFadeATo = 1.0f;
float dcAudioFadeBStart = 0.0f;
float dcAudioFadeBDuration = 0.0f;
float dcAudioFadeBFrom = 1.0f;
float dcAudioFadeBTo = 1.0f;
int dcAudioTimerInited = 0;
/* B843C..844C;0image credit. */
float dcAudioCurrentA;
float dcAudioCurrentB;
float dcAudioTime;
unsigned int dcAudioPreviousTick;

void func_8C06AE4C(void);
void func_8C06B052(int, int, int);
void func_8C06B182(int, int, int);
int func_8C06B1C8(void);
int func_8C06B22C(void);
void func_8C06B254(void *);
void func_8C06B298(const char *, ...);

void func_8C06ADD4(int unused, int voices)
{
    int previous;
    func_8C06B182(1, 0, 0);
    if (!func_8C09F2CC(1))
        func_8C06B298("acSetTransferMode( AC_TRANSFER_DMA ) failed");
    func_8C09EEE6(500000);
    if (!func_8C09C06C()) func_8C06B298("a64FileInit failed");
    previous = func_8C010080(15);
    func_8C0D9484(0x900, func_8C06AE4C, 0x58, 0);
    dcAudioTime = 0.0f;
    func_8C010080(previous);
    dcAudioCurrentA = 1.0f;
    dcAudioCurrentB = 1.0f;
    AudioSfxInit(voices);
    func_8C069E34();
}

void func_8C06AE46(void)
{
    func_8C0A273A();
}

void func_8C06AE4C(void)
{
    unsigned int current;
    float elapsed;
    if (!dcAudioTimerInited) {
        dcAudioTimerInited = 1;
        dcAudioPreviousTick = func_8C0D8726();
    }
    current = func_8C0D8726();
    elapsed = (float)func_8C0D8736(func_8C0D8730(dcAudioPreviousTick, current)) / 1000000.0f;
    dcAudioPreviousTick = current;
    dcAudioTime += elapsed;
    if (dcAudioTime >= 14400.0f) dcAudioTime = 0.0f;
}

unsigned int func_8C06AEB2(void)
{
    unsigned int result;
    func_8C0A18E6(&result);
    return result;
}

float func_8C06AEC8(void)
{
    return dcAudioTime;
}

int func_8C06AF30(float start, float duration)
{
    float end = start + duration;
    if (start > dcAudioTime) end -= 14400.0f;
    return dcAudioTime > end;
}

float func_8C06AF48(float start, float duration)
{
    float current = dcAudioTime;
    if (start > dcAudioTime) current += 14400.0f;
    return (current - start) / duration;
}

void func_8C06AF60(unsigned int milliseconds, int channelA, int channelB, float to)
{
    float duration = (float)milliseconds / 1000.0f;
    if (channelA) {
        dcAudioFadeAStart = dcAudioTime;
        dcAudioFadeADuration = duration;
        dcAudioFadeAFrom = dcAudioCurrentA;
        dcAudioFadeATo = (gMono ? .60000004f : 1.0f) * to;
    }
    if (channelB) {
        dcAudioFadeBStart = dcAudioTime;
        dcAudioFadeBDuration = duration;
        dcAudioFadeBFrom = dcAudioCurrentB;
        dcAudioFadeBTo = to;
    }
}

void func_8C06AFC4(void)
{
    if (dcAudioCurrentA != dcAudioFadeATo) {
        if (func_8C06AF30(dcAudioFadeAStart, dcAudioFadeADuration)) dcAudioCurrentA = dcAudioFadeATo;
        else dcAudioCurrentA = dcAudioFadeAFrom + (dcAudioFadeATo - dcAudioFadeAFrom) * func_8C06AF48(dcAudioFadeAStart, dcAudioFadeADuration);
    }
    if (dcAudioCurrentB != dcAudioFadeBTo) {
        if (func_8C06AF30(dcAudioFadeBStart, dcAudioFadeBDuration)) dcAudioCurrentB = dcAudioFadeBTo;
        else dcAudioCurrentB = dcAudioFadeBFrom + (dcAudioFadeBTo - dcAudioFadeBFrom) * func_8C06AF48(dcAudioFadeBStart, dcAudioFadeBDuration);
    }
    AudioSfxServer();
    func_8C06A1E4();
}

void func_8C06B052(int driver, int param, int interrupts)
{
    const char *filename;
    unsigned int size = 0;
    unsigned int alignedSize;
    void *data;
    filename = (const char *)size;
    dcAudioErrorB = func_8C09E3A2(0);
    func_8C09C06C();
    func_8C09E946(interrupts);
    if (!func_8C09EA0C()) func_8C06B298("acIntInit failed");
    if (driver == 1) filename = "audio64.drv";
    else if (driver == 2) filename = "midida.drv";
    else func_8C06B298("Invalid driver type in _AcSetup");
    if (!func_8C09C354(filename, &size)) func_8C06B298("a64FileGetSize failed");
    alignedSize = (size + 2047) & ~2047;
    data = AllocateMemoryHeap(0, alignedSize);
    if (!func_8C09C456(filename, data, alignedSize)) func_8C06B298("a64FileLoad failed");
    if (!func_8C09EF10(driver, data, size, param)) func_8C06B298("acSystemInit failed");
    func_8C06B254(data);
    DeallocateMemory(data);
}

void func_8C06B182(int driver, int param, int interrupts)
{
    func_8C06B052(driver, param, interrupts);
    dcAudioErrorA = func_8C0A1832();
    if (!func_8C0A26C8()) func_8C06B298("amInit failed");
}

int func_8C06B1A8(void)
{
    int failed = 0;
    if (func_8C06B1C8()) failed = 1;
    if (func_8C06B22C()) failed = 1;
    return failed;
}

int func_8C06B1C8(void)
{
    unsigned int error;
    if (!dcAudioErrorA) return 0;
    error = dcAudioErrorA->error;
    if (!error || error == 301) return 0;
    dcAudioErrorA->error = 0;
    dcAudioErrorA->flag = 0;
    return 1;
}

int func_8C06B22C(void)
{
    unsigned int error;
    if (!dcAudioErrorB) return 0;
    error = dcAudioErrorB->error;
    if (!error || error == 301) return 0;
    dcAudioErrorB->error = 0;
    dcAudioErrorB->flag = 0;
    return 1;
}

void func_8C06B254(void *data)
{
    unsigned char major = 0;
    unsigned char minor = 0;
    signed char patch = 0;
    func_8C09EE78(data, &major, &minor, &patch);
    if (major != 1 || minor != 90 || patch != 102)
        func_8C06B298("Audio64 library version doesn't match Audio64 driver version!");
}

void func_8C06B298(const char *format, ...)
{
    char buffer[256];
    char *args;
    if ((unsigned int)(&format + 1) & 3) args = (char *)&format + 8;
    else args = (char *)&format + 4;
    func_8C09C044(buffer, format, args);
    func_8C06B22C();
    func_8C06B1C8();
}
