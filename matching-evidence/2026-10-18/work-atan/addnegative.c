/* atan.c - target unit 8C0113C0..8C011530, Unix V7 libm algorithm.
 * Private Dreamcast candidate based on MAT3 semantics and target pool words.
 */
#line 1 "atan_private.h"
/* Self-contained C89 declarations. No Dreamcast struct overlays required.
 * Target bsr/bra call graph, 8C01140A..8C0114F0, proves all callees are local.
 */
#ifndef ATAN_PRIVATE_H
#define ATAN_PRIVATE_H
float fsatan(float arg);
float fatan2(float arg_1, float arg_2);
float fatan(float arg);
#endif
#line 5 "atan.c"

#define SQ2P1 2.4142137f
#define SQ2M1 0.41421357f
#define PIO2  1.5707964f
#define PIO4  0.7853982f
#define PI    3.1415928f /* 8C011528 = 40490FDB; SHC truncates literals. */
#define P4 16.153641f   /* 8C0114F8 = 41813AA8 */
#define P3 268.42548f
#define P2 1153.0293f
#define P1 1780.4064f
#define P0 896.78595f
#define Q4 58.956971f   /* 8C011504 = 426BD3F0 */
#define Q3 536.2654f
#define Q2 1666.7839f   /* 8C01150C = 44D05915 */
#define Q1 2079.335f
#define Q0 896.78595f

static float fxatan(float arg);

static float fxatan(float arg)
{
    float argsq;
    float value;

    argsq = arg * arg;
    value = ((((P4 * argsq + P3) * argsq + P2) * argsq + P1) * argsq + P0);
    value = value / (((((argsq + Q4) * argsq + Q3) * argsq + Q2) * argsq + Q1) * argsq + Q0);
    return value * arg;
}

float fsatan(float arg)
{
    if (arg < SQ2M1)
        return fxatan(arg);
    else if (arg > SQ2P1)
        return PIO2 - fxatan(1.0f / arg);
    else
        return PIO4 + fxatan((arg + -1.0f) / (arg + 1.0f));
}

float fatan2(float arg_1, float arg_2)
{
    if (arg_1 + arg_2 == arg_1) {
        if (arg_1 == 0.0f)
            return 0.0f;
        else if (arg_1 > 0.0f)
            return PIO2;
        else
            return -PIO2;
    } else if (arg_2 < 0.0f) {
        if (arg_1 >= 0.0f)
            return PI - fsatan(-arg_1 / arg_2);
        else
            return -PI + fsatan(arg_1 / arg_2);
    } else if (arg_1 > 0.0f) {
        return fatan(arg_1 / arg_2);
    } else {
        return -fatan(-arg_1 / arg_2);
    }
}

float fatan(float arg)
{
    if (arg > 0.0f)
        return fsatan(arg);
    else
        return -fsatan(-arg);
}