#include "tdm_terrain.h"
#include "tdm_math.h"

#include <math.h>

void tdm_terrain_init(Terrain *t, float seed)
{
    t->seed = seed;
}

float tdm_terrain_height(const Terrain *t, float x, float z)
{
    float roadNS = expf(-(x * x) / 25.0f);
    float roadEW = expf(-(z * z) / 16.0f);
    float flat = roadNS > roadEW ? roadNS : roadEW;
    float n = sinf(x * 0.1f + t->seed) * cosf(z * 0.1f + t->seed) * 1.5f;
    n += sinf(x * 0.04f) * cosf(z * 0.04f) * 3.2f;
    n *= 1.0f - flat * 0.9f;

    float dx = x - TDM_LAKE_X, dz = z - TDM_LAKE_Z;
    float lf = expf(-(dx * dx + dz * dz) / (TDM_LAKE_R * TDM_LAKE_R));
    return tdm_lerpf(n, TDM_LAKE_H, lf * 0.95f);
}

int tdm_terrain_is_road(const Terrain *t, float x, float z)
{
    (void)t;
    return fabsf(x) < 5.0f || fabsf(z) < 4.0f;
}

int tdm_terrain_is_lake(const Terrain *t, float x, float z, float pad)
{
    (void)t;
    float dx = x - TDM_LAKE_X, dz = z - TDM_LAKE_Z;
    float r = TDM_LAKE_R + pad;
    return (dx * dx + dz * dz) < r * r;
}
