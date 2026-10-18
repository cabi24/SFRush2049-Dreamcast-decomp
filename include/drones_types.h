#include "dc_types.h"
/* Private DC drones overlays; target citations verify each field/stride. */
#ifndef DRONES_TYPES_H
#define DRONES_TYPES_H
#define kCircuit 3
#define fabs(x) _builtin_fabsf((float)(x))


 /* stride4c:immediate5BEE/muls.w5BF4. */
typedef char dc_check_drone_model[(sizeof(tModelData)==0x808)?1:-1];
typedef char dc_check_drone_gcar[(sizeof(tCarData)==0x3cc)?1:-1];
typedef char dc_check_drone_circuit[(sizeof(tCircuitCar)==0x4c)?1:-1];
#endif
