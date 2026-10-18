#include "dc_types.h"
#ifndef DC_STREAKS_TYPES_H
#define DC_STREAKS_TYPES_H



typedef struct tStreakLL {
    void *next; /* +0: 8C098E8A and 8C099292 */
    char pad0[4];
} tStreakLL;

/* +0x10 head: pool 8C098F4C = list+0x10; +0x18 extent from
   list/array pairs at pools 8C098F38..3C and 8C098F44..48.
   Remaining named fields agree with the independently matched statll unit. */




/* sizeof 0x40 passed at 8C098E5C. */
typedef struct tStreak {
    tStreakLL ll;
    unsigned int flags; /* +8: 8C098F86, 8C09928C */
    float delay;        /* +c: 8C099184..186, 8C0992A8..2BA */
    float newpos[3];    /* +10..18: 8C0991EA..1FE and 8C09909C..0B4 */
    float lastpos[3];   /* +1c..24: 8C099200..210, 8C099132..14A */
    float backVerts[2][3]; /* +28..3c: 8C099066..09A and 8C099212..244 */
} tStreak;

/* sizeof 0x1c passed at 8C098E6C. */
typedef struct tStreakPoly {
    tStreakLL ll;
    tPoly *poly;        /* +8: 8C098E9E, 8C0990FC */
    float lifeTime;     /* +c: 8C098E88..98 */
    float incTime;      /* +10: 8C098EC8..ED6 */
    tRGBA clr;          /* +14: 8C098F06; alpha+17: 8C098EF0..F12 */
    tStreak *s;         /* +18: 8C098EAC, 8C09906E */
} tStreakPoly;

typedef char dc_check_statlist[(sizeof(tStatList) == 0x18) ? 1 : -1];
typedef char dc_check_streak[(sizeof(tStreak) == 0x40) ? 1 : -1];
typedef char dc_check_streakpoly[(sizeof(tStreakPoly) == 0x1c) ? 1 : -1];

#endif
