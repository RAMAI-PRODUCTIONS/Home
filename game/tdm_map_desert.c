#include "tdm_map.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_rng.h"

static void radar_tower(Game *g)
{
    float baseY = tdm_terrain_height(&g->terrain, 0.0f, 0.0f);
    tdm_map_box(g, 0, baseY + 1.0f, 0, 10, 2, 10, TDM_T_BUILDING);
    tdm_map_box(g, 0, baseY + 13.5f, 0, 3, 25, 3, TDM_T_BUILDING);
    tdm_map_steps(g, 6, 0, 3, 25, 12, 20, TDM_T_STAIR);
    tdm_map_steps(g, -6, 0, 3, 25, 12, 20, TDM_T_STAIR);
}

void tdm_map_desert(Game *g)
{
    int i;
    for (i = 0; i < 20; i++) {
        float w = tdm_rng_range(&g->rng, 6.0f, 12.0f);
        float h = tdm_rng_range(&g->rng, 4.0f, 8.0f);
        float d = tdm_rng_range(&g->rng, 6.0f, 12.0f);
        float x = tdm_rng_range(&g->rng, -110.0f, 110.0f);
        float z = tdm_rng_range(&g->rng, -110.0f, 110.0f);
        if (fabsf(x) < 25.0f && fabsf(z) < 25.0f) continue;
        tdm_map_box(g, x, tdm_terrain_height(&g->terrain, x, z) + h * 0.5f,
                    z, w, h, d, TDM_T_BUILDING);
    }
    for (i = 0; i < 8; i++) {
        float ang = (float)i * 0.78539816f;
        float dist = 40.0f + tdm_rng_f(&g->rng) * 30.0f;
        float x = cosf(ang) * dist;
        float z = sinf(ang) * dist;
        tdm_map_box(g, x, tdm_terrain_height(&g->terrain, x, z) + 1.0f,
                    z, 12, 2, 2, TDM_T_WALL);
    }
    radar_tower(g);
}
