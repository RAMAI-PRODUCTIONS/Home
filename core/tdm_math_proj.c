#include "tdm_math.h"
#include "tdm_simd.h"

#include <math.h>

Mat4 m4_perspective(float fovy, float aspect, float zn, float zf)
{
    float f = 1.0f / tanf(fovy * 0.5f);
    Mat4 r = {{ 0 }};
    r.m[0] = f / aspect;
    r.m[5] = -f;
    r.m[10] = zf / (zn - zf);
    r.m[11] = -1.0f;
    r.m[14] = (zf * zn) / (zn - zf);
    return r;
}

/* View = Rz(-roll) * Rx(-pitch) * Ry(-yaw) * T(-eye), matching the
   prototype's YXZ camera order plus the death roll. */
Mat4 m4_view(Vec3 eye, float yaw, float pitch, float roll)
{
    float cy = cosf(yaw), sy = sinf(yaw);
    float cp = cosf(pitch), sp = sinf(pitch);
    float cr = cosf(roll), sr = sinf(roll);
    float a00 = cy, a01 = 0.0f, a02 = -sy;
    float a10 = sp * sy, a11 = cp, a12 = sp * cy;
    float a20 = cp * sy, a21 = -sp, a22 = cp * cy;
    float r00 = cr * a00 + sr * a10;
    float r01 = cr * a01 + sr * a11;
    float r02 = cr * a02 + sr * a12;
    float r10 = -sr * a00 + cr * a10;
    float r11 = -sr * a01 + cr * a11;
    float r12 = -sr * a02 + cr * a12;
    Mat4 r = {{ 0 }};
    r.m[0] = r00;  r.m[4] = r01;  r.m[8] = r02;
    r.m[1] = r10;  r.m[5] = r11;  r.m[9] = r12;
    r.m[2] = a20;  r.m[6] = a21;  r.m[10] = a22;
    r.m[12] = -(r00 * eye.x + r01 * eye.y + r02 * eye.z);
    r.m[13] = -(r10 * eye.x + r11 * eye.y + r12 * eye.z);
    r.m[14] = -(a20 * eye.x + a21 * eye.y + a22 * eye.z);
    r.m[15] = 1.0f;
    return r;
}
