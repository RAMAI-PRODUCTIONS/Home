#include "tdm_scene.h"

#include <math.h>

#include "tdm_math.h"
#include "tdm_props.h"

static Vec3 place(Vec3 o, float yaw, float ox, float oy, float oz)
{
    float cs = cosf(yaw), sn = sinf(yaw);
    return v3(o.x + ox * cs + oz * sn, o.y + oy, o.z - ox * sn + oz * cs);
}

void tdm_scene_bot(Geo *g, const Bot *b, float t)
{
    uint32_t col = b->e.team == 0 ? 0x3366FFu : 0xFF3333u;
    uint32_t dark = 0x222222u;
    float swing = b->moving ? sinf(t * 9.0f) * 0.22f : 0.0f;
    Vec3 p = b->e.pos;
    float y = b->e.yaw;

    tdm_geo_box(g, place(p, y, 0, 0.9f, 0), v3(0.35f, 0.52f, 0.3f), y, col);
    tdm_geo_box(g, place(p, y, 0, 1.6f, 0), v3(0.26f, 0.26f, 0.26f), y, 0xD8B088u);
    tdm_geo_box(g, place(p, y, 0, 1.72f, 0), v3(0.3f, 0.14f, 0.3f), y, col);
    tdm_geo_box(g, place(p, y, 0.32f, 1.05f, -0.35f), v3(0.07f, 0.07f, 0.35f), y, dark);
    tdm_geo_box(g, place(p, y, 0.17f, 0.35f + swing, 0.1f), v3(0.09f, 0.35f, 0.09f), y, dark);
    tdm_geo_box(g, place(p, y, -0.17f, 0.35f - swing, 0.1f), v3(0.09f, 0.35f, 0.09f), y, dark);
}

void tdm_scene_vehicle(Geo *g, const Vehicle *v)
{
    uint32_t body = 0x4A5A3Au, dark = 0x1C1C1Cu, wheel = 0x111111u;
    Vec3 p = v->pos;
    float y = v->yaw;
    int i;
    tdm_geo_box(g, place(p, y, 0, 0.8f, 0), v3(1.1f, 0.5f, 1.8f), y, body);
    tdm_geo_box(g, place(p, y, 0, 1.5f, -0.3f), v3(0.9f, 0.35f, 0.8f), y, 0x2A3324u);
    tdm_geo_box(g, place(p, y, 0, 1.9f, -1.6f), v3(0.25f, 0.15f, 0.25f), y, dark);
    tdm_geo_box(g, place(p, y, 0, 1.95f, -2.2f), v3(0.06f, 0.06f, 0.55f), y, dark);
    for (i = 0; i < 4; i++) {
        float sx = (i & 1) ? 1.0f : -1.0f;
        float sz = (i & 2) ? 1.4f : -1.4f;
        tdm_geo_box(g, place(p, y, sx, 0.4f, sz), v3(0.2f, 0.45f, 0.45f), y, wheel);
    }
}
