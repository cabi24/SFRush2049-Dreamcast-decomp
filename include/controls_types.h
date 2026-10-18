#include "dc_types.h"
/* Private DC controls overlays; every named field has target evidence. */
#ifndef CONTROLS_TYPES_H
#define CONTROLS_TYPES_H




 /* stride50:E8F0,E BCA; no inferred profile stride. */

typedef char dc_check_ctl_main[(sizeof(tMainIn)==0x14)?1:-1];
typedef char dc_check_ctl_model[(sizeof(tModelData)==0x808)?1:-1];
typedef char dc_check_ctl_car[(sizeof(tCarData)==0x3cc)?1:-1];
typedef char dc_check_ctl_pad[(sizeof(tPadState)==0x50)?1:-1];
typedef char dc_check_ctl_info[(sizeof(tPadInfo)==0x14)?1:-1];
#endif
