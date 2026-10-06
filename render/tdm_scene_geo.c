#include "tdm_scene.h"

#include <math.h>

#include "tdm_math.h"

uint32_t tdm_geo_shade(uint32_t tint, Vec3 n)
{
    float ndl = n.x * TDM_SUN_X + n.y * TDM_SUN_Y + n.z * TDM_SUN_Z;
    float lit = 0.42f + 0.58f * (ndl > 0.0f ? ndl : 0.0f);
    float r = (float)((tint >> 16) & 255) * lit;
    float g = (float)((tint >> 8) & 255) * lit;
    float b = (float)(tint & 255) * lit;
    if (r > 255.0f) r = 255.0f;
    if (g > 255.0f) g = 255.0f;
    if (b > 255.0f) b = 255.0f;
    return 0xFF000000u | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

void tdm_geo_vert(Geo *g, Vec3 p, uint32_t col)
{
    Vtx *t;
    if (*g->n >= g->max) return;
    t = &g->v[(*g->n)++];
    t->px = p.x;
    t->py = p.y;
    t->pz = p.z;
    t->u = 0.0f;
    t->v = 0.0f;
    t->r = (float)((col >> 16) & 255) / 255.0f;
    t->g = (float)((col >> 8) & 255) / 255.0f;
    t->b = (float)(col & 255) / 255.0f;
    t->a = 1.0f;
}
