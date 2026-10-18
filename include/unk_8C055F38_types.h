#ifndef UNK_8C055F38_TYPES_H
#define UNK_8C055F38_TYPES_H
/* Target call555A..62 and5588..8C: FR4 float,R4 pointer; resultFR0. */
float modff(float,float *);
/* FABS is directly present at55F44/66/FB4; native single-FP builtin, no library double ABI. */
float _builtin_fabsf(float);
#endif
