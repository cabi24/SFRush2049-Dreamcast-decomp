/* asincos.c -- Cody & Waite single-precision inverse circular functions.
 * MAT3 semantic port; Dreamcast instruction and literal evidence 8C011290..13C0.
 */
#include "..\include\asincos_private.h"

/* SHC decimal conversion truncates; these spellings select target float words. */
#define PIO2 1.5707964301f
#define PIO4 0.78539821506f
#define EPS 2.3000001209e-10f
#define DOMAIN_MAX 1.0000100732f
#define P1 -27.368494987f
#define P2 57.208230972f
#define P3 -39.688863754f
#define P4 10.152522564f
#define P5 -0.69674578309f
#define Q0 -164.21097565f
#define Q1 417.14433289f
#define Q2 -381.86305237f
#define Q3 150.95272064f
#define Q4 -23.823859215f

float farcsine(float x, int flag)
{
    static const float a[2] = { 0.0f, PIO4 };
    static const float b[2] = { PIO2, PIO4 };
    register float y;
    register float g;
    float r;
    int i;

    y = fabs(x);
    i = flag;
    if (y < EPS) {
        r = y;
    } else if (y >= 1.0f) {
        /* The target preserves this comparison with no following error write. */
        if (y < DOMAIN_MAX) {
        }
        r = PIO2;
    } else {
        if (y > 0.5f) {
            i = 1 - i;
            g = 1.0f - y;
            g /= 2.0f;
            y = sqrtf(g);
            y = -y - y;
        } else {
            g = y * y;
        }
        r = y + y * ((g * ((((P5 * g + P4) * g + P3) * g + P2) * g + P1))
                     / (((((g + Q4) * g + Q3) * g + Q2) * g + Q1) * g + Q0));
    }
    if (flag) {
        if (x < 0.0f)
            r = (b[i] + r) + b[i];
        else
            r = (a[i] - r) + a[i];
    } else {
        r = (a[i] + r) + a[i];
        if (x < 0.0f)
            r = -r;
    }
    return r;
}

float fasin(float x)
{
    return farcsine(x, 0);
}

float facos(float x)
{
    return farcsine(x, 1);
}
