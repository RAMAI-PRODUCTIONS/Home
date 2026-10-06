#include "tdm_pathfind.h"

#include <math.h>

#include "tdm_math.h"
#include "tdm_rng.h"

void tdm_path_bake(PathGrid *g, const World *w)
{
    int x, z;
    for (z = 0; z < TDM_PATH_DIM; z++)
        for (x = 0; x < TDM_PATH_DIM; x++) {
            Vec3 p = tdm_path_world(x, z);
            g->blocked[z * TDM_PATH_DIM + x] =
                (unsigned char)tdm_world_blocked(w, p.x, p.z, 1.1f, -1.0f, -1.0f);
        }
}

int tdm_path_walkable(const PathGrid *g, int cx, int cz)
{
    if (cx < 0 || cz < 0 || cx >= TDM_PATH_DIM || cz >= TDM_PATH_DIM) return 0;
    return !g->blocked[cz * TDM_PATH_DIM + cx];
}

void tdm_path_cell(Vec3 world, int *cx, int *cz)
{
    int x = (int)floorf((world.x + TDM_HALF) / TDM_PATH_CELL);
    int z = (int)floorf((world.z + TDM_HALF) / TDM_PATH_CELL);
    *cx = x < 0 ? 0 : (x >= TDM_PATH_DIM ? TDM_PATH_DIM - 1 : x);
    *cz = z < 0 ? 0 : (z >= TDM_PATH_DIM ? TDM_PATH_DIM - 1 : z);
}

Vec3 tdm_path_world(int cx, int cz)
{
    return v3(-TDM_HALF + (cx + 0.5f) * TDM_PATH_CELL, 0.0f,
              -TDM_HALF + (cz + 0.5f) * TDM_PATH_CELL);
}
