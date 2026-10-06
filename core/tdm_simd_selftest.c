#include "tdm_simd.h"

#include <math.h>
#include <string.h>

#include "tdm_math.h"

static float diff(const float *a, const float *b, int n)
{
    float md = 0.0f;
    int i;
    for (i = 0; i < n; i++) {
        float d = fabsf(a[i] - b[i]);
        if (d > md) md = d;
    }
    return md;
}

/* Compares every dispatched kernel against its scalar reference on a fixed
   input so a bad port shows up at startup instead of as dead geometry. */
int tdm_simd_selftest(float *worst)
{
    Mat4 a = m4_perspective(1.2f, 1.777f, 0.1f, 600.0f);
    Mat4 b = m4_view(v3(1, 2, 3), 0.5f, 0.2f, 0.1f);
    Mat4 rn, rs;
    float vin[16], vn[16], vs[16];
    float pn[12] = { 1, 2, 3, 0.5f, 0.25f, -1, 9, 8, 7, -2, 3, 0.1f };
    float ps[12];
    float md = 0.0f, d;
    int i;

    tdm_mat4_mul_impl(a.m, b.m, rn.m);
    tdm_mat4_mul_scalar(a.m, b.m, rs.m);
    d = diff(rn.m, rs.m, 16);
    if (d > md) md = d;

    for (i = 0; i < 16; i++) vin[i] = (float)(i * 3 - 7) * 0.25f;
    tdm_xform_impl(a.m, vin, vn, 4, 4, 4);
    tdm_vec3_transform_batch_scalar(a.m, vin, vs, 4, 4, 4);
    d = diff(vn, vs, 16);
    if (d > md) md = d;

    memcpy(ps, pn, sizeof pn);
    tdm_particle_step_impl(pn, 2, 0.05f, 9.8f);
    tdm_particle_step_scalar(ps, 2, 0.05f, 9.8f);
    d = diff(pn, ps, 12);
    if (d > md) md = d;

    if (worst) *worst = md;
    return md < 1e-4f;
}
