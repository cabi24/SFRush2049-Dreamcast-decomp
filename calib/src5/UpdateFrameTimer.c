/* timer.c: UpdateFrameTimer (held-out slot), shape 2: MAT3 file-scope declarations verbatim
   (static/volatile as in MAT3 timer.c), with the DC retrace counter in place of sc.frameCount. */
float gRetraceRate = 60.0f;
float gRetraceTime = 0.016666668f;

volatile float gSeconds;
volatile float gFrameSeconds;
volatile int gLastFrameCount;
static unsigned int gCountdownEnd;
static unsigned int gCountdownStart;
static volatile unsigned int gLastFrameCounter;
static int gTimerPaused = 0;
volatile unsigned int gFrameCounter;

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
