#include "tdm_math.h"
#include "tdm_simd.h"

#include <math.h>

Mat4 m4_identity(void)
{
    Mat4 r = {{ 1, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 1, 0, 0, 0, 0, 1 }};
    return r;
}

Mat4 m4_mul(const Mat4 *a, const Mat4 *b)
{
    Mat4 r;
    tdm_mat4_mul_impl(a->m, b->m, r.m);
    return r;
}

/* Vulkan clip space: depth 0..1 and inverted Y. */