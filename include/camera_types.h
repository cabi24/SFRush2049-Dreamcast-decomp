#include "dc_types.h"
/* Private camera overlays. DC offsets are proved by this unit or prior integrated evidence. */
#ifndef CAMERA_TYPES_H
#define CAMERA_TYPES_H
typedef tCamera anon_mat_n64_98;



 /* stride98:8C018E4C */
 /* CheckCameraView save handle+48 */
 /* stride50:CheckCameraView; control8/9 at38/3c */



 /* model75c:pool8C0195A2 */

typedef struct anon_pos_mph_8 { signed short pos[3]; unsigned char mph,hints; } anon_pos_mph_8;
typedef struct anon_numPts_pts_8 { unsigned short numPts; anon_pos_mph_8 *pts; } anon_numPts_pts_8; /* stride8 and pos short:8C01AA14..64 */
typedef struct anon_zoom_unused_10 { signed char zoom,unused; signed short unused2; float pos[3]; } anon_zoom_unused_10; /* target16byte spot stride8C01AB0C */
typedef char check_car[(sizeof(tCarData)==0x3cc)?1:-1];
typedef char check_model[(sizeof(tModelData)==0x808)?1:-1];
typedef char check_camera[(sizeof(anon_mat_n64_98)==0x98)?1:-1];
typedef char check_pad[(sizeof(tPadState)==0x50)?1:-1];
extern tCarData gGameCar[6]; extern tModelData model[6]; extern tPadState gPlayers[4];
extern anon_numPts_pts_8 gDronePaths[4];
extern unsigned int gGameState; extern int gPauseState,gRace;
extern signed short gNumCars,gCountDown;
extern unsigned int gContEdges[4],gContLevels[4];
extern float gModelTime; extern volatile float gFrameSeconds; extern volatile int gLastFrameCount;
extern signed char gTrackNum,gFrameScale;
extern float unitmatrix[3][3],rvector[3],uvector[3];
float _builtin_sqrtf(float x); float _builtin_fabsf(float x);
float dc_cos(float x); float dc_sin(float x);
float magnitude(float *v); float Distance2D(float x,float z);
void direction(float *src,float *dst); void CopyUV(float src[3][3],float dst[3][3]);
void WorldVector(float *v,float *out,float uvs[3][3]); void BodyVector(float *v,float *out,float uvs[3][3]);
void YawUV(float angle,float uvs[3][3]);
void Interpolate(float *a,float *b,signed short n,float start,float end,float value,float *out);
void SkyUpdate(int i); void UpdateHUDPostCamera(void); void ShowCountdown(int show);
signed short NextPathPt(signed short idx,signed short path);
float GetCountdownTimeRemaining(void); float Random(float max);
void SavePlayerCarView(tPlayerProfile **p,unsigned char view); unsigned char GetPlayerCarView(tPlayerProfile **p);
void BlinkResurrectingCars(void); tUVMatrix *ZOID_FindMatrix(int obj); void ZOID_UpdateObject(int obj,float *pos,float uvs[3][3]);
void CameraCollision(float *pos,float *old);
void ZeroCameras(void); void ZeroCamera(signed short camera); void init_views(void); void init_view3(tCarData *car);
void UpdateCameras(void); void SetCameraView(signed short camera); void CheckCameraView(void);
float view1_suscomp_offset(tModelData *m); void View3Cam(tCarData *car,float *pos); void update_camera(tCarData *car);
void update_car_object(tCarData *car,float *pos,float uvs[3][3]); void UpdateCarObj(tCarData *car,float *pos,float uvs[3][3],float *carpos);
void DeathCam(tCarData *car,float *pos,float uvs[3][3]); void circle_camera_around_car(tCarData *car,float *pos,float uvs[3][3]);
void init_camera_on_track(tCarData *car); void fix_camera_in_space(signed short mode,tCarData *car,float *pos,float uvs[3][3]);
void init_steady_move_cam(tCarData *car); void steady_move_cam(signed short mode,tCarData *car,float *pos,float uvs[3][3]);
void maxpath_cam(signed short mode,tCarData *car,float *pos,float uvs[3][3]); void point_at_car(tCarData *car,float *pos,float uvs[3][3],float *res);
void LookInDir(float *dir,tUVMatrix *mat);
#endif
