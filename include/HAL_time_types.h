#ifndef HAL_TIME_TYPES_H
#define HAL_TIME_TYPES_H
#define NULL ((void *)0)
/* TargetFD04/FDB4 andFD0A/FDB8 use32-bit r0 value/result. */
unsigned int dcReadTick(void);
unsigned int dcConvertTick(unsigned int tick);
#endif
