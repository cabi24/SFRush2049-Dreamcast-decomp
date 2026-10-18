/* timer.c: UpdateFrameTimer (held-out slot). DC keeps its own retrace counter (incremented by a
   registered callback) where MAT3 reads sc.frameCount; otherwise the MAT3 body. Shape 1: MAT3's inferred volatile qualifiers (evidence: target re-derives each global's address per access). Globals are
   externs here: only the instruction pattern is compared (pool displacements masked). */
extern float gRetraceTime;
extern volatile float gSeconds;
extern volatile float gFrameSeconds;
extern volatile int gLastFrameCount;
extern unsigned int gCountdownEnd;
extern unsigned int gCountdownStart;
extern volatile unsigned int gLastFrameCounter;
extern int gTimerPaused;
extern volatile unsigned int gFrameCounter;

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
