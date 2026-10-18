/* drivetra.c partial unit: enginetorque, interp, engine (0x8C0230B0..0x8C0232F8 incl. pools).
   The preceding functions flush their pool at 0x8C0230A0, so this tail is a faithful placement test.
   tModelData overlay from the target listing: autotrans +0x0A, handling +0x0B, engine_type +0x0C,
   tune +0x04 (torque_scale +0x08), throttle +0x3D0, gear +0x3F4 (short), engtorque +0x404,
   engangvel +0x408, torquecurve +0x40C, roadcode +0x60C (int[4]), crashflag +0x640 (char). */
typedef struct tTune {
    char pad[8];
    float torque_scale;
} tTune;

typedef struct tModelData {
    char pad0[4];
    tTune *tune;
    char pad1[2];
    char autotrans;
    char handling;
    char engine_type;
    char pad2[0x3d0 - 0x0d];
    float throttle;
    char pad3[0x3f4 - 0x3d4];
    short gear;
    char pad4[0x404 - 0x3f6];
    float engtorque;
    float engangvel;
    const signed short *torquecurve;
    char pad5[0x60c - 0x410];
    int roadcode[4];
    char pad6[0x640 - 0x61c];
    char crashflag;
} tModelData;

extern float carEngineTorque[][3];

#pragma inline(interp)
signed short interp(int a, int b, int rem, int total);
signed short enginetorque(tModelData *m, signed short rpm, signed short throttle, const signed short *torquecurve);
void engine(tModelData *m);

signed short enginetorque(tModelData *m, signed short rpm, signed short throttle, const signed short *torquecurve)
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
    tindex = throttle / (signed short)14;
    trem = throttle % (signed short)14;
    if (tindex >= 9) {
        tindex = 8;
        trem = 13;
    }
    low_ptr = torquecurve + tindex * 12 + rindex;
    left = interp(low_ptr[0], low_ptr[1], rrem, 1149);
    low_ptr = torquecurve + (tindex + 1) * 12 + rindex;
    right = interp(low_ptr[0], low_ptr[1], rrem, 1149);
    left = interp(left, right, trem, 14);
    left *= m->tune->torque_scale * carEngineTorque[m->engine_type][m->handling];
    return left;
}

signed short interp(int a, int b, int rem, int total)
{
    return a + rem * (b - a) / total;
}

void engine(tModelData *m)
{
    if (m->crashflag) {
        m->engtorque = enginetorque(m, 9.5493045f * m->engangvel, 0, m->torquecurve);
    } else if (m->gear < 0) {
        m->engtorque = enginetorque(m, 9.5493045f * m->engangvel, 128.0f * m->throttle, m->torquecurve);
    } else {
        m->engtorque = enginetorque(m, 9.5493045f * m->engangvel, 128.0f * m->throttle, m->torquecurve);
    }
    if ((m->roadcode[2] == 1 || m->roadcode[2] == 2) && (m->roadcode[3] == 1 || m->roadcode[3] == 2)) {
        m->engtorque *= 1.5f;
    }
    if (m->autotrans && m->gear != 4) {
        m->engtorque *= 0.925f;
    }
}
