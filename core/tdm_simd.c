#include "tdm_simd.h"

tdm_mat4_mul_fn tdm_mat4_mul_impl;
tdm_xform_fn tdm_xform_impl;
tdm_particle_fn tdm_particle_step_impl;

#if defined(TDM_ENABLE_ASM) && TDM_ENABLE_ASM && defined(__aarch64__)
#define TDM_HAVE_NEON 1
void tdm_mat4_mul_neon(const float *a, const float *b, float *out);
void tdm_vec3_transform_batch_neon(const float *m, const float *in, float *out,
                                   unsigned count, unsigned inStride,
                                   unsigned outStride);
void tdm_particle_step_neon(float *posVel, unsigned count,
                            float dt, float gravity);
#else
#define TDM_HAVE_NEON 0
#endif

void tdm_simd_init(void)
{
    tdm_mat4_mul_impl = tdm_mat4_mul_scalar;
    tdm_xform_impl = tdm_vec3_transform_batch_scalar;
    tdm_particle_step_impl = tdm_particle_step_scalar;
#if TDM_HAVE_NEON
    tdm_mat4_mul_impl = tdm_mat4_mul_neon;
    tdm_xform_impl = tdm_vec3_transform_batch_neon;
    tdm_particle_step_impl = tdm_particle_step_neon;
#endif
}
