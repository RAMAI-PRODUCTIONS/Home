#include "tdm_scene.h"

#include <math.h>

#include "tdm_math.h"

Vec3 tdm_geo_rot_y(Vec3 p, float cs, float sn)
{
    return v3(p.x * cs + p.z * sn, p.y, -p.x * sn + p.z * cs);
}

void tdm_geo_box(Geo *g, Vec3 c, Vec3 half, float yaw, uint32_t tint)
{
    float cs = cosf(yaw), sn = sinf(yaw);
    Vec3 k[8];
    int i;
    for (i = 0; i < 8; i++) {
        Vec3 l = v3((i & 1 ? 1.0f : -1.0f) * half.x,
                    (i & 2 ? 1.0f : -1.0f) * half.y,
                    (i & 4 ? 1.0f : -1.0f) * half.z);
        k[i] = v3_add(c, tdm_geo_rot_y(l, cs, sn));
    }
    tdm_geo_quad(g, k[2], k[3], k[7], k[6], tdm_geo_rot_y(v3(0, 1, 0), cs, sn), tint);
    tdm_geo_quad(g, k[0], k[1], k[5], k[4], tdm_geo_rot_y(v3(0, -1, 0), cs, sn), tint);
    tdm_geo_quad(g, k[4], k[5], k[7], k[6], tdm_geo_rot_y(v3(0, 0, 1), cs, sn), tint);
    tdm_geo_quad(g, k[0], k[1], k[3], k[2], tdm_geo_rot_y(v3(0, 0, -1), cs, sn), tint);
    tdm_geo_quad(g, k[1], k[5], k[7], k[3], tdm_geo_rot_y(v3(1, 0, 0), cs, sn), tint);
    tdm_geo_quad(g, k[0], k[4], k[6], k[2], tdm_geo_rot_y(v3(-1, 0, 0), cs, sn), tint);
}
