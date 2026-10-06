#include "tdm_scene.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_map.h"
#include "tdm_math.h"
#include "tdm_terrain.h"

#define SEG 64

static Vec3 normal_at(const Game *g, float x, float z)
{
    float e = 2.0f;
    float hl = tdm_terrain_height(&g->terrain, x - e, z);
    float hr = tdm_terrain_height(&g->terrain, x + e, z);
    float hd = tdm_terrain_height(&g->terrain, x, z - e);
    float hu = tdm_terrain_height(&g->terrain, x, z + e);
    return v3_norm(v3(hl - hr, 2.0f * e, hd - hu));
}

void tdm_scene_terrain(Geo *g, Game *game)
{
    float step = TDM_MAP_SIZE / (float)SEG;
    float x, z;
    for (z = -TDM_HALF; z <= TDM_HALF + 0.5f; z += step)
        for (x = -TDM_HALF; x <= TDM_HALF + 0.5f; x += step) {
            float h00 = tdm_terrain_height(&game->terrain, x, z);
            float h10 = tdm_terrain_height(&game->terrain, x + step, z);
            float h01 = tdm_terrain_height(&game->terrain, x, z + step);
            float h11 = tdm_terrain_height(&game->terrain, x + step, z + step);
            Vec3 n = normal_at(game, x + step * 0.5f, z + step * 0.5f);
            tdm_geo_quad(g, v3(x, h00, z), v3(x + step, h10, z),
                         v3(x + step, h11, z + step), v3(x, h01, z + step), n,
                         tdm_ground_tint(game, x + step * 0.5f, z + step * 0.5f,
                                         (h00 + h11) * 0.5f, n));
        }
    tdm_geo_quad(g,
                 v3(TDM_LAKE_X - TDM_LAKE_R, TDM_LAKE_H + 0.08f, TDM_LAKE_Z - TDM_LAKE_R),
                 v3(TDM_LAKE_X + TDM_LAKE_R, TDM_LAKE_H + 0.08f, TDM_LAKE_Z - TDM_LAKE_R),
                 v3(TDM_LAKE_X + TDM_LAKE_R, TDM_LAKE_H + 0.08f, TDM_LAKE_Z + TDM_LAKE_R),
                 v3(TDM_LAKE_X - TDM_LAKE_R, TDM_LAKE_H + 0.08f, TDM_LAKE_Z + TDM_LAKE_R),
                 v3(0, 1, 0), 0xFF2A5A70u);
}
