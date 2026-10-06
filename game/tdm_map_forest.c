#include "tdm_map.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_rng.h"

static void bunker(Game *g)
{
    float baseY = tdm_terrain_height(&g->terrain, 0.0f, 0.0f);
    tdm_map_box(g, 0, baseY + 3.0f, 0, 26, 6, 26, TDM_T_BUILDING);
    tdm_map_box(g, 0, baseY + 6.75f, 0, 28, 1.5f, 28, TDM_T_WALL);
    tdm_map_box(g, 16, baseY + 2.0f, 0, 2, 4, 12, TDM_T_WALL);
    tdm_map_box(g, -16, baseY + 2.0f, 0, 2, 4, 12, TDM_T_WALL);
    tdm_map_box(g, 0, baseY + 2.0f, 16, 12, 4, 2, TDM_T_WALL);
    tdm_map_box(g, 0, baseY + 2.0f, -16, 12, 4, 2, TDM_T_WALL);
    tdm_map_steps(g, 0, 14, 6, 6, 8, 8, TDM_T_STAIR);
    tdm_map_steps(g, 0, -14, 6, 6, 8, 8, TDM_T_STAIR);
}

void tdm_map_forest(Game *g)
{
    int i;
    for (i = 0; i < 150; i++) {
        float x = tdm_rng_range(&g->rng, -125.0f, 125.0f);
        float z = tdm_rng_range(&g->rng, -125.0f, 125.0f);
        if (fabsf(x) < 25.0f && fabsf(z) < 25.0f) continue;
        if (tdm_world_blocked(&g->world, x, z, 2.5f, -1.0f, -1.0f)) continue;
        if (tdm_terrain_is_lake(&g->terrain, x, z, 1.0f)) continue;
        tdm_map_tree(g, x, z, 3.4f, 4.0f);
    }
    for (i = 0; i < 20; i++) {
        float x = tdm_rng_range(&g->rng, -100.0f, 100.0f);
        float z = tdm_rng_range(&g->rng, -100.0f, 100.0f);
        float y;
        if (fabsf(x) < 20.0f && fabsf(z) < 20.0f) continue;
        y = tdm_terrain_height(&g->terrain, x, z);
        tdm_map_box(g, x, y + 0.5f, z, 6, 1.2f, 1.2f, TDM_T_TREE);
    }
    bunker(g);
}
