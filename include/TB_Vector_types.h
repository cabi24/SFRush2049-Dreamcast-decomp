#ifndef TB_VECTOR_TYPES_H
#define TB_VECTOR_TYPES_H
/* SH-4 intrinsic: targetB0AE/B0F2; established native skids.c convention. */
extern float _builtin_sqrtf(float value);
#define sqrtf(value) _builtin_sqrtf(value)
#endif
