#include "tdm_math.h"
#include "tdm_simd.h"

#include <math.h>

Vec3 v3_cross(Vec3 a, Vec3 b)
{
    return v3(a.y * b.z - a.z * b.y,
              a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

float v3_len(Vec3 a)
{
    return sqrtf(v3_dot(a, a));
}

Vec3 v3_norm(Vec3 a)
{
    float l = v3_len(a);
    return l > 1e-6f ? v3_scale(a, 1.0f / l) : v3(0, 0, 0);
}

float tdm_dist2d(Vec3 a, Vec3 b)
{
    float dx = a.x - b.x, dz = a.z - b.z;
    return sqrtf(dx * dx + dz * dz);
}
