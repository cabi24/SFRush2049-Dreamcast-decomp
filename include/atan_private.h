/* Self-contained C89 declarations. No Dreamcast struct overlays required.
 * Target bsr/bra call graph, 8C01140A..8C0114F0, proves all callees are local.
 */
#ifndef ATAN_PRIVATE_H
#define ATAN_PRIVATE_H
float fsatan(float arg);
float fatan2(float arg_1, float arg_2);
float fatan(float arg);
#endif
