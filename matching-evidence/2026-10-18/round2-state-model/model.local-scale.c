/* Private model.c port. Backend calls and layouts follow DC target evidence. */
#include "model_types.h"
extern volatile unsigned short modelrun;
extern signed char gUseCatchup;
extern signed short gNumPlayers;
extern signed char gDifficulty;
extern tModelData model[6];
#define gGameCar ((tCarData *)0x8C004000)
extern tLink gLink[6];
void dcModelService(int value);
void init_model_task(int frames);
int ControllerUpdate(void);
void model_iteration(void);
void PeriodicMaintenance(void);
void dcAudioMain(void);

static void no_catchup(void);

/* Probably a helper or macro in the original: a plain `lagdist * 0.100000002f` puts the constant first. */
#define scalef(a,b) ((a)*(b))

/* 0x8002ca78, size 0x64 */
void ModelThread(void)
{
    static signed char sInited = 0;

    dcModelService(0x400);
    if (!sInited) {
        sInited = 1;
        modelrun = 0;
        init_model_task(0);
    }
    if (ControllerUpdate() && modelrun) {
        model_iteration();
    }
    PeriodicMaintenance();
    dcAudioMain();
}

/* 0x8002cadc, size 0xcc */
static void no_catchup(void)
{
    int i; /* r0 */
    for (i = 0; i < 6; i++) {
        if (gLink[i].owner == 0 || gLink[i].owner == 6) {
            model[i].catchup = 1.0f;
        }
    }
}

/* 0x8002cba8, size 0x1c8 */
void SetCatchup(void)
{
    signed short i; /* r10 */
    signed short high_index; /* r11 */
    float firstdist; /* f3 */
    float lagdist; /* f4 */
    float target_catchup; /* f4 */
    if ((gNumPlayers == 1 && gDifficulty < 5) || !gUseCatchup) {
        no_catchup();
        return;
    }
    {
    float catchup_scale = 0.100000002f;
    high_index = -1;
    for (i = 0; i < 6; i++) {
        if (model[i].in_game && gGameCar[i].dead < 2 && (model[i].drone_type == 2 || gDifficulty == 5)) {
            if (high_index == -1) {
                high_index = i;
            } else if (gGameCar[i].distance > gGameCar[high_index].distance) {
                high_index = i;
            }
        }
    }
    firstdist = gGameCar[high_index].distance;
    for (i = 0; i < 6; i++) {
        if (model[i].in_game && gGameCar[i].dead < 2 && (model[i].drone_type == 2 || gDifficulty == 5)) {
            lagdist = firstdist - gGameCar[i].distance;
            if (lagdist > 1500.0f) {
                target_catchup = 1.10000003f;
            } else {
                target_catchup = 1.0f + scalef(lagdist, catchup_scale) / 1500.0f;
            }
            if (gUseCatchup == 1) {
                target_catchup = 1.0f + 0.5f * (1.0f - target_catchup);
            }
            model[i].catchup = 0.03f * target_catchup + 0.97000003f * model[i].catchup;
        }
    }
}
}
