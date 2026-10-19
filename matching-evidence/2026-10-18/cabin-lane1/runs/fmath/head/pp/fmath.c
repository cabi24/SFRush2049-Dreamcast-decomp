#line 1 "fmath_types.h"
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
#line 2 "fmath.c"

float unitmatrix[4][3] = {1,0,0,0,1,0,0,0,1,0,0,0};
float uvector[3] = {0,1,0};
float rvector[3] = {1,0,0};

float func_8C026278(float val)
{
    if (val < 9.9999998383282218e-05f) val = 9.9999998383282218e-05f;
    return 1.0f / sqrtf(val);
}
float magnitude(float *vec)
{
    return sqrtf(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}
float invmagnitude(float *vec)
{
    float x,y,z;
    x=vec[0]; y=vec[1]; z=vec[2];
    return func_8C026278(x*x+y*y+z*z);
}
#pragma inline(func_8C0262D2)
float func_8C0262D2(float x, float y, float z) { return sqrtf(x*x + y*y + z*z); }
float func_8C0262E4(float x, float z) { return sqrtf(x*x + z*z); }
void direction(float *vec, float *dir)
{
    float invmag = invmagnitude(vec);
    dir[0] = vec[0] * invmag;
    dir[1] = vec[1] * invmag;
    dir[2] = vec[2] * invmag;
}
void CopyUV(float (*src)[3], float (*dst)[3])
{
    dst[0][0] = src[0][0]; dst[0][1] = src[0][1]; dst[0][2] = src[0][2];
    dst[1][0] = src[1][0]; dst[1][1] = src[1][1]; dst[1][2] = src[1][2];
    dst[2][0] = src[2][0]; dst[2][1] = src[2][1]; dst[2][2] = src[2][2];
}

void TransposeUV(float (*uv)[3])
{
    float tmp;
    tmp=uv[0][1]; uv[0][1]=uv[1][0]; uv[1][0]=tmp;
    tmp=uv[0][2]; uv[0][2]=uv[2][0]; uv[2][0]=tmp;
    tmp=uv[1][2]; uv[1][2]=uv[2][1]; uv[2][1]=tmp;
}
void ScaleUVXYZ(float (*a)[3], float (*b)[3], float sx, float sy, float sz)
{
    int i;
    for(i=0;i<3;i++) {
        b[0][i]=sx*a[0][i]; b[1][i]=sy*a[1][i]; b[2][i]=sz*a[2][i];
    }
}
void ScaleUV(float (*a)[3], float (*b)[3], float sfactor)
{
    int i,j;
    for(i=0;i<3;i++) for(j=0;j<3;j++) b[i][j]=sfactor*a[i][j];
}
void WYawUV(float angle, float (*uv)[3])
{
    float ut,sint,cost;
    int i;
    if(angle < -0.0010000000620493665f || angle > 0.0010000000620493665f) {
        sint=sinf(angle); cost=cosf(angle);
        for(i=0;i<3;i++) {
            ut=cost*uv[i][0]+sint*uv[i][2]; uv[i][2]=cost*uv[i][2]-sint*uv[i][0]; uv[i][0]=ut;
        }
    }
}
void WPitchUV(float angle, float (*uv)[3])
{
    float ut,sint,cost;
    int i;
    if(angle < -0.0010000000620493665f || angle > 0.0010000000620493665f) {
        sint=sinf(angle); cost=cosf(angle);
        for(i=0;i<3;i++) {
            ut=cost*uv[i][1]-sint*uv[i][2]; uv[i][2]=cost*uv[i][2]+sint*uv[i][1]; uv[i][1]=ut;
        }
    }
}
void WRollUV(float angle, float (*uv)[3])
{
    float ut,sint,cost;
    int i;
    if(angle < -0.0010000000620493665f || angle > 0.0010000000620493665f) {
        sint=sinf(angle); cost=cosf(angle);
        for(i=0;i<3;i++) {
            ut=cost*uv[i][0]-sint*uv[i][1]; uv[i][1]=cost*uv[i][1]+sint*uv[i][0]; uv[i][0]=ut;
        }
    }
}
void RotateUV(float *pyr, float (*uvs)[3])
{ YawUV(pyr[1], uvs); PitchUV(pyr[0], uvs); RollUV(pyr[2], uvs); }

void YawUV(float angle, float (*uv)[3])
{
    float ut,sint,cost;
    int i;
    if(angle < -9.9999998383282218e-05f || angle > 9.9999998383282218e-05f) {
        sint=sinf(angle); cost=cosf(angle);
        for(i=0;i<3;i++) {
            ut=cost*uv[0][i]+sint*uv[2][i]; uv[2][i]=cost*uv[2][i]-sint*uv[0][i]; uv[0][i]=ut;
        }
    }
}
void PitchUV(float angle, float (*uv)[3])
{
    float ut,sint,cost;
    int i;
    if(angle < -9.9999998383282218e-05f || angle > 9.9999998383282218e-05f) {
        sint=sinf(angle); cost=cosf(angle);
        for(i=0;i<3;i++) {
            ut=cost*uv[1][i]-sint*uv[2][i]; uv[2][i]=cost*uv[2][i]+sint*uv[1][i]; uv[1][i]=ut;
        }
    }
}
void RollUV(float angle, float (*uv)[3])
{
    float ut,sint,cost;
    int i;
    if(angle < -9.9999998383282218e-05f || angle > 9.9999998383282218e-05f) {
        sint=sinf(angle); cost=cosf(angle);
        for(i=0;i<3;i++) {
            ut=cost*uv[0][i]-sint*uv[1][i]; uv[1][i]=cost*uv[1][i]+sint*uv[0][i]; uv[0][i]=ut;
        }
    }
}
void fpitch(float sint, float cost, float (*uv)[3])
{
    float ut;
    ut = uv[1][0] * cost - uv[2][0] * sint;
    uv[2][0] = uv[2][0] * cost + uv[1][0] * sint;
    uv[1][0] = ut;
    ut = uv[1][1] * cost - uv[2][1] * sint;
    uv[2][1] = uv[2][1] * cost + uv[1][1] * sint;
    uv[1][1] = ut;
    ut = uv[1][2] * cost - uv[2][2] * sint;
    uv[2][2] = uv[2][2] * cost + uv[1][2] * sint;
    uv[1][2] = ut;
}
void fyaw(float sint, float cost, float (*uv)[3])
{
    float ut;
    ut = uv[0][0] * cost + uv[2][0] * sint;
    uv[2][0] = uv[2][0] * cost - uv[0][0] * sint;
    uv[0][0] = ut;
    ut = uv[0][1] * cost + uv[2][1] * sint;
    uv[2][1] = uv[2][1] * cost - uv[0][1] * sint;
    uv[0][1] = ut;
    ut = uv[0][2] * cost + uv[2][2] * sint;
    uv[2][2] = uv[2][2] * cost - uv[0][2] * sint;
    uv[0][2] = ut;
}
void froll(float sint, float cost, float (*uv)[3])
{
    float ut;
    ut = uv[0][0] * cost - uv[1][0] * sint;
    uv[1][0] = uv[1][0] * cost + uv[0][0] * sint;
    uv[0][0] = ut;
    ut = uv[0][1] * cost - uv[1][1] * sint;
    uv[1][1] = uv[1][1] * cost + uv[0][1] * sint;
    uv[0][1] = ut;
    ut = uv[0][2] * cost - uv[1][2] * sint;
    uv[1][2] = uv[1][2] * cost + uv[0][2] * sint;
    uv[0][2] = ut;
}
void MulUVS(float (*B)[3], float (*A)[3], float (*C)[3])
{
    int i;
    for (i = 0; i < 3; i++) {
        C[i][0] = B[0][0] * A[i][0] + B[1][0] * A[i][1] + B[2][0] * A[i][2];
        C[i][1] = B[0][1] * A[i][0] + B[1][1] * A[i][1] + B[2][1] * A[i][2];
        C[i][2] = B[0][2] * A[i][0] + B[1][2] * A[i][1] + B[2][2] * A[i][2];
    }
}
void BodyVector(float *src, float *dst, float (*uvs)[3])
{
    dst[0] = src[0] * uvs[0][0] + src[1] * uvs[0][1] + src[2] * uvs[0][2];
    dst[1] = src[0] * uvs[1][0] + src[1] * uvs[1][1] + src[2] * uvs[1][2];
    dst[2] = src[0] * uvs[2][0] + src[1] * uvs[2][1] + src[2] * uvs[2][2];
}
float BodyY(float *src, float (*uvs)[3])
{
    return src[0]*uvs[1][0]+src[1]*uvs[1][1]+src[2]*uvs[1][2];
}
void WorldVector(float *src, float *dst, float (*uvs)[3])
{
    dst[0] = src[0] * uvs[0][0] + src[1] * uvs[1][0] + src[2] * uvs[2][0];
    dst[1] = src[0] * uvs[0][1] + src[1] * uvs[1][1] + src[2] * uvs[2][1];
    dst[2] = src[0] * uvs[0][2] + src[1] * uvs[1][2] + src[2] * uvs[2][2];
}
void U64WorldVector(float *bodvec, float *uvec, float (*uvs)[3])
{
    uvec[0] = bodvec[0]*uvs[0][0] - bodvec[1]*uvs[1][0] - bodvec[2]*uvs[2][0];
    uvec[1] = -bodvec[0]*uvs[0][1] + bodvec[1]*uvs[1][1] + bodvec[2]*uvs[2][1];
    uvec[2] = -bodvec[0]*uvs[0][2] + bodvec[1]*uvs[1][2] + bodvec[2]*uvs[2][2];
}
float NormalVector(float *v)
{
    float mag = sqrtf(v[2]*v[2]+v[0]*v[0]+v[1]*v[1]);
    float invmag;
    if (mag <= 9.9999998610655894e-06f) return 1.7516230804060213e-46f;
    invmag = 1.0f / mag;
    v[0] *= invmag; v[1] *= invmag; v[2] *= invmag;
    return mag;
}



void CreateYPRMatrix(float (*mat)[3], float *pyr)
{
    float cp,sp,cy,sy,cr,sr;
    float temp;
    cp = cosf(pyr[0]);
    sp = sinf(pyr[0]);
    cy = cosf(pyr[1]);
    sy = sinf(pyr[1]);
    cr = cosf(pyr[2]);
    sr = sinf(pyr[2]);
    temp = sy * sp;
    mat[0][0] = cy*cr - temp*sr;
    mat[1][0] = cy*sr + temp*cr;
    mat[2][0] = -sy*cp;
    mat[0][1] = -cp*sr;
    mat[1][1] = cp*cr;
    mat[2][1] = sp;
    temp = cy*sp;
    mat[0][2] = sy*cr + temp*sr;
    mat[1][2] = sy*sr - temp*cr;
    mat[2][2] = cy*cp;
}

void CreateQuatMat(float *mat, float *q, int normal)
{
    float s, xs, ys, zs, Nq, xx, yy, zz, xy, wz, yz, wx, xz, wy;
    if (!normal) {
        Nq = q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3];
        s = Nq > 1.7516230804060213e-46f ? 2.0f/Nq : 1.7516230804060213e-46f;
    } else s = 2.0f;
    xs = q[0]*s; ys=q[1]*s; zs=q[2]*s;
    xx=q[0]*xs; yy=q[1]*ys; zz=q[2]*zs;
    mat[0]=1.0f-yy-zz; mat[4]=1.0f-xx-zz; mat[8]=1.0f-xx-yy;
    xy=q[0]*ys; wz=q[3]*zs;
    mat[1]=-xy-wz; mat[3]=wz-xy;
    yz=q[1]*zs; wx=q[3]*xs;
    mat[5]=-yz-wx; mat[7]=wx-yz;
    xz=q[0]*zs; wy=q[3]*ys;
    mat[2]=xz-wy; mat[6]=xz+wy;
}

