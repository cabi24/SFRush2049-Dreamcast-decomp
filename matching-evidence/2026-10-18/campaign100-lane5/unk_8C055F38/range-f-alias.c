#include "unk_8C055F38_types.h"

#define YMAX (6746520128.0f)
#define TWOOPI (0.63661975413560867f)
#define C1 (1.5708007961511612f)
#define C2 (-4.4544549950842338e-06f)
#define EPS (2.3300001164572315e-10f)
#define P1 (-0.1333834994584322f)
#define P2 (0.0034248878073412925f)
#define P3 (-1.7861706737676286e-05f)
#define Q1 (-0.46671682968735695f)
#define Q2 (0.025663832435384393f)
#define Q3 (-0.00031181532904156484f)
#define Q4 (4.9819437464293514e-07f)

/* MAT3 tan.c semantic match; anonymous map identity retained for root review. */
float func_8C055F38(float x)
{
    float g; /* f1 */ /* declared before f: the other order swaps their registers */
    float f; /* f8 */
    float xn; /* f0 */
    float xnum; /* f3 */
    float xden; /* f1 */
    float temp; /* f4 */
    float tt1; /* f0 */

    f = x;
    if (_builtin_fabsf(f) > YMAX) {
        return 0.0f;
    }
    temp = x * TWOOPI;
    temp = modff(temp, &xn);
    if (_builtin_fabsf(temp) >= 0.5) {
        xn += (x < 0.0f) ? -1.0f : 1.0f;
    }
    temp = modff(x, &tt1);
    f = ((tt1 - xn * C1) + temp) - xn * C2;
    if (_builtin_fabsf(f) < EPS) {
        xnum = f;
        xden = 1.0f;
    } else {
        g = f * f;
        xnum = f * (g * ((P3 * g + P2) * g + P1)) + f;
        xden = (((Q4 * g + Q3) * g + Q2) * g + Q1) * g + 1.0f;
    }
    if ((int)xn & 1) {
        return xden / -xnum;
    }
    return xnum / xden;
}
