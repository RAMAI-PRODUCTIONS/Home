#pragma once

/* Runtime-selected kernels. Game code must call the function pointers,
   never the NEON symbols directly, so x86_64 builds keep working. */

typedef void (*tdm_mat4_mul_fn)(const float *a, const float *b, float *out);

typedef void (*tdm_xform_fn)(const float *m, const float *in, float *out,
                             unsigned count, unsigned inStride, unsigned outStride);

typedef void (*tdm_particle_fn)(float *posVel, unsigned count,
                                float dt, float gravity);

extern tdm_mat4_mul_fn tdm_mat4_mul_impl;
extern tdm_xform_fn tdm_xform_impl;
extern tdm_particle_fn tdm_particle_step_impl;

void tdm_simd_init(void);

/* 1 when every dispatched kernel matches its scalar reference. */
int tdm_simd_selftest(float *worst);

/* Scalar reference implementations (always linked). */
void tdm_mat4_mul_scalar(const float *a, const float *b, float *out);
void tdm_vec3_transform_batch_scalar(const float *m, const float *in, float *out,
                                     unsigned count, unsigned inStride,
                                     unsigned outStride);
void tdm_particle_step_scalar(float *posVel, unsigned count,
                              float dt, float gravity);
