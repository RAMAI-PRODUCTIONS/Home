#include "tdm_collision.h"
#include "tdm_math.h"

#include <math.h>
#include <string.h>

void tdm_world_reset(World *w)
{
    memset(w, 0, sizeof *w);
}

Aabb *tdm_world_add(World *w, Vec3 mn, Vec3 mx, int type)
{
    if (w->count >= TDM_MAX_BOXES) return &w->boxes[TDM_MAX_BOXES - 1];
    Aabb *b = &w->boxes[w->count++];
    b->min = mn;
    b->max = mx;
    b->type = type;
    return b;
}

static int circle_hits(const Aabb *b, float x, float z, float r, float y, float h)
{
    if (h >= 0.0f) {
        if (y + h < b->min.y) return 0;
        if (b->max.y < y + 0.4f) return 0;
    }
    float cx = tdm_clampf(x, b->min.x, b->max.x);
    float cz = tdm_clampf(z, b->min.z, b->max.z);
    float dx = x - cx, dz = z - cz;
    return dx * dx + dz * dz < r * r;
}

int tdm_world_blocked(const World *w, float x, float z, float r, float y, float h)
{
    int i;
    /* The arena edge is a wall: keeps actors and the path grid inside. */
    if (fabsf(x) + r > TDM_HALF || fabsf(z) + r > TDM_HALF) return 1;
    for (i = 0; i < w->count; i++)
        if (circle_hits(&w->boxes[i], x, z, r, y, h)) return 1;
    return 0;
}
