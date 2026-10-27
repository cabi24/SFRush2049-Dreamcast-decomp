/* DC audio: merged audio/audiodebug semantics, target-backed private layouts.
   Pause/resume, frame counter and resource placement follow DC evidence. */
#include "audio_types.h"
#define fabs(x) _builtin_fabsf(x)
#define sqrtf(x) _builtin_sqrtf(x)
anon_id_basePitchRpm_1c gEngineSpecs[9][2] = {
  {
    { 36, 2900, 4000, 3000, 4500, 6000, 1.0000000298023223876953125f, 0.80000002682209014892578125f, 0.0f },
    { 41, 5000, 5000, 3000, 4500, 6000, 0.250000007450580596923828125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f }
  },
  {
    { 39, 4000, 4000, 3000, 3000, 6000, 1.0000000298023223876953125f, 1.0000000298023223876953125f, 0.0f },
    { 38, 5000, 5000, 2000, 3500, 6000, 0.250000007450580596923828125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f }
  },
  {
    { 44, 3000, 3000, 3000, 4500, 6000, 1.0000000298023223876953125f, 1.0000000298023223876953125f, 0.0f },
    { 43, 5000, 5000, 2000, 5000, 5000, 0.250000007450580596923828125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f }
  },
  {
    { 34, 2500, 3000, 3000, 3000, 6000, 1.0000000298023223876953125f, 1.0000000298023223876953125f, 0.0f },
    { 33, 5000, 5000, 2000, 5000, 5000, 0.250000007450580596923828125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f }
  },
  {
    { 42, 5000, 5000, 1500, 3000, 5000, 1.0000000298023223876953125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f },
    { -1, 5000, 5000, 2000, 5000, 5000, 0.250000007450580596923828125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f }
  },
  {
    { 47, 2500, 3000, 2000, 3000, 6000, 0.50000001490116119384765625f, 0.75000001490116119384765625f, 0.0f },
    { 40, 5000, 5000, 900, 3000, 5000, 1.0000000298023223876953125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f }
  },
  {
    { 37, 7000, 6900, 5000, 5000, 5000, 1.0000000298023223876953125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f },
    { -1, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f }
  },
  {
    { 46, 3100, 3000, 3000, 3000, 6000, 1.0000000298023223876953125f, 1.0000000298023223876953125f, 0.0f },
    { 45, 4000, 5000, 2000, 5000, 5000, 0.60000003874301910400390625f, 1.0000000298023223876953125f, 1.0000000298023223876953125f }
  },
  {
    { 36, 3000, 3000, 3000, 3000, 6000, 1.0000000298023223876953125f, 1.0000000298023223876953125f, 0.0f },
    { 35, 5000, 5000, 2000, 5000, 5000, 0.250000007450580596923828125f, 1.0000000298023223876953125f, 1.0000000298023223876953125f }
  }
};

/* The original inline sound-state helpers shared this first .sdata2 value.
 * Keep one addressable float so the merged audio/audiodebug split retains it. */

