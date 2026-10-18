/* shape 3: pointer-arithmetic argument form. drivetra.c: enginetorque (held-out slot) with interp. DC layout from the target:
   m->tune at +4 (tune->torque_scale at +8), m->handling at +11, m->engine_type at +12,
   carEngineTorque[engine][3] floats, torque curve rows of 12 shorts.
   The target inlines interp three times yet keeps a standalone body, and automatic inlining is
   off for this build (ll.c evidence), so interp is requested inline explicitly. */
typedef struct tTune {
    char pad[8];
    float torque_scale;
} tTune;

typedef struct tModelData {
    char pad0[4];
    tTune *tune;
    char pad1[3];
    char handling;
    char engine_type;
} tModelData;

extern float carEngineTorque[][3];

#pragma inline(interp)
signed short interp(int a, int b, int rem, int total);

signed short enginetorque(tModelData *m, signed short rpm, unsigned int throttle, const signed short *torquecurve)
{
    signed short rindex;
    signed short tindex;
    signed short rrem;
    signed short trem;
    signed short left;
    signed short right;
    const signed short *low_ptr;

    rindex = rpm / 1150;
    rrem = rpm % 1150;
    if (rindex < 0) {
        rindex = 0;
        rrem = 0;
    }
    if (rindex >= 11) {
        rindex = 10;
        rrem = 1149;
    }
    tindex = throttle / 14;
    trem = throttle % 14;
    if (tindex >= 9) {
        tindex = 8;
        trem = 13;
    }
    low_ptr = torquecurve + tindex * 12 + rindex;
    left = interp(*low_ptr, *(low_ptr + 1), rrem, 1149);
    low_ptr = torquecurve + (tindex + 1) * 12 + rindex;
    right = interp(*low_ptr, *(low_ptr + 1), rrem, 1149);
    left = interp(left, right, trem, 14);
    left *= m->tune->torque_scale * carEngineTorque[m->engine_type][m->handling];
    return left;
}

signed short interp(int a, int b, int rem, int total)
{
    return a + rem * (b - a) / total;
}
