/* fmath.c: BodyVector, MAT3 body verbatim. */
void BodyVector(float *src, float *dst, float (*uvs)[3])
{
    dst[0] = src[0] * uvs[0][0] + src[1] * uvs[0][1] + src[2] * uvs[0][2];
    dst[1] = src[0] * uvs[1][0] + src[1] * uvs[1][1] + src[2] * uvs[1][2];
    dst[2] = src[0] * uvs[2][0] + src[1] * uvs[2][1] + src[2] * uvs[2][2];
}
