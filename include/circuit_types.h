#include "dc_types.h"
#ifndef DC_CIRCUIT_TYPES_H
#define DC_CIRCUIT_TYPES_H
/* Partial Dreamcast overlays; offsets follow circuit target code. */

 /* sizeof4c:8C01C55C/562 and PS_WriteFile size8C01C58A */


 /* stride50:8C01C56C..578 */
 /* stride4c:8C01C70E,8C01C6D6 */
 /* stride5:8C01C8B2..BA */
typedef char dc_check_circuit[(sizeof(tCircuit)==0x4c)?1:-1];
typedef char dc_check_circuit_profile[(sizeof(tPlayerProfile)==0x4c)?1:-1];
typedef char dc_check_circuit_player[(sizeof(tPadState)==0x50)?1:-1];
typedef char dc_check_circuit_car[(sizeof(tCircuitCar)==0x4c)?1:-1];
typedef char dc_check_circuit_track[(sizeof(tCircuitTrack)==5)?1:-1];
/* Target uses the fixed low-memory player area as a separate base, rather
   than folding a relocatable array symbol plus field offset. Pool8C01C68C. */
#define gPlayers ((tPadState *)0x8C006000)
extern signed short gNumPlayers;
extern signed char gOptions[];
extern volatile float gSeconds;
void *memset(void *dst,int fill,unsigned int size);
void PS_WriteFile(tPS_File **fileh,void *src,int size);
unsigned int rand_r(unsigned int *seed);
void RecordCircuit(void);
void SavePlayerCircuit(tPlayerProfile **player);
int func_8C044C2C(unsigned int numDataBits,unsigned int numChecksumBits);
int func_8C044CA4(unsigned int data,unsigned int numBits);
int func_8C044D0C(unsigned int *data,unsigned int numBits);
void GetPasswordString(char *password);
int func_8C044E94(char *password);
#define NewPassword func_8C044C2C
#define StorePasswordElement func_8C044CA4
#define GetPasswordElement func_8C044D0C
#define SetPasswordString func_8C044E94
void func_8C01C554(int circuitType);
void func_8C01C5E4(void);
#endif
