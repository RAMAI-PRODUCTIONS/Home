#include "tdm_map.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_rng.h"

static void skyscraper(Game *g)
{
    float baseY = tdm_terrain_height(&g->terrain, 0.0f, 0.0f);
    int i;
    for (i = 0; i < 4; i++) {
        float size = 24.0f - (float)i * 5.0f;
        float h = 7.0f;
        float y = baseY + (float)i * h + h * 0.5f;
        tdm_map_box(g, 0, y, 0, size, h, size, TDM_T_BUILDING);
        if (i < 3) tdm_map_steps(g, 0, size * 0.5f + 2.0f, 4, h, 6, 8, TDM_T_STAIR);
    }
}

void tdm_map_urban(Game *g)
{
    int i;
    for (i = 0; i < 9; i++) {
        int j;
        for (j = 0; j < 9; j++) {
            float x = -100.0f + (float)i * 25.0f;
            float z = -100.0f + (float)j * 25.0f;
            float w, d, h, px, pz;
            if (fabsf(x) < 25.0f && fabsf(z) < 25.0f) continue;
            if (tdm_rng_f(&g->rng) > 0.7f) continue;
            w = 12.0f + tdm_rng_f(&g->rng) * 8.0f;
            d = 12.0f + tdm_rng_f(&g->rng) * 8.0f;
            h = 10.0f + tdm_rng_f(&g->rng) * 20.0f;
            px = x + tdm_rng_range(&g->rng, -3.0f, 3.0f);
            pz = z + tdm_rng_range(&g->rng, -3.0f, 3.0f);
            tdm_map_box(g, px, tdm_terrain_height(&g->terrain, px, pz) + h * 0.5f,
                        pz, w, h, d, TDM_T_BUILDING);
        }
    }
    for (i = 0; i < 30; i++) {
        float x = tdm_rng_range(&g->rng, -110.0f, 110.0f);
        float z = tdm_rng_range(&g->rng, -110.0f, 110.0f);
        if (fabsf(x) < 20.0f && fabsf(z) < 20.0f) continue;
        tdm_map_box(g, x, tdm_terrain_height(&g->terrain, x, z) + 1.0f,
                    z, 4, 2, 2, TDM_T_CAR);
    }
    skyscraper(g);
}
