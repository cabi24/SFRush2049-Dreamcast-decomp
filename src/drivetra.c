/* drivetra.c - whole translation unit, Dreamcast target 0x8C022CA4..0x8C0232F8 (incl. pools).
   Bodies from MAT3 GameSource/src/drivetra.c; DC layouts from include/dc_types.h.
   Frozen configuration: SHC 5.1r11, base flags + -speed -noinline (interp requested inline). */
#include "C:\rush2049-dreamcast\include\dc_types.h"

extern float carEngineTorque[][3];    /* 0x8C0BBC4C, pool word 8C023238 */
extern float carTransGearRatio[][6];  /* 0x8C0BBA48, pool words 8C022FE8, 8C0230AC; row stride 24 */

void drivetrain(tModelData *m);
void whatslips(tModelData *m);
void autoshift(tModelData *m);
void transmission(tModelData *m);
void find_best_gear(tModelData *m, float usang, float dsang);
void upshift(tModelData *m);
void downshift(tModelData *m);
#pragma inline(interp)
signed short interp(int a, int b, int rem, int total);
signed short enginetorque(tModelData *m, signed short rpm, signed short throttle, const signed short *torquecurve);
void engine(tModelData *m);

void drivetrain(tModelData *m)
{
    float rearload;

    if (m->autotrans) {
        autoshift(m);
    }
    engine(m);
    transmission(m);
    m->dwangvel = 0.5f * (m->tires[2].angvel + m->tires[3].angvel);
    whatslips(m);
    rearload = m->tireForce[2][1] + m->tireForce[3][1];
    if (!m->magicdiff || rearload < 500.0f) {
        m->torque[2] += 0.5f * m->dwtorque;
        m->torque[3] += 0.5f * m->dwtorque;
    } else if (m->tireForce[2][1] <= 0.0f) {
        m->torque[2] = 0.0f;
        m->torque[3] += m->dwtorque;
    } else if (m->tireForce[3][1] <= 0.0f) {
        m->torque[2] += m->dwtorque;
        m->torque[3] = 0.0f;
    } else {
        m->torque[2] += m->dwtorque * m->tireForce[2][1] / rearload;
        m->torque[3] += m->dwtorque * m->tireForce[3][1] / rearload;
    }
    m->tires[2].invmi = 2.0f * m->efdwinvmi;
    m->tires[3].invmi = 2.0f * m->efdwinvmi;
}

void whatslips(tModelData *m)
{
    float totratsq;
    float curclmaxt;
    float dv;
    int foo;

    totratsq = m->totalratio * m->totalratio;
    if (m->engangvel < 104.719666f) {
        m->engangvel = 104.719666f;
    }
    /* 0.80000002f: SHC truncates literals; target word 8C022DE4 = 0x3F4CCCCD (0.8 rounded) */
    if (m->clutch > 0.80000002f) {
        curclmaxt = 0.0f;
    } else {
        curclmaxt = 1.25f * (0.80000002f - m->clutch) * m->car->clutchmaxt;
    }
    if (m->gear == 0 || curclmaxt == 0.0f) {
        m->clutchtorque = 0.0f;
        m->dwtorque = 0.0f;
        m->clutchangvel = m->engangvel;
        /* left-linear product: SHC flattens a*(b*c) into (a*b)*c, target keeps engtorque*enginvmi first (8C022E28..34) */
        m->engangvel += m->engtorque * m->car->enginvmi * m->dt;
        m->efdwinvmi = m->dwinvmi;
        return;
    }
    curclmaxt *= m->tune->clutchmaxtk;
    m->clutchangvel = m->dwangvel * m->totalratio;
    foo = m->engangvel > m->clutchangvel;
    m->clutchtorque = foo ? curclmaxt : -curclmaxt;
    /* target 8C022E70..88: (engtorque - clutchtorque) * enginvmi, then * dt */
    dv = (m->engtorque - m->clutchtorque) * m->car->enginvmi * m->dt;
    if (foo ^ (m->engangvel + dv > m->clutchangvel)) {
        m->engangvel = m->clutchangvel;
        m->clutchtorque = m->engtorque;
    } else {
        m->engangvel += dv;
    }
    m->efdwinvmi = 1.0f / (1.0f / m->dwinvmi + totratsq / m->car->enginvmi);
    m->dwtorque = m->clutchtorque * m->totalratio;
}

void autoshift(tModelData *m)
{
    float modupshiftangvel;
    float moddownshiftangvel;
    float fact;

    fact = 0.25f * (3.0f + m->throttle);
    /* target 8C022F2A..40: (shiftk * up) * fact and (shiftk * down) * fact, no shared fact*shiftk */
    modupshiftangvel = m->tune->shiftk * m->car->upshiftangvel * fact;
    moddownshiftangvel = m->tune->shiftk * m->car->downshiftangvel * fact;
    if (m->commandgear == 0 || m->commandgear == -1) {
        m->gear = m->commandgear;
        return;
    }
    if (m->gear == 0 || m->gear == -1) {
        find_best_gear(m, modupshiftangvel, moddownshiftangvel);
    }
    if (m->engangvel > modupshiftangvel) {
        upshift(m);
    }
    if (m->engangvel < moddownshiftangvel) {
        downshift(m);
    }
}

void transmission(tModelData *m)
{
    m->transratio = m->tune->trans_scale * carTransGearRatio[m->trans][m->gear + 1];
    m->totalratio = m->transratio * m->car->dwratio;
}

void find_best_gear(tModelData *m, float usang, float dsang)
{
    float ratio;

    for (m->gear = 1; m->gear < 4; m->gear++) {
        ratio = m->tune->trans_scale * m->car->dwratio * carTransGearRatio[m->trans][m->gear + 1];
        ratio = m->dwangvel * ratio;  /* target 8C02302C fmov fr5,fr3 then dwangvel*ratio */
        if (ratio < usang) {
            break;  /* return gives the r2/r3 swap in the gear++ tail (8C023036..40) */
        }
    }
}

void upshift(tModelData *m)
{
    if (m->gear >= 4) {
        return;
    }
    if (m->fastin.modelTime > m->shifttime) {
        m->shifttime = 1.0f + m->fastin.modelTime;
        m->gear++;
    }
}

void downshift(tModelData *m)
{
    if (m->gear <= 1) {
        return;
    }
    if (m->fastin.modelTime > m->shifttime) {
        m->shifttime = 1.0f + m->fastin.modelTime;
        m->gear--;
    }
}

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
        m->engtorque *= 0.92500002f;  /* target word 8C0232F4 = 0x3F6CCCCD; 0.925f truncates to ...CCCC */
    }
}
