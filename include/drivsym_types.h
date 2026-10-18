#include "dc_types.h"
/* Private drivsym DC overlays. Named offsets verified in target.txt; gaps are unknown.
   Model allocation end is not claimed here; only its used prefix through appearance+4.
   Canonical full layouts remain read-only. */
#ifndef DRIVSYM_TYPES_H
#define DRIVSYM_TYPES_H
typedef tTire anon_tradius_springK_5c;









typedef char check_tire_stride[(sizeof(anon_tradius_springK_5c)==0x5c)?1:-1];
typedef char check_surface_stride[(sizeof(surftyp)==0x18)?1:-1]; /* 3B6C..3B74 = index*24 */
typedef char check_car_stride[(sizeof(tCarData)==0x3cc)?1:-1]; /* pool4208/muls.w4126 */
#define gGameCar ((tCarData *)0x8C004000)
enum { kBattle=6,kStunt=4 };
extern signed char gEndGame,gCoastFlag,gMirrorWaves;
extern int gRace;
extern signed char gPhysicsWingRows[5][13];
extern surftyp *surf;
void initialize(tModelData *);
void road(tModelData *);
void drivetrain(tModelData *);
void GetTargetVelocity(int,float *);
float NormalVector(float *);
void BodyVector(float *,float *,float (*)[3]);
void WorldVector(float *,float *,float (*)[3]);
void RotateUV(float *,float (*)[3]);
void calctireuv(float *,float *,float *,float,float (*)[3],float (*)[3],float (*)[3],float *);
void dotireforce(tModelData *,float *,float (*)[3],anon_tradius_springK_5c *,float,float *,float,float,float,float,float,float,int,float);
float magnitude(float *);
float invmagnitude(float *);
void make_super_speed(tModelData *);
void AddBattlePoint(signed char,signed char,signed char);
void syminit(tModelData *);void sym(tModelData *);void regular(tModelData *);void antispin(tModelData *);
void MovingSurfaceForce(tModelData *);void forces(tModelData * volatile);void forces1(tModelData *);void forces2(tModelData *);
void torques(tModelData *);void accelerations(tModelData *);void velocities(tModelData *);void positions(tModelData *);
void calcaa(tModelData *,float *,float *);void checkok(tModelData *);void mcommunication(tModelData *);
#endif
