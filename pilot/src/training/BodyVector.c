/* Static translation scaffold only; not an accepted equivalent-C or byte match.
 * Target code 0x8C026B00..0x8C026B5E (file 0x16B00..0x16B5E).
 * DC_FMAC records the observed SH-4 instruction operation. It must be replaced
 * only after target FPSCR/rounding is established. Host fmaf is not proof.
 * Keep each source reload after the preceding destination write: aliasing is
 * observable in the original. Do not add restrict or cache src across rows.
 */
extern float DC_FMAC(float fr0, float frm, float frn);

void BodyVector_candidate(float *src, float *dst, float (*uvs)[3])
{
    float acc;
    acc = src[2] * uvs[0][2];
    acc = DC_FMAC(src[1], uvs[0][1], acc);
    acc = DC_FMAC(src[0], uvs[0][0], acc);
    dst[0] = acc;
    acc = src[2] * uvs[1][2];
    acc = DC_FMAC(src[1], uvs[1][1], acc);
    acc = DC_FMAC(src[0], uvs[1][0], acc);
    dst[1] = acc;
    acc = src[2] * uvs[2][2];
    acc = DC_FMAC(src[1], uvs[2][1], acc);
    acc = DC_FMAC(src[0], uvs[2][0], acc);
    dst[2] = acc; /* emitted in RTS delay slot */
}