static signed char gGameSoundsEnabled = 1;
static signed char gCarActive[6] = { 0, 0, 0, 0, 0, 0 };
static signed char gCarUpdatedLastTime[6] = { 0, 1, 0, 1, 0, 1 };
static int gWaterfall = { -1 };
static const signed short gBumpLRTab[16] = {
    0, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0
};
static const signed short gBumpFBTab[16] = {
    0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
struct AudioEmitter {
    /* Members */
    unsigned char numFx; /* offset 0x0, size 0x1 */
    int fxid; /* offset 0x4, size 0x4 */
    int tag; /* offset 0x8, size 0x4 */
    float pos[3]; /* offset 0xC, size 0xC */
    unsigned short radius; /* offset 0x18, size 0x2 */
    unsigned int nextTime; /* offset 0x1C, size 0x4 */
    float freqMin; /* offset 0x20, size 0x4 */
    float freqMax; /* offset 0x24, size 0x4 */
};
static struct AudioEmitter gStEmitters[6][20] = {
    3, 99, -1, 3638.00006103515625f, 450.00000762939453125f, -708.0000152587890625f, 0xc8, 0, 10.0000002384185791015625f, 30.000000476837158203125f, 3, 99, -1, 3722.00006103515625f, 450.00000762939453125f,
    -650.0000152587890625f, 0xc8, 0, 0.0f, -1.0000000298023223876953125f, 3, 99, -1, 3802.00006103515625f, 450.00000762939453125f, -592.0000152587890625f, 0xc8, 0, 0.0f, -1.0000000298023223876953125f, 3,
    99, -1, 3926.00006103515625f, 450.00000762939453125f, -504.00000762939453125f, 0xc8, 0, 0.0f, -1.0000000298023223876953125f, 3, 99, -1, 3300.00006103515625f, 450.00000762939453125f, -750.0000152587890625f,
    0xc8, 0, 0.0f, -1.0000000298023223876953125f, 3, 99, -1, 3155.00006103515625f, 450.00000762939453125f, -547.0000152587890625f, 0xc8, 0, 0.0f, -1.0000000298023223876953125f, 3, 99, -1,
    2967.00006103515625f, 450.00000762939453125f, -302.00000762939453125f, 0xc8, 0, 0.0f, -1.0000000298023223876953125f, 3, 99, -1, 5044.0001220703125f, 450.00000762939453125f, 116.0000019073486328125f, 0xc8, 0,
    0.0f, -1.0000000298023223876953125f, 20, 102, -1, 664.0000152587890625f, 530.0000152587890625f, 1622.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 693.0000152587890625f,
    530.0000152587890625f, 1482.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 858.0000152587890625f, 530.0000152587890625f, 1198.000030517578125f, 0x190, 0, 0.50000001490116119384765625f,
    5.00000011920928955078125f, 20, 102, -1, 1003.0000152587890625f, 530.0000152587890625f, 1910.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 1449.000030517578125f,
    530.0000152587890625f, 1759.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 2202.00006103515625f, 545.0000152587890625f, 1765.000030517578125f, 0x190, 0, 0.50000001490116119384765625f,
    5.00000011920928955078125f, 20, 102, -1, 1323.000030517578125f, 520.0000152587890625f, 1297.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 3217.00006103515625f,
    656.0000152587890625f, 1110.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 4724.0001220703125f, 573.0000152587890625f, 0.0f, 0x190, 0, 0.50000001490116119384765625f,
    5.00000011920928955078125f, 0, -1, -1, 0.0f, 0.0f, 0.0f, 0, 0, 0.0f, 0.0f, 0, -1, -1, 0.0f, 0.0f, 0.0f, 0, 0, 0.0f,
    0.0f, 0, -1, -1, 0.0f, 0.0f, 0.0f, 0, 0, 0.0f, 0.0f, 20, 102, -1, 1921.000030517578125f, 206.000003814697265625f, -2686.00006103515625f,
    0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 1797.000030517578125f, 200.000003814697265625f, -2876.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102,
    -1, 2387.00006103515625f, 173.000003814697265625f, -2762.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 3154.00006103515625f, 173.000003814697265625f, -2724.00006103515625f,
    0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 1653.000030517578125f, 182.000003814697265625f, -2475.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102,
    -1, 1523.000030517578125f, 207.000003814697265625f, -2473.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 1558.000030517578125f, 219.000003814697265625f, -2248.00006103515625f,
    0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 985.0000152587890625f, 191.000003814697265625f, -2273.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1,
    -206.000003814697265625f, 235.000003814697265625f, -2114.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -634.0000152587890625f, 174.000003814697265625f, -2165.00006103515625f, 0x190,
    0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -1755.000030517578125f, -17.000000476837158203125f, -3463.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1,
    -1676.000030517578125f, 9.0000002384185791015625f, -3711.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -1806.000030517578125f, 5.00000011920928955078125f, -3852.00006103515625f, 0x190,
    0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -1755.000030517578125f, 16.000000476837158203125f, -3968.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1,
    -1621.000030517578125f, 43.00000095367431640625f, -4223.0001220703125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -1902.000030517578125f, 19.000000476837158203125f, -4335.0001220703125f, 0x190,
    0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -1759.000030517578125f, -6.00000011920928955078125f, -4618.0001220703125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1,
    -1533.000030517578125f, -6.00000011920928955078125f, -5175.0001220703125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -1383.000030517578125f, -5.00000011920928955078125f, -5601.0001220703125f, 0x190,
    0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 3171.00006103515625f, 330.00000762939453125f, -3477.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1,
    94.0000019073486328125f, 230.000003814697265625f, 626.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 249.000003814697265625f, 230.000003814697265625f, 459.00000762939453125f, 0x190, 0,
    0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -263.00000762939453125f, 230.000003814697265625f, 481.00000762939453125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -155.000003814697265625f,
    250.000003814697265625f, 740.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -267.00000762939453125f, 250.000003814697265625f, 1486.000030517578125f, 0x190, 0, 0.50000001490116119384765625f,
    5.00000011920928955078125f, 20, 102, -1, 518.0000152587890625f, 250.000003814697265625f, 1134.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 754.0000152587890625f, 250.000003814697265625f,
    1441.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 565.0000152587890625f, 240.000003814697265625f, 1847.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20,
    102, -1, 2135.00006103515625f, 600.0000152587890625f, -515.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 2924.00006103515625f, 540.0000152587890625f, -357.00000762939453125f,
    0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 2882.00006103515625f, 420.00000762939453125f, 623.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1,
    3262.00006103515625f, 380.00000762939453125f, 565.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 1593.000030517578125f, 590.0000152587890625f, -2209.00006103515625f, 0x190,
    0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 1509.000030517578125f, 550.0000152587890625f, -1385.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1,
    2123.00006103515625f, 662.0000152587890625f, -1320.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 684.0000152587890625f, 424.00000762939453125f, -2552.00006103515625f, 0x190,
    0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 0, -1, -1, 0.0f, 0.0f, 0.0f, 0, 0, 0.0f, 0.0f, 0, -1, -1, 0.0f, 0.0f, 0.0f, 0,
    0, 0.0f, 0.0f, 0, -1, -1, 0.0f, 0.0f, 0.0f, 0, 0, 0.0f, 0.0f, 0, -1, -1, 0.0f, 0.0f, 0.0f, 0,
    0, 0.0f, 0.0f, 1, 125, -1, 5408.0001220703125f, 24.000000476837158203125f, 88.0000019073486328125f, 0x190, 0, 0.270000018179416656494140625f, 0.419999994337558746337890625f, 1, 126, -1, 5340.0001220703125f,
    24.000000476837158203125f, 106.0000019073486328125f, 0x190, 0, 1.2000000774860382080078125f, 1.5499999821186065673828125f, 1, 127, -1, 5377.0001220703125f, 0.0f, 198.000003814697265625f, 0x190, 0, 0.2000000067055225372314453125f, 0.60000003874301910400390625f,
    1, 128, -1, 5424.0001220703125f, 8.0000002384185791015625f, 149.000003814697265625f, 0x190, 0, 0.64999999105930328369140625f, 1.2000000774860382080078125f, 1, 129, -1, 5442.0001220703125f, 26.000000476837158203125f, 66.0000019073486328125f,
    0x190, 0, 0.300000019371509552001953125f, 0.89999999105930328369140625f, 1, 130, -1, 4958.0001220703125f, 207.000003814697265625f, -1386.000030517578125f, 0x190, 0, 0.60000003874301910400390625f, 1.0000000298023223876953125f, 1, 131, -1,
    4859.0001220703125f, 199.000003814697265625f, -1398.000030517578125f, 0x190, 0, 0.1500000096857547760009765625f, 0.2099999971687793731689453125f, 1, 132, -1, 4716.0001220703125f, 205.000003814697265625f, -1333.000030517578125f,
    0x190, 0, 0.2000000067055225372314453125f, 0.250000007450580596923828125f, 1, 133, -1, 4771.0001220703125f, 205.000003814697265625f, -1300.000030517578125f, 0x190, 0, 0.370000012218952178955078125f, 0.50000001490116119384765625f, 1, 134,
    -1, 4870.0001220703125f, 216.000003814697265625f, -1257.000030517578125f, 0x190, 0, 0.270000018179416656494140625f, 0.320000000298023223876953125f, 1, 125, -1, 5004.0001220703125f, 218.000003814697265625f, -1171.000030517578125f,
    0x190, 0, 0.270000018179416656494140625f, 0.419999994337558746337890625f, 1, 126, -1, 5026.0001220703125f, 208.000003814697265625f, -1263.000030517578125f, 0x190, 0, 1.2000000774860382080078125f, 1.5499999821186065673828125f, 1, 127,
    -1, 5405.0001220703125f, 134.000003814697265625f, -253.000003814697265625f, 0x190, 0, 0.2000000067055225372314453125f, 0.60000003874301910400390625f, 1, 128, -1, 5425.0001220703125f, 124.0000019073486328125f, -241.000003814697265625f,
    0x190, 0, 0.64999999105930328369140625f, 1.2000000774860382080078125f, 1, 129, -1, 5430.0001220703125f, 133.000003814697265625f, -310.00000762939453125f, 0x190, 0, 0.300000019371509552001953125f, 0.89999999105930328369140625f, 1, 130, -1,
    5980.0001220703125f, 193.000003814697265625f, -1361.000030517578125f, 0x190, 0, 0.60000003874301910400390625f, 1.0000000298023223876953125f, 1, 131, -1, 6012.0001220703125f, 183.000003814697265625f, -1408.000030517578125f, 0x190,
    0, 0.1500000096857547760009765625f, 0.2099999971687793731689453125f, 1, 132, -1, 4154.0001220703125f, 77.0000019073486328125f, -3579.00006103515625f, 0x190, 0, 0.2000000067055225372314453125f, 0.250000007450580596923828125f, 1, 133, -1,
    4252.0001220703125f, 50.00000095367431640625f, -3719.00006103515625f, 0x190, 0, 0.370000012218952178955078125f, 0.50000001490116119384765625f, 1, 134, -1, 4339.0001220703125f, 50.00000095367431640625f, -3654.00006103515625f, 0x190,
    0, 0.270000018179416656494140625f, 0.320000000298023223876953125f, 3, 99, -1, -312.00000762939453125f, 155.000003814697265625f, -1003.0000152587890625f, 0x190, 0, 10.0000002384185791015625f, 30.000000476837158203125f, 20, 102, -1,
    346.00000762939453125f, 36.00000095367431640625f, 1800.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 496.00000762939453125f, 66.0000019073486328125f, 1798.000030517578125f, 0x190, 0,
    0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 550.0000152587890625f, 66.0000019073486328125f, 1737.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 2809.00006103515625f,
    260.00000762939453125f, -205.000003814697265625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 3132.00006103515625f, 276.00000762939453125f, -550.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f,
    5.00000011920928955078125f, 20, 102, -1, 3022.00006103515625f, 277.00000762939453125f, -315.00000762939453125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 3052.00006103515625f,
    301.00000762939453125f, -969.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 3062.00006103515625f, 300.00000762939453125f, -1140.000030517578125f, 0x190, 0, 0.50000001490116119384765625f,
    5.00000011920928955078125f, 20, 102, -1, 3215.00006103515625f, 300.00000762939453125f, -1243.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 1690.000030517578125f,
    75.0000019073486328125f, -1663.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 1988.000030517578125f, 139.000003814697265625f, -1987.000030517578125f, 0x190, 0, 0.50000001490116119384765625f,
    5.00000011920928955078125f, 20, 102, -1, 1819.000030517578125f, 130.000003814697265625f, -2094.00006103515625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 2309.00006103515625f,
    179.000003814697265625f, -1916.000030517578125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -758.0000152587890625f, 62.00000095367431640625f, -1070.000030517578125f, 0x190, 0, 0.50000001490116119384765625f,
    5.00000011920928955078125f, 20, 102, -1, -741.0000152587890625f, 48.00000095367431640625f, -676.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -530.0000152587890625f, 87.0000019073486328125f,
    525.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, -719.0000152587890625f, 88.0000019073486328125f, 503.00000762939453125f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20,
    102, -1, -751.0000152587890625f, 88.0000019073486328125f, 644.0000152587890625f, 0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 20, 102, -1, 2602.00006103515625f, 101.0000019073486328125f, 569.0000152587890625f,
    0x190, 0, 0.50000001490116119384765625f, 5.00000011920928955078125f, 1, 125, -1, 300.00000762939453125f, 100.0000019073486328125f, 1639.000030517578125f, 0x190, 0, 0.270000018179416656494140625f, 0.419999994337558746337890625f, 1, 126, -1,
    302.00000762939453125f, 100.0000019073486328125f, 1577.000030517578125f, 0x190, 0, 1.2000000774860382080078125f, 1.5499999821186065673828125f, 1, 127, -1, 148.000003814697265625f, 56.00000095367431640625f, 1538.000030517578125f, 0x190, 0,
    0.2000000067055225372314453125f, 0.60000003874301910400390625f, 1, 128, -1, 307.00000762939453125f, 96.0000019073486328125f, 1771.000030517578125f, 0x190, 0, 0.64999999105930328369140625f, 1.2000000774860382080078125f, 1, 129, -1, 159.000003814697265625f,
    69.0000019073486328125f, 1895.000030517578125f, 0x190, 0, 0.300000019371509552001953125f, 0.89999999105930328369140625f, 1, 130, -1, 16.000000476837158203125f, 70.0000019073486328125f, 1670.000030517578125f, 0x190, 0, 0.60000003874301910400390625f, 1.0000000298023223876953125f,
    1, 131, -1, -71.0000019073486328125f, 68.0000019073486328125f, 1672.000030517578125f, 0x190, 0, 0.1500000096857547760009765625f, 0.2099999971687793731689453125f, 1, 132, -1, -72.0000019073486328125f, 68.0000019073486328125f, 1724.000030517578125f,
    0x190, 0, 0.2000000067055225372314453125f, 0.250000007450580596923828125f, 1, 133, -1, 32.00000095367431640625f, 70.0000019073486328125f, 1694.000030517578125f, 0x190, 0, 0.370000012218952178955078125f, 0.50000001490116119384765625f, 1, 134, -1,
    108.0000019073486328125f, 70.0000019073486328125f, 1490.000030517578125f, 0x190, 0, 0.270000018179416656494140625f, 0.320000000298023223876953125f, 1, 125, -1, 1806.000030517578125f, 144.000003814697265625f, 1549.000030517578125f, 0x190, 0,
    0.270000018179416656494140625f, 0.419999994337558746337890625f, 1, 126, -1, 1720.000030517578125f, 123.0000019073486328125f, 1597.000030517578125f, 0x190, 0, 1.2000000774860382080078125f, 1.5499999821186065673828125f, 1, 127, -1, 1866.000030517578125f,
    156.000003814697265625f, 1589.000030517578125f, 0x190, 0, 0.2000000067055225372314453125f, 0.60000003874301910400390625f, 1, 128, -1, 1936.000030517578125f, 174.000003814697265625f, 1520.000030517578125f, 0x190, 0, 0.64999999105930328369140625f,
    1.2000000774860382080078125f, 1, 129, -1, 1889.000030517578125f, 163.000003814697265625f, 1570.000030517578125f, 0x190, 0, 0.300000019371509552001953125f, 0.89999999105930328369140625f, 1, 130, -1, 421.00000762939453125f, 108.0000019073486328125f,
    1734.000030517578125f, 0x190, 0, 0.60000003874301910400390625f, 1.0000000298023223876953125f, 1, 131, -1, 553.0000152587890625f, 108.0000019073486328125f, 1822.000030517578125f, 0x190, 0, 0.1500000096857547760009765625f, 0.2099999971687793731689453125f, 1,
    132, -1, 707.0000152587890625f, 108.0000019073486328125f, 1714.000030517578125f, 0x190, 0, 0.2000000067055225372314453125f, 0.250000007450580596923828125f, 1, 133, -1, 777.0000152587890625f, 108.0000019073486328125f, 1792.000030517578125f,
    0x190, 0, 0.370000012218952178955078125f, 0.50000001490116119384765625f, 1, 134, -1, 901.0000152587890625f, 108.0000019073486328125f, 1664.000030517578125f, 0x190, 0, 0.270000018179416656494140625f, 0.320000000298023223876953125f
};

static void _StopStationaryEmitters(void);

void _InitSoundState(anon_sound_vol_14 *s)
{
    s->sound = -1;
    s->vol = (-2.0f);
    s->pitch = (-2.0f);
    s->lrpan = (-2.0f);
    s->fbpan = (-2.0f);
}

void _StopSound(anon_sound_vol_14 *s, int emitter)
{
    if (emitter) AudStopEmitter(s->sound);
    else AudStopEffect(s->sound);
    _InitSoundState(s);
}


void PlayExitSound(unsigned int edges)
{
    AudPlayEffect((edges & 1) ? 7 : 1, 0, 1, 0);
}

void PlayStartABSound(unsigned int edges)
{
    AudPlayEffect((edges & 1) ? 7 : ((edges & 0xA) ? 0 : 1), 0, 1, 0);
}

void PlayLRSound(unsigned int edges)
{
    AudPlayEffect((edges & 0x1000) ? 3 : 6, 0, 1, 0);
}

void PlayUDSound(unsigned int edges)
{
    AudPlayEffect((edges & 0x400) ? 8 : 2, 0, 1, 0);
}

void EnableGameSounds(int enabled)
{
    gGameSoundsEnabled = enabled;
    AudSetFxVol(enabled ? 0.0f : gOptions[13] / 10.0f, 0.100000002f);
}

int PlayGameSound(int fxid, int subId, int replace,
                  unsigned char priority)
{
    if (!gGameSoundsEnabled) return -1;
    if (fxid == -1) return -1;
    return AudPlayEffect(fxid, subId, replace, priority);
}

int EmitGameSound(float *pos, float *dir, float fadeDistance,
                  float fadeCompression, float maxVol, float minVol,
                  int fxid, int subId, int replace,
                  unsigned char priority)
{
    if (!gGameSoundsEnabled) return -1;
    if (fxid == -1) return -1;
    return AudEmitEffect(pos, dir, fadeDistance, fadeCompression, maxVol,
                         minVol, fxid, subId, replace, priority);
}

static void _StartGameMusic(int resume)
{
    int song;
    static int sTrackSongs[19] = {
        5, 7, 12, 6, 2, 13, 4, 15, 10, 8, 0, 2, 16, 4, 11, 1, 17, 14, 3
    };
    if (gOptions[12]) {
        song = (sTrackSongs[gTrackNum] == -1) ? Random(18.0f) : sTrackSongs[gTrackNum];
        AudSetMusicVol(gOptions[12] / 10.0f, resume ? 1.0f : 0.050000001f);
        if (gOptions[14] == 18) AudPlaySong(song, 1);
        else if (gOptions[14] < 18) AudPlaySong(gOptions[14], 1);
    }
}

void StartGameMusic(void) { _StartGameMusic(0); }
void PauseGameMusic(void) { AudSetMusicVol(0.0f,0.050000001f); }
void ResumeGameMusic(void) { _StartGameMusic(1); }

void StartListeners(void)
{
    int p;
    if (gGameState & 8) return;
    {
        for (p = 0; p < gNumPlayers; p++) {
            if (gRace != 2 || gPlayers[p].slot == 0)
                gPlayers[p].listener = AudAddListener(unitmatrix[3], unitmatrix[3],
                                                     unitmatrix[2], unitmatrix[1]);
            else gPlayers[p].listener = -1;
        }
        AudSetFxVol(gOptions[13] / 10.0f, 0.100000002f);
    }
}

void UpdateListeners(void)
{
    int p;
    for (p = 0; p < gNumPlayers; p++) {
        if (gPlayers[p].listener != -1)
            AudUpdateListener(gPlayers[p].listener, model[gPlayers[p].slot].rwr,
                              unitmatrix[3], gCamera[p].uvs[2], gCamera[p].uvs[1]);
    }
}

void StopListeners(void)
{
    int p;
    _StopStationaryEmitters();
    for (p = 0; p < gNumPlayers; p++) {
        AudRemoveListener(gPlayers[p].listener);
        gPlayers[p].listener = -1;
    }
}

void UpdateStationaryEmitters(void)
{
    int i;
    int fxid;
    struct AudioEmitter *em;
    struct AudioEmitter *prevem;
    float pos[3];
    if (gGameSoundsEnabled) {
        if (gTrackNum == 16 && gWaterfall == -1) {
            pos[0] = -482.0f;
            pos[1] = 217.0f;
            pos[2] = -1772.0f;
            gWaterfall = AudEmitEffect(pos, unitmatrix[3], 1000.0f, 0.0f,
                                      1.0f, 0.0f, 91, 0, 0, 255);
        }
        if (gTrackNum <= 5) {
            em = gStEmitters[gTrackNum];
            for (i = 0, prevem = em; i < 20; i++, prevem = em, em++) {
                if (em->fxid != -1 && em->freqMax != 0.0f && em->nextTime < gFrameCount) {
                    if (em->freqMax < 0.0f) em->nextTime = prevem->nextTime;
                    else {
                        fxid = em->fxid;
                        if (em->numFx > 1) fxid += Random(em->numFx);
                        em->nextTime = gFrameCount + (int)(gRetraceRate *
                                        (em->freqMin + Random(em->freqMax - em->freqMin)));
                    }
                    em->tag = AudEmitEffect(em->pos, unitmatrix[3], em->radius,
                                           0.0f, 1.0f, 0.0f, fxid, 0, 0, 255);
                }
            }
        }
    }
}

static void _StopStationaryEmitters(void)
{
    int i;
    struct AudioEmitter *em;
    if (gGameSoundsEnabled) {
        if (gTrackNum == 16) {
            AudStopEmitter(gWaterfall);
            gWaterfall = -1;
        }
        if (gTrackNum <= 5) {
            em = gStEmitters[gTrackNum];
            for (i = 0; i < 20; i++, em++) {
                if (em->fxid != -1) AudStopEmitter(em->tag);
            }
        }
    }
}

/* Car-sound state and helpers, recovered from the original unit DWARF. */

static anon_specs_state_54 gEngineSounds[6];
static anon_sound_vol_14 gPaveSkidSounds[6];
static anon_dirt_water_3c gRoadNoises[4];
static int gBogSounds[4];
static int gHornSounds[4];
static anon_peak_peakVec_18 gBump[4][5];
static signed short gScrapeState[4];
static int gScrapes[4];
static float gScrapeTime[4];
static float gBumpTime[4];
static float gThumpTime[4];
/* debug_info has no target accesses or proven end; no BSS extent invented. */

static void _GetForceAndPeak(signed short index, int p, float *vec, float threshold);

static void _SetSoundVP(anon_sound_vol_14 *sound, float vol, float pitch)
{
    if (sound->vol != vol) {
        sound->vol = vol;
        AudChangeEffectVol(sound->sound, vol);
    }
    if (sound->pitch != pitch) {
        sound->pitch = pitch;
        AudChangeEffectPitch(sound->sound, pitch);
    }
}

/* Engine specifications retained at their original typed extent. */

static void _InitBumps(signed short p);
static void _UpdateEngineSound(struct tModelData *model);
static void _UpdateSkids(struct tModelData *model);
static void _UpdateRoadSounds(struct tModelData *model);
static void _UpdateBumps(struct tModelData *model);
void StartCarSounds(struct tModelData *model)
{
    int i;
    int p = model->slot;
    anon_specs_state_54 *eng = &gEngineSounds[p];
    if (gGameSoundsEnabled && !model->crashflag) {
        eng->specs = gEngineSpecs[model->engine_type];
        for (i = 0; i < 2; i++) {
            _InitSoundState(&eng->state[i]);
            if (eng->specs[i].id != -1) {
                if (model->drone_type == 2)
                    eng->state[i].sound = AudPlayEffectVP(eng->specs[i].id, p, 0, 0, 0.0f, 0.0f, 0.0f);
                else
                    eng->state[i].sound = AudEmitEffect(unitmatrix[3], unitmatrix[3], 400.0f, 0.0f, 1.0f, 0.0f, eng->specs[i].id, p, 0, 128);
            }
        }
        _InitSoundState(&eng->hum);
        if (model->body_type == 2) {
            if (model->drone_type == 2)
                eng->hum.sound = AudPlayEffectVP(83, p, 0, 0, 0.0f, 0.0f, 0.0f);
            else
                eng->hum.sound = AudEmitEffect(unitmatrix[3], unitmatrix[3], 400.0f, 0.0f, 1.0f, 0.0f, 83, p, 0, 128);
        }
        _InitSoundState(&eng->rev);
        _InitSoundState(&gPaveSkidSounds[p]);
        if (p < 4) {
            _InitSoundState(&gRoadNoises[p].dirt);
            _InitSoundState(&gRoadNoises[p].water);
            _InitSoundState(&gRoadNoises[p].pave);
            gScrapes[p] = -1;
            _InitBumps(p);
            gHornSounds[p] = -1;
            gBogSounds[p] = -1;
        }
        gCarActive[p] = 1;
    }
}

void UpdateCarSounds(struct tModelData *model)
{
    int p = model->slot;
    tPadState *player = &gPlayers[p];
    static float sLastHornTime[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    if (gGameSoundsEnabled && gCarActive[p]) {
        _UpdateEngineSound(model);
        _UpdateSkids(model);
        if (model->drone_type == 2) {
            _UpdateRoadSounds(model);
            _UpdateBumps(model);
            if (gSeconds < sLastHornTime[p]) sLastHornTime[p] = 0.0f;
            if (!(model->appearance & 16) && (player->edges & player->controls[10]) &&
                gSeconds - sLastHornTime[p] > 0.100000002f) {
                sLastHornTime[p] = gSeconds;
                gHornSounds[p] = AudPlayEffect(gCarHorn[p+1][model->body_type] + 10, p, 1, 4);
            }
            if (model->bogState == 1) {
                gBogSounds[p] = AudPlayEffect(55, p, 2, 1);
                model->bogState = 2;
            } else if (model->bogState == 3) {
                AudStopEffect(gBogSounds[p]);
                gBogSounds[p] = -1;
                model->bogState = 0;
            }
        }
    }
}

static void _UpdateEngineSound(struct tModelData *model)
{
    float rpm;
    float torq;
    float pitch;
    float vol;
    int p = model->slot;
    int i;
    anon_specs_state_54 *eng = &gEngineSounds[p];
    float scale1;
    if (gGameSoundsEnabled) {
        rpm = model->rpm < 0 ? -model->rpm : model->rpm;
        torq = model->engtorque;
        if (!(gGameState & 0x400000) && model->drone_type != 2)
            rpm = rpm < 890.0f ? rpm : 890.0f;
        for (i = 0; i < 2; i++) {
            if (eng->state[i].sound == -1) break;
            pitch = 1.0f + (rpm / eng->specs[i].basePitchRpm - 1.0f) *
                    ((float)eng->specs[i].basePitchRpm / eng->specs[i].pitchRpmRange);
            pitch = pitch < 0.0f ? 0.0f : (pitch > 2.0f ? 2.0f : pitch);
            if (rpm < eng->specs[i].lowVolRpm) vol = eng->specs[i].lowVol;
            else if (rpm < eng->specs[i].midVolRpm)
                vol = eng->specs[i].lowVol +
                    ((rpm - eng->specs[i].lowVolRpm) / (eng->specs[i].midVolRpm - eng->specs[i].lowVolRpm)) *
                    (eng->specs[i].midVol - eng->specs[i].lowVol);
            else if (rpm < eng->specs[i].hiVolRpm)
                vol = eng->specs[i].midVol +
                    ((rpm - eng->specs[i].midVolRpm) / (eng->specs[i].hiVolRpm - eng->specs[i].midVolRpm)) *
                    (eng->specs[i].hiVol - eng->specs[i].midVol);
            else vol = eng->specs[i].hiVol;
            scale1 = 0.85f + 0.15f * ((200.0f + torq) / 900.0f);
            vol *= scale1;
            if (model->drone_type == 2) {
                vol *= 0.80000002f - 0.050000001f * (gRace == 2 ? 1 : gNumPlayers);
                if (eng->state[i].pitch != pitch) {
                    eng->state[i].pitch = pitch;
                    AudChangeEffectPitch(eng->state[i].sound, eng->state[i].pitch);
                }
                if (eng->state[i].vol != vol) {
                    eng->state[i].vol = vol;
                    AudChangeEffectVol(eng->state[i].sound, vol);
                }
            } else {
                vol *= 0.85f;
                if (gGameState & 0x400000) {
                    if (eng->rev.sound != -1) _StopSound(&eng->rev, 1);
                } else if (gCountDown > 0.75f &&
                           (eng->rev.sound == -1 || !AudEmitterPlaying(eng->rev.sound)) &&
                           (int)Random(5.0f) == 0) {
                    eng->rev.sound = AudEmitEffect(unitmatrix[3], unitmatrix[3],
                        400.0f, 0.0f, 1.0f, 0.0f, 80.0f + Random(3.0f), p, 0, 128);
                    AudUpdateEmitter(eng->rev.sound, model->rwr, unitmatrix[3], 0.80000002f, (-2.0f));
                }
                if (eng->state[i].pitch != pitch) eng->state[i].pitch = pitch;
                else pitch = (-2.0f);
                AudUpdateEmitter(eng->state[i].sound, model->rwr, unitmatrix[3], vol, pitch);
            }
        }
        if (eng->hum.sound != -1) {
            if ((model->reckon.rwmph >> 2) < 0) vol = 0.25f - Random(0.15f);
            else if (model->throttle < 0.100000002f) {
                if (vol < 0.28f) vol = 0.25f - Random(0.15f);
                else {
                    vol -= 0.01f + Random(0.025f);
                    if (vol < 0.2f) vol = 0.2f;
                }
            } else {
                vol = 0.2f + (1.35f * model->rpm) / 10000.0f;
                vol = vol > 1.0f ? 1.0f : vol;
                vol -= Random(0.15f);
            }
            if (model->drone_type == 2) {
                vol *= 0.80000002f - 0.050000001f * (gRace == 2 ? 1 : gNumPlayers);
                if (eng->hum.vol != vol) {
                    eng->hum.vol = vol;
                    AudChangeEffectVol(eng->hum.sound, vol);
                }
            } else {
                vol *= 0.75f;
                AudUpdateEmitter(eng->hum.sound, model->rwr, unitmatrix[3], vol, 1.0f);
            }
        }
    }
}

static void _UpdateSkids(struct tModelData *model)
{
    int p = model->slot;
    int i;
    float pitch;
    float maxPitch = 0.5f;
    float maxVol = 0.0f;
    static const float sPitchRange[4] = { 0.80000002682209014892578125f, 0.80000002682209014892578125f, 1.0000000298023223876953125f, 0.60000003874301910400390625f };
    for (i = 0; i < 4; i++) {
        if (model->sviscode[i] == 0) {
            pitch = 1.0f + (sPitchRange[i] * model->skidIntensity[i] - sPitchRange[i] / 2.0f);
            if (pitch > maxPitch) maxPitch = pitch;
            pitch = 0.7f * (model->skidIntensity[i] * sPitchRange[i]);
            if (pitch > maxVol) maxVol = pitch;
        }
    }
    if (gPaveSkidSounds[p].sound == -1 && maxVol > 0.0f) {
        if (model->drone_type == 2)
            gPaveSkidSounds[p].sound = AudPlayEffect(79, model->slot, 2, 2);
        else
            gPaveSkidSounds[p].sound = AudEmitEffect(model->rwr, unitmatrix[3], 400.0f, 0.0f, 1.0f, 0.0f, 79, p, 0, 130);
    }
    if (gPaveSkidSounds[p].sound != -1) {
        if (maxVol <= 0.0f) {
            if (model->drone_type == 2) AudStopEffect(gPaveSkidSounds[p].sound);
            else AudStopEmitter(gPaveSkidSounds[p].sound);
            _InitSoundState(&gPaveSkidSounds[p]);
        } else if (model->drone_type == 2) {
            if (gPaveSkidSounds[p].vol != maxVol) {
                gPaveSkidSounds[p].vol = maxVol;
                AudChangeEffectVol(gPaveSkidSounds[p].sound, maxVol);
            }
            if (gPaveSkidSounds[p].pitch != maxPitch) {
                gPaveSkidSounds[p].pitch = maxPitch;
                AudChangeEffectPitch(gPaveSkidSounds[p].sound, gPaveSkidSounds[p].pitch);
            }
        } else {
            if (gPaveSkidSounds[p].pitch != maxPitch)
                gPaveSkidSounds[p].pitch = maxPitch;
            else maxPitch = (-2.0f);
            AudUpdateEmitter(gPaveSkidSounds[p].sound, model->rwr, unitmatrix[3], maxVol, maxPitch);
        }
    }
}
static void _UpdateRoadSounds(struct tModelData *model)
{
    int i;
    int p = model->slot;
    int numPave = 0;
    int numDirt = 0;
    int numWater = 0;
    float vol;
    float dirtVol = 0.0f;
    float paveVol = 0.0f;
    float waterVol = 0.0f;
    for (i = 0; i < 4; i++) {
        if (model->sviscode[i] == 0) {
            vol = (model->reckon.rwmph >> 2) / 160.0f;
            vol = vol < 0.0f ? -vol : vol;
            vol = vol < 0.0f ? 0.0f : (vol > 1.0f ? 1.0f : vol);
            vol = 1.0f - (1.0f - vol) * (1.0f - vol);
            ++numPave;
            vol /= (numPave == 1 ? 2.0f : (numPave == 2 ? 4.0f : 8.0f));
            paveVol += vol;
        } else if (model->sviscode[i] == 1) {
            vol = (model->reckon.rwmph >> 2) / 80.0f;
            if (vol) {
                if (vol < 0.0f) vol = -vol;
                vol = vol < 0.0f ? 0.0f : (vol > 1.0f ? 1.0f : vol);
                vol = 1.0f - (1.0f - vol) * (1.0f - vol);
            }
            if (model->skidIntensity[i] > vol) vol = model->skidIntensity[i];
            ++numDirt;
            vol /= (numDirt == 1 ? 2.0f : (numDirt == 2 ? 4.0f : 8.0f));
            dirtVol += vol;
        } else if (model->sviscode[i] == 2 || model->sviscode[i] == 3) {
            vol = (model->reckon.rwmph >> 2) / 80.0f;
            if (vol) {
                if (vol < 0.0f) vol = -vol;
                vol = vol < 0.0f ? 0.0f : (vol > 1.0f ? 1.0f : vol);
                vol = 1.0f - (1.0f - vol) * (1.0f - vol);
            }
            ++numWater;
            vol /= (numWater == 1 ? 2.0f : (numWater == 2 ? 4.0f : 8.0f));
            waterVol += vol;
        }
    }
    if (dirtVol > 0.0f) {
        if (gRoadNoises[p].dirt.sound == -1)
            gRoadNoises[p].dirt.sound = AudPlayEffect(63, p, 1, 2);
        _SetSoundVP(&gRoadNoises[p].dirt, 0.7f * dirtVol,
                    (dirtVol > 0.5f ? dirtVol : 0.5f) - 0.25f);
    } else if (!numDirt && gRoadNoises[p].dirt.sound != -1) {
        AudStopEffect(gRoadNoises[p].dirt.sound);
        _InitSoundState(&gRoadNoises[p].dirt);
    }
    if (waterVol > 0.0f) {
        if (gRoadNoises[p].water.sound == -1) {
            if (waterVol > 0.33f) AudPlayEffect(86, model->slot, 2, 1);
            gRoadNoises[p].water.sound = AudPlayEffect(92, model->slot, 1, 2);
        }
        _SetSoundVP(&gRoadNoises[p].water, waterVol, 1.0f);
    } else if (!numWater && gRoadNoises[p].water.sound != -1) {
        AudStopEffect(gRoadNoises[p].water.sound);
        _InitSoundState(&gRoadNoises[p].water);
    }
    if (paveVol > 0.0f) { }
    else if (!numPave && gRoadNoises[p].pave.sound != -1) {
        AudStopEffect(gRoadNoises[p].pave.sound);
        _InitSoundState(&gRoadNoises[p].pave);
    }
}

static void _CheckForces(struct tModelData *model);

static void _UpdateBumps(struct tModelData *model)
{
    int i;
    signed short *scrapeState;
    int p = model->slot;
    float vol;
    float highForce;
    signed short bumpIndex;
    signed short scrapeSide;
    signed short scrapeSnd;
    if (model->crashflag) {
        _InitBumps(p);
        return;
    }
    _CheckForces(model);
    highForce = 0.0f;
    bumpIndex = 0;
    if (gBumpTime[p] && (gSeconds - gBumpTime[p] > 1.0f || gSeconds < gBumpTime[p]))
        gBumpTime[p] = 0.0f;
    for (i = 0; i < 4; i++) {
        if (gBump[p][i].bump && gBump[p][i].peak > highForce) highForce = gBump[p][i].peak;
    }
    vol = (0.01f * highForce < 150.0f ? 150.0f : (0.01f * highForce > 235.0f ? 235.0f : 0.01f * highForce));
    vol *= 0.00390625f;
    vol *= 0.65f;
    highForce = 0.5f * highForce;
    for (i = 0; i < 4; i++) {
        if (gBump[p][i].bump) {
            if (gBump[p][i].peak > highForce) bumpIndex |= 1 << i;
            gBump[p][i].peak = 0.0f;
            gBump[p][i].bump = 0;
        }
    }
    if (bumpIndex) {
        if (gBumpTime[p] == 0.0f) {
            AudPlayEffectVP(64.0f + Random(13.0f), model->slot, 1, 1, vol,
                            gBumpLRTab[bumpIndex], gBumpFBTab[bumpIndex]);
            gBumpTime[p] = gSeconds;
        } else {
            AudPlayEffectVP(64.0f + Random(13.0f), model->slot, 1, 1, vol,
                            gBumpLRTab[bumpIndex], gBumpFBTab[bumpIndex]);
            gBumpTime[p] = 0.0f;
        }
    } else {
        bumpIndex = 0;
        if (gBump[p][4].bump) {
            highForce = 0.0f;
            for (i = 0; i < 3; i++)
                if (fabs(gBump[p][4].peakVec[i]) > highForce) highForce = fabs(gBump[p][4].peakVec[i]);
            vol = (0.001f * highForce < 180.0f ? 180.0f : (0.001f * highForce > 235.0f ? 235.0f : 0.001f * highForce));
            vol *= 0.00390625f;
            if (fabs(gBump[p][4].peakVec[0]) > 0.5f * highForce) {
                if (gBump[p][4].peakVec[0] < 0.0f) bumpIndex |= 1;
                else bumpIndex |= 2;
            }
            if (fabs(gBump[p][4].peakVec[1]) > 0.5f * highForce) {
                if (gBump[p][4].peakVec[1] < 0.0f) bumpIndex |= 4;
                else bumpIndex |= 8;
            }
            if (fabs(gBump[p][4].peakVec[2]) > 0.5f * highForce) bumpIndex |= 1;
            gBump[p][4].peak = 0.0f;
            gBump[p][4].bump = 0;
        }
        if (bumpIndex)
            AudPlayEffectVP(64.0f + Random(13.0f), model->slot, 1, 1, vol,
                            gBumpLRTab[bumpIndex], gBumpFBTab[bumpIndex]);
    }
    if (!bumpIndex && model->magvel > 20.0f) {
        scrapeSide = 0;
        if (model->bodyForce[0][0] || model->bodyForce[2][0]) scrapeSide |= 1;
        if (model->bodyForce[1][0] || model->bodyForce[3][0]) scrapeSide |= 2;
        scrapeState = &gScrapeState[p];
        scrapeSnd = 0;
        switch (*scrapeState) {
        case 0:
            *scrapeState = scrapeSide;
            scrapeSnd = scrapeSide;
            gScrapeTime[p] = gSeconds;
            break;
        case 1: case 2: case 3:
            if (!scrapeSide) {
                AudStopEffect(gScrapes[p]);
                gScrapes[p] = -1;
                *scrapeState = 0;
                gScrapeTime[p] = 0.0f;
            } else if (gSeconds - gScrapeTime[p] > 0.100000002f || gSeconds < gScrapeTime[p]) {
                *scrapeState += 3;
                scrapeSnd = *scrapeState;
            }
            break;
        case 4: case 5: case 6:
            if (!scrapeSide) {
                AudStopEffect(gScrapes[p]);
                gScrapes[p] = -1;
                *scrapeState = 0;
                gScrapeTime[p] = 0.0f;
            }
            break;
        }
        switch (scrapeSnd) {
        case 1: AudPlayEffectVP(64.0f + Random(13.0f), model->slot, 1, 1, 0.65f, 1.0f, 0.0f); break;
        case 2: AudPlayEffectVP(64.0f + Random(13.0f), model->slot, 1, 1, 0.65f, -1.0f, 0.0f); break;
        case 3: AudPlayEffectVP(64.0f + Random(13.0f), model->slot, 1, 1, 0.65f, 0.0f, 0.0f); break;
        case 4: gScrapes[p] = AudPlayEffectVP(85, model->slot, 1, 2, 0.65f, 1.0f, 0.0f); break;
        case 5: gScrapes[p] = AudPlayEffectVP(85, model->slot, 1, 2, 0.65f, -1.0f, 0.0f); break;
        case 6: gScrapes[p] = AudPlayEffectVP(85, model->slot, 1, 2, 0.65f, 0.0f, 0.0f); break;
        }
    } else {
        AudStopEffect(gScrapes[p]);
        gScrapes[p] = -1;
        gScrapeState[p] = 0;
        gScrapeTime[p] = 0.0f;
    }
    if (gThumpTime[p] != 0.0f && (gSeconds - gThumpTime[p] > (1.0f / 6.0f) || gSeconds < gThumpTime[p]))
        gThumpTime[p] = 0.0f;
    if (model->mthumpflag > model->thumpflag) model->mthumpflag = model->thumpflag;
    if (model->mthumpflag != model->thumpflag) {
        model->mthumpflag = model->thumpflag;
        if (!gThumpTime[p]) {
            gThumpTime[p] = gSeconds;
            if (!(gGameState & 0x200000)) {
                switch (model->mthumpflag) {
                case 1: AudPlayEffectVP(60, model->slot, 1, 2, 1.0f, gBumpLRTab[model->thump_side], gBumpFBTab[model->thump_side]); break;
                case 2: AudPlayEffectVP(58, model->slot, 1, 2, 1.0f, gBumpLRTab[model->thump_side], gBumpFBTab[model->thump_side]); break;
                case 3: AudPlayEffectVP(56, model->slot, 1, 2, 1.0f, gBumpLRTab[model->thump_side], gBumpFBTab[model->thump_side]); break;
                }
            }
        }
    }
}

static void _CheckForces(struct tModelData *model)
{
    signed short i;
    for (i = 0; i < 4; i++)
        _GetForceAndPeak(i, model->slot, model->bodyForce[i], 5000.0f);
    _GetForceAndPeak(4, model->slot, model->centerForceLast, 5000.0f);
}

static void _GetForceAndPeak(signed short index, int p, float *vec, float threshold)
{
    if (gBump[p][index].bump) return;
    if (gBump[p][index].time && gBump[p][index].peak == 0.0f) {
        if (vec[0] == 0.0f && vec[1] == 0.0f && vec[2] == 0.0f) {
            if (gSeconds - gBump[p][index].time > 0.5f)
                gBump[p][index].time = 0.0f;
        } else gBump[p][index].time = gSeconds;
        return;
    }
    if (vec[0] != 0.0f || vec[1] != 0.0f || vec[2] != 0.0f) {
        float force = sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        if (threshold < force && force > gBump[p][index].peak) {
            gBump[p][index].peak = force;
            gBump[p][index].peakVec[0] = vec[0];
            gBump[p][index].peakVec[1] = vec[1];
            gBump[p][index].peakVec[2] = vec[2];
            gBump[p][index].time = gSeconds;
        }
    }
    if (gBump[p][index].time && gSeconds - gBump[p][index].time > 0.100000002f) {
        gBump[p][index].bump = 1;
        gBump[p][index].time = gSeconds;
    }
}

static void _InitBumps(signed short p)
{
    int i;
    for (i = 0; i < 5; i++) {
        gBump[p][i].peak = 0.0f;
        gBump[p][i].time = 0.0f;
        gBump[p][i].bump = 0;
    }
    AudStopEffect(gScrapes[p]);
    gScrapes[p] = -1;
    gScrapeState[p] = 0;
    gScrapeTime[p] = 0.0f;
    gBumpTime[p] = 0.0f;
    gThumpTime[p] = 0.0f;
}

void StopCarSounds(struct tModelData *model)
{
    int i;
    int p = model->slot;
    if (gCarActive[p]) {
        gCarActive[p] = 0;
        gCarUpdatedLastTime[p] = !(p & 1);
        for (i = 0; i < 2; i++) {
            if (gEngineSounds[p].state[i].sound != -1)
                _StopSound(&gEngineSounds[p].state[i], model->drone_type != 2);
        }
        if (gEngineSounds[p].hum.sound != -1)
            _StopSound(&gEngineSounds[p].hum, model->drone_type != 2);
        if (gEngineSounds[p].rev.sound != -1)
            _StopSound(&gEngineSounds[p].rev, model->drone_type != 2);
        _StopSound(&gPaveSkidSounds[p], model->drone_type != 2);
        if (model->drone_type == 2) {
            _StopSound(&gRoadNoises[p].dirt, 0);
            _StopSound(&gRoadNoises[p].water, 0);
            _StopSound(&gRoadNoises[p].pave, 0);
            AudStopEffect(gScrapes[p]);
            gScrapes[p] = -1;
            gScrapeState[p] = 0;
            gScrapeTime[p] = 0.0f;
            AudStopEffect(gHornSounds[p]);
            gHornSounds[p] = -1;
            AudStopEffect(gBogSounds[p]);
            gBogSounds[p] = -1;
        }
    }
}

int PlayTargetSound(int fxid, int slot, float *pos, unsigned char priority)
{
    int bumpIndex;
    float pos2[3];
    float delta[3];
    float sq_mag;
    float vec1;
    float vec2;
    float vec3;
    struct tCarData *gc = &gGameCar[slot];
    if (!gGameSoundsEnabled) return -1;
    if (gLink[slot].owner != 6) return -1;
    delta[0] = pos[0] - gc->dr_pos[0];
    delta[1] = pos[1] - gc->dr_pos[1];
    delta[2] = pos[2] - gc->dr_pos[2];
    BodyVector(delta, pos2, gc->dr_uvs);
    bumpIndex = 0;
    vec1 = pos2[2] * pos2[2];
    vec2 = pos2[0] * pos2[0];
    sq_mag = vec1 + vec2;
    if (sq_mag > 1.0f) {
        vec1 *= pos2[2] >= 0.0f ? 1 : -1;
        vec2 *= pos2[0] >= 0.0f ? 1 : -1;
        vec3 = (0.924f * 0.924f) * sq_mag;
        if (vec2 > vec3) bumpIndex |= 4;
        else if (vec2 < -vec3) bumpIndex |= 8;
        if (vec1 > vec3) bumpIndex |= 1;
        else if (vec1 < -vec3) bumpIndex |= 2;
    }
    if (fxid == 59)
        return AudPlayEffectVP(fxid, slot, 1, priority, 1.0f, 0.0f, 0.0f);
    return AudPlayEffectVP(fxid, slot, 1, priority, 1.0f,
                           gBumpLRTab[bumpIndex], gBumpFBTab[bumpIndex]);
}

