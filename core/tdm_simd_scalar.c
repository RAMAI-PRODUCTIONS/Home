#include "tdm_simd.h"

void tdm_mat4_mul_scalar(const float *a, const float *b, float *out)
{
    int c, r, k;
    for (c = 0; c < 4; c++)
        for (r = 0; r < 4; r++) {
            float s = 0.0f;
            for (k = 0; k < 4; k++) s += a[k * 4 + r] * b[c * 4 + k];
            out[c * 4 + r] = s;
        }
}

void tdm_vec3_transform_batch_scalar(const float *m, const float *in, float *out,
                                     unsigned count, unsigned inStride,
                                     unsigned outStride)
{
    unsigned i;
    for (i = 0; i < count; i++) {
        const float *s = in + i * inStride;
        float *d = out + i * outStride;
        float x = s[0], y = s[1], z = s[2];
        d[0] = m[0] * x + m[4] * y + m[8] * z + m[12];
        d[1] = m[1] * x + m[5] * y + m[9] * z + m[13];
        d[2] = m[2] * x + m[6] * y + m[10] * z + m[14];
        d[3] = s[3];
    }
}

void tdm_particle_step_scalar(float *posVel, unsigned count,
                              float dt, float gravity)
{
    unsigned i;
    for (i = 0; i < count; i++, posVel += 6) {
        posVel[4] -= gravity * dt;
        posVel[0] += posVel[3] * dt;
        posVel[1] += posVel[4] * dt;
        posVel[2] += posVel[5] * dt;
    }
}
