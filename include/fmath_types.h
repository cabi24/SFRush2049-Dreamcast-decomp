/* DC single-float ABI, with target-proven native instruction builtins. */
#ifndef FMATH_TYPES_H
#define FMATH_TYPES_H
extern float _builtin_sqrtf(float);
extern float _builtin_fabsf(float);
#define sqrtf _builtin_sqrtf
#define fabsf _builtin_fabsf
extern float sinf(float);
extern float cosf(float);
extern float facos(float);
extern float fatan2(float,float);
extern int rand(void);
void YawUV(float,float (*)[3]);
void PitchUV(float,float (*)[3]);
void RollUV(float,float (*)[3]);
#endif
