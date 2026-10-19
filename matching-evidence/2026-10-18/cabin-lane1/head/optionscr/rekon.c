#include "rekon_types.h"
extern tRekonModel model[];
extern tRekonCar gGameCar[];
extern void CopyUV(float [3][3],float [3][3]);
extern void RotateUV(float *,float [3][3]);
void dead_reckon_posuv(tRekonModel *);

void dead_reckon_all(void)
{
    int i;
    tRekonModel *m;
    for (i = 0; i < 6; i++) {
        m = &model[i];
        if (m->in_game && gGameCar[i].dead < 2)
            dead_reckon_posuv(m);
    }
}

void game_reckon_all(float time)
{
    int i;
    float deltime;
    float temp[3];
    tRekonModel *m;
    for (i = 0; i < 6; i++) {
        m = &model[i];
        if (m->in_game && gGameCar[i].dead < 2) {
            deltime = (time - m->fastin.modelTime) * m->time_fudge;
            temp[0] = m->rwv[0] * deltime;
            temp[1] = m->rwv[1] * deltime;
            temp[2] = m->rwv[2] * deltime;
            m->reckon.rwr[0] = m->rwr[0] + temp[0];
            m->reckon.rwr[1] = m->rwr[1] + temp[1];
            m->reckon.rwr[2] = m->rwr[2] + temp[2];
            CopyUV(m->uvs, m->reckon.uvs);
            temp[0] = m->w[0] * deltime;
            temp[1] = m->w[1] * deltime;
            temp[2] = m->w[2] * deltime;
            RotateUV(temp, m->reckon.uvs);
        }
    }
}

void dead_reckon_posuv(tRekonModel *m)
{
    float temp[3];
    int i;
    CopyUV(m->uvs, m->reckon.uvs);
    m->reckon.rwmph = 2.7272728f * (0.5f * (m->tires[2].angvel * m->tires[2].tradius
                                          + m->tires[3].angvel * m->tires[3].tradius));
    for (i = 0; i < 4; i++) {
        m->reckon.suscomp[i] = m->suscomp[i];
        m->reckon.airdist[i] = m->airdist[i];
    }
    m->reckon.rwa[0] = m->reckon.base_rwa[0];
    m->reckon.rwa[1] = m->reckon.base_rwa[1];
    m->reckon.rwa[2] = m->reckon.base_rwa[2];
    m->reckon.rwv[0] = m->reckon.base_rwv[0];
    m->reckon.rwv[1] = m->reckon.base_rwv[1];
    m->reckon.rwv[2] = m->reckon.base_rwv[2];
    m->reckon.rwr[0] = m->reckon.base_rwr[0];
    m->reckon.rwr[1] = m->reckon.base_rwr[1];
    m->reckon.rwr[2] = m->reckon.base_rwr[2];
}

void update_reckon_base(int slot)
{
    tRekonModel *m = &model[slot];
    m->reckon.base_rwa[0] = m->rwa[0];
    m->reckon.base_rwa[1] = m->rwa[1];
    m->reckon.base_rwa[2] = m->rwa[2];
    m->reckon.base_rwv[0] = m->rwv[0] * m->time_fudge;
    m->reckon.base_rwv[1] = m->rwv[1] * m->time_fudge;
    m->reckon.base_rwv[2] = m->rwv[2] * m->time_fudge;
    m->reckon.base_rwr[0] = m->rwr[0];
    m->reckon.base_rwr[1] = m->rwr[1];
    m->reckon.base_rwr[2] = m->rwr[2];
}
