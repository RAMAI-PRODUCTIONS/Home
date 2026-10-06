#include "tdm_collision.h"
#include "tdm_math.h"

#include <math.h>
#include <string.h>

static RayHit ray_aabb(Vec3 o, Vec3 d, const Aabb *b, float maxDist)
{
    RayHit h = { 0, maxDist, { 0, 0, 0 }, b->type };
    float tmin = 0.0f, tmax = maxDist;
    const float *op = &o.x, *dp = &d.x;
    const float *mn = &b->min.x, *mx = &b->max.x;
    int i;
    for (i = 0; i < 3; i++) {
        if (fabsf(dp[i]) < 1e-8f) {
            if (op[i] < mn[i] || op[i] > mx[i]) return h;
            continue;
        }
        float inv = 1.0f / dp[i];
        float t0 = (mn[i] - op[i]) * inv;
        float t1 = (mx[i] - op[i]) * inv;
        if (t0 > t1) { float tmp = t0; t0 = t1; t1 = tmp; }
        if (t0 > tmin) tmin = t0;
        if (t1 < tmax) tmax = t1;
        if (tmin > tmax) return h;
    }
    h.hit = 1;
    h.t = tmin;
    h.point = v3_add(o, v3_scale(d, tmin));
    return h;
}

RayHit tdm_world_ray(const World *w, Vec3 o, Vec3 d, float maxDist)
{
    RayHit best = { 0, maxDist, { 0, 0, 0 }, 0 };
    int i;
    for (i = 0; i < w->count; i++) {
        RayHit h = ray_aabb(o, d, &w->boxes[i], best.t);
        if (h.hit && h.t < best.t) best = h;
    }
    return best;
}

int tdm_world_los(const World *w, Vec3 a, Vec3 b)
{
    Vec3 d = v3_sub(b, a);
    float len = v3_len(d);
    if (len < 0.01f) return 1;
    d = v3_scale(d, 1.0f / len);
    return !tdm_world_ray(w, a, d, len - 0.2f).hit;
}
