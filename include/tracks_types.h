#include "dc_types.h"
/* Private DC layout proposals for tracks.c; no canonical header edits. */
#ifndef DC_TRACKS_TYPES_H
#define DC_TRACKS_TYPES_H

/* The wad occupies stack+8..20 in LoadTrack: BEE8, BF66..6E. */


typedef struct anon_loading_loaded_8 {
    signed char loading; /* track+0 BF12, world+0 BF44, collision BFF2, path C08C */
    signed char loaded; /* BF24, BF56, C000, C09E */
    signed char inited; /* BF26, BFB0, C04A, C0A2 */
    signed char ready; /* BF28, BFD4, C060, C0B8 */
    int idx; /* stage+4: BF0E, BF40, BFEE, C088; stage stride8 */
} anon_loading_loaded_8;

typedef struct anon_trackInited_track_2c {
    signed char trackInited; /* +0: C1F8/C200; byte stores in ReloadTrack. */
    char pad0[0x4 - 0x1];
    anon_loading_loaded_8 track; /* +4: BEF6, BF0E. */
    anon_loading_loaded_8 world; /* +C: BF2A, BF40. */
    anon_loading_loaded_8 collision; /* +14: BFD6, BFEE. */
    anon_loading_loaded_8 path; /* +1C: C062, C088. */
    anon_loading_loaded_8 other; /* +24: C0BA, C1EC..F6; memset size2C at C2D4. */
} anon_trackInited_track_2c;



typedef struct tTarget tTarget;
typedef struct tTargetGroup tTargetGroup;

typedef char dc_check_track_stage[(sizeof(anon_loading_loaded_8)==8)?1:-1];
typedef char dc_check_track_info[(sizeof(anon_trackInited_track_2c)==0x2c)?1:-1];
typedef char dc_check_world_header[(sizeof(anon_woff_wname_14)==0x14)?1:-1];
typedef char dc_check_track_wad[(sizeof(anon_base_TOCentries_c)==12)?1:-1];
#endif
