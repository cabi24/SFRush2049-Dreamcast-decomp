#include "dc_types.h"
extern tCarLoadLink gLink[13];
extern tCarLoadDroneSlice model_drone[13];
extern unsigned int gGameState;
extern signed char gCarPaintJob[5][13];
extern unsigned int gTextureRamUsed;
extern unsigned int gWaveSizes[315];
extern signed short gNumPlayers;
extern signed short gCarModelNums[52];
extern int gCarPaintJobWaveNums[13];

void InitSelectCarLoad(void);
float Random(float max);
int FindWaveInstance(int waveNum);
int LoadWave(int waveNum, int loadAgain, int unused, int handle, void *heap);
void SetupCarColor(signed short slot, signed short car, unsigned char PaintJob);


/* carload.c - chooses and loads the race cars, falling back when texture memory runs out.
   Dreamcast LoadCars: 0x8C01B6F0..0x8C01BADA; unit ends 0x8C01BAF4.
   Semantic reference: MAT3 GameSource/src/carload.c. Private layout slice; native real-address match recorded in evidence/receipt.json. */

void LoadCars(int select)
{
    int i; /* r25 */
    int j; /* r0 */
    int numLoadedCars; /* r24 */
    if (select) {
        InitSelectCarLoad();
        return;
    }
    numLoadedCars = 0;
    for (i = 0; i < 13; i++) {
        if (i < 6 && ((gGameState & 8) || model_drone[i].drone_type == 1)) {
        again:
            gLink[i].car = Random(13.0f);
            for (j = 0; j < i; j++) {
                if (gLink[i].car == gLink[j].car) {
                    goto again;
                }
            }
            gLink[i].PaintJob = gCarPaintJob[0][gLink[i].car];
            if (0x440000 - gTextureRamUsed < gWaveSizes[(unsigned int)gLink[i].car + 0x40] + 0x200) {
                for (j = 0; j < 13; j++) {
                    if (FindWaveInstance(gLink[i].car + 0x40) < 0 &&
                        0x440000 - gTextureRamUsed >= gWaveSizes[(unsigned int)gLink[i].car + 0x40] + 0x200) {
                        goto load;
                    }
                    gLink[i].car = (gLink[i].car + 1) % 13;
                }
                for (j = 0; j < 13; j++) {
                    if (0x440000 - gTextureRamUsed >= gWaveSizes[(unsigned int)gLink[i].car + 0x40] + 0x200) {
                        goto load;
                    }
                    gLink[i].car = (gLink[i].car + 1) % 13;
                }
                do {
                    j = Random(numLoadedCars);
                } while (numLoadedCars > gNumPlayers && gLink[j].car >= 13);
                gLink[i].car = gLink[j].car;
                gCarModelNums[i] = gCarModelNums[j];
            } else {
            load:
                gCarModelNums[i] = LoadWave(gLink[i].car + 0x40, 1, 0, 0, 0);
                numLoadedCars++;
            }
            LoadWave(gCarPaintJobWaveNums[gLink[i].car] + gLink[i].PaintJob, 0, 0, 0, 0);
            SetupCarColor(i, gLink[i].car, gLink[i].PaintJob);
        } else if (i < 6 && model_drone[i].drone_type == 2) {
            if (0x440000 - gTextureRamUsed < gWaveSizes[(unsigned int)gLink[i].car + 0x40] + 0x200) {
                for (gLink[i].car = 0; gLink[i].car < 13; gLink[i].car++) {
                    if (0x440000 - gTextureRamUsed >= gWaveSizes[(unsigned int)gLink[i].car + 0x40] + 0x200) {
                        goto load2;
                    }
                }
                gLink[i].car = gLink[0].car;
                gCarModelNums[i] = gCarModelNums[0];
            } else {
            load2:
                gCarModelNums[i] = LoadWave(gLink[i].car + 0x40, 1, 0, 0, 0);
                LoadWave(gLink[i].PaintJob + gCarPaintJobWaveNums[gLink[i].car], 0, 0, 0, 0);
                SetupCarColor(i, gLink[i].car, gLink[i].PaintJob);
                numLoadedCars++;
            }
        } else {
            gLink[i].car = gLink[0].car;
            gCarModelNums[i] = gCarModelNums[0];
        }
    }
}

