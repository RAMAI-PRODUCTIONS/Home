#pragma once

#include "tdm_types.h"

/* Column-major 4x4, matching GLSL std140 push constants exactly. */
typedef struct Mat4 {
    float m[16];
} Mat4;

static inline float tdm_clampf(float v, float a, float b)
{
    return v < a ? a : (v > b ? b : v);
}

static inline float tdm_lerpf(float a, float b, float t)
{
    return a + (b - a) * t;
}

static inline Vec3 v3(float x, float y, float z)
{
    Vec3 r = { x, y, z };
    return r;
}

static inline Vec3 v3_add(Vec3 a, Vec3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline Vec3 v3_sub(Vec3 a, Vec3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
static inline Vec3 v3_scale(Vec3 a, float s) { return v3(a.x * s, a.y * s, a.z * s); }
static inline float v3_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vec3 v3_cross(Vec3 a, Vec3 b);
float v3_len(Vec3 a);
Vec3 v3_norm(Vec3 a);
float tdm_dist2d(Vec3 a, Vec3 b);

Mat4 m4_identity(void);
Mat4 m4_mul(const Mat4 *a, const Mat4 *b);
Mat4 m4_perspective(float fovy, float aspect, float zn, float zf);
Mat4 m4_view(Vec3 eye, float yaw, float pitch, float roll);
