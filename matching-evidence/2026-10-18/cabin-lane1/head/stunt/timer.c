/* timer.c as one translation unit for the DC exact-link test (0x8C0100A4..0x8C01020E incl. pools).
   Bodies from MAT3 timer.c; DC differences read from the target listing (calib/_ghidra_units.txt):
   - a retrace callback increments a counter global (0x8C12253C) in place of sc.frameCount;
   - InitializeTimer brackets itself with an interrupt-mask call (arg 15, restore), registers the
     callback once with (0x900, callback, 0x58, 0), and multiplies gRetraceTime by 1.0f;
   - data order: D section gTimerPaused, gRetraceRate, gRetraceTime, sInited (so sInited = 0);
     B section gFrameCounter, two unknown words, gSeconds, gFrameSeconds, gLastFrameCount,
     gCountdownEnd, gCountdownStart, gLastFrameCounter. External function names are placeholders. */
extern int SetInterruptMask(int level);
extern void RegisterRetraceCallback(int kind, void (*fn)(void), int a, int b);

static int gTimerPaused = 0;
float gRetraceRate = 60.0f;
float gRetraceTime = 0.016666668f;

volatile unsigned int gFrameCounter;
int gTimerUnknownA;
int gTimerUnknownB;
volatile float gSeconds;
volatile float gFrameSeconds;
volatile int gLastFrameCount;
static unsigned int gCountdownEnd;
static unsigned int gCountdownStart;
static volatile unsigned int gLastFrameCounter;

void RetraceCallback(void);
void InitializeTimer(void);
void PauseTimer(void);
void ResumeTimer(void);
void UpdateFrameTimer(void);
void SetCountdownTimer(float sec);
void AddCountdownTime(float sec);
float GetCountdownTimeRemaining(void);
int CountdownTimeOut(void);

void RetraceCallback(void)
{
    gFrameCounter++;
}

void InitializeTimer(void)
{
    static int sInited = 0;
    int mask;

    mask = SetInterruptMask(15);
    if (!sInited) {
        gFrameCounter = 0;
        RegisterRetraceCallback(0x900, RetraceCallback, 0x58, 0);
        sInited = 1;
    }
    gLastFrameCounter = gFrameCounter;
    gSeconds = gFrameSeconds = 1.0f * gRetraceTime;
    SetInterruptMask(mask);
}

void PauseTimer(void)
{
    gTimerPaused = 1;
}

void ResumeTimer(void)
{
    gTimerPaused = 0;
}

void UpdateFrameTimer(void)
{
    gLastFrameCount = gFrameCounter - gLastFrameCounter;
    gLastFrameCounter = gFrameCounter;
    if (gTimerPaused) {
        gCountdownStart += gLastFrameCount;
        gCountdownEnd += gLastFrameCount;
    } else {
        gSeconds = gRetraceTime * (float)(int)(gFrameCounter - gCountdownStart);
    }
    if (gLastFrameCount > 5)
        gLastFrameCount = 5;
    gFrameSeconds = (float)gLastFrameCount * gRetraceTime;
}

void SetCountdownTimer(float sec)
{
    gCountdownStart = gFrameCounter;
    gCountdownEnd = gFrameCounter + (int)(sec * gRetraceRate);
}

void AddCountdownTime(float sec)
{
    gCountdownEnd += (int)(sec * gRetraceRate);
}

float GetCountdownTimeRemaining(void)
{
    return gRetraceTime * (float)(int)(gCountdownEnd - gFrameCounter);
}

int CountdownTimeOut(void)
{
    return (int)(gCountdownEnd - gFrameCounter) <= 0;
}
