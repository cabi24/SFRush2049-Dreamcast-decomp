#include "dc_types.h"
#ifndef DC_REKON_TYPES_H
#define DC_REKON_TYPES_H
typedef tTire tRekonTire;
typedef tFastIn tRekonFastIn;
typedef tModelData tRekonModel;
typedef tCarData tRekonCar;











typedef char dc_check_rekon_tire[(sizeof(tRekonTire)==0x5c)?1:-1];
typedef char dc_check_rekon_state[(sizeof(tRekonState)==0x90)?1:-1];
typedef char dc_check_rekon_model[(sizeof(tRekonModel)==0x808)?1:-1];
typedef char dc_check_rekon_car[(sizeof(tRekonCar)==0x3cc)?1:-1];
#endif



