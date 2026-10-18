#ifndef MAIN_HELPERS_TYPES_H
#define MAIN_HELPERS_TYPES_H
extern volatile unsigned int gFrameCounter; /* Timer IRQ increments12253C. */
extern signed char gSoftReset; /* pool03A404 and controlsys byte12C200. */
extern signed char gGameStarted; /* pool03A408 byte158340. */
extern void func_8C06B976(void);
extern int func_8C06BA20(void);
extern void func_8C02E350(void); /* StartGraphics role, target pool03A414. */
extern void func_8C02E390(void); /* EndGraphics role, target pool03A410. */
extern void func_8C039718(void); /* ExitSystem role, target pool03A40C. */
extern int GameLoop(void); /* target pool03A418. */
#endif