void InterpQuats(float frac, float *q1, float *q2, float *qr)
{
    float dp, theta, isint, X1, X2;
    int i;
    if (frac < 0.0010000000620493665f) {
        qr[0]=q1[0]; qr[1]=q1[1]; qr[2]=q1[2]; qr[3]=q1[3];
    } else if (frac > 0.99900002032518387f) {
        qr[0]=q2[0]; qr[1]=q2[1]; qr[2]=q2[2]; qr[3]=q2[3];
    } else {
        dp=0.99900002032518387f*(q1[0]*q2[0]+q1[1]*q2[1]+q1[2]*q2[2]+q1[3]*q2[3]);
        if (dp < 1.7516230804060213e-46f) {
            dp=-dp;
            qr[0]=-q2[0]; qr[1]=-q2[1]; qr[2]=-q2[2]; qr[3]=-q2[3];
        } else {
            qr[0]=q2[0]; qr[1]=q2[1]; qr[2]=q2[2]; qr[3]=q2[3];
        }
        if (dp < 0.98000002652406693f) {
            theta=facos(dp);
            isint=1.0f/(float)sinf(theta);
            X1=isint*(float)sinf((1.0f-frac)*theta);
            X2=isint*(float)sinf(frac*theta);
            qr[0]=X1*q1[0]+X2*qr[0]; qr[1]=X1*q1[1]+X2*qr[1];
            qr[2]=X1*q1[2]+X2*qr[2]; qr[3]=X1*q1[3]+X2*qr[3];
        } else {
            for (i=0;i<4;i++) {
                float delta=qr[i]-q1[i];
                if (delta > 1.0f) delta-=2.0f;
                if (delta < -1.0f) delta+=2.0f;
                qr[i]=q1[i]+frac*delta;
                if (qr[i] > 1.0f) qr[i]-=2.0f;
                else if (qr[i] < -1.0f) qr[i]+=2.0f;
            }
        }
    }
}

void ExtractYaw(float (*mat)[3], float *yaw)
{
    if (fabsf(mat[2][0]) < 1.0000000116860974e-06f && fabsf(mat[2][2]) < 1.0000000116860974e-06f)
        *yaw=fatan2(mat[0][2],mat[0][0]);
    else *yaw=fatan2(-mat[2][0],mat[2][2]);
}

float Random(float max)
{
    return (rand() & 0x7fff) * max / 32768.0f;
}
