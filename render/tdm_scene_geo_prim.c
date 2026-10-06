#include "tdm_scene.h"

#include <math.h>

#include "tdm_math.h"

void tdm_geo_quad(Geo *g, Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 normal,
                  uint32_t tint)
{
    uint32_t col = tdm_geo_shade(tint, normal);
    tdm_geo_vert(g, a, col);
    tdm_geo_vert(g, b, col);
    tdm_geo_vert(g, c, col);
    tdm_geo_vert(g, a, col);
    tdm_geo_vert(g, c, col);
    tdm_geo_vert(g, d, col);
}

void tdm_geo_billboard(Geo *g, Vec3 c, float size, uint32_t tint,
                       Vec3 right, Vec3 up)
{
    Vec3 r = v3_scale(right, size * 0.5f);
    Vec3 u = v3_scale(up, size * 0.5f);
    Vec3 n = v3_norm(v3_cross(r, u));
    tdm_geo_quad(g, v3_add(v3_sub(c, r), u), v3_add(v3_add(c, r), u),
                 v3_sub(v3_sub(c, r), u), v3_sub(v3_add(c, r), u), n, tint);
}

void tdm_geo_strip(Geo *g, Vec3 a, Vec3 b, float width, uint32_t tint)
{
    Vec3 axis = v3_sub(b, a);
    Vec3 side, n;
    float len = v3_len(axis);
    if (len < 0.001f) return;
    axis = v3_scale(axis, 1.0f / len);
    side = v3_cross(axis, v3(0, 1, 0));
    if (v3_len(side) < 0.01f) side = v3_cross(axis, v3(1, 0, 0));
    side = v3_norm(v3_scale(side, width * 0.5f));
    n = v3_norm(v3_cross(axis, side));
    tdm_geo_quad(g, v3_sub(a, side), v3_add(a, side),
                 v3_add(b, side), v3_sub(b, side), n, tint);
}
