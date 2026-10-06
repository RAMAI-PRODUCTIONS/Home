#include "tdm_pathfind.h"

#include <math.h>

#include "tdm_math.h"
#include "tdm_rng.h"

/* Spiral out until a walkable cell is found; callers only use this when
   the requested cell itself is blocked (a building or a tree). */
void tdm_path_nearest(const PathGrid *g, int *cx, int *cz)
{
    int r, dx, dz, found;
    for (r = 1; r < 6; r++) {
        found = 0;
        for (dz = -r; dz <= r && !found; dz++)
            for (dx = -r; dx <= r && !found; dx++)
                if (tdm_path_walkable(g, *cx + dx, *cz + dz)) {
                    *cx += dx;
                    *cz += dz;
                    found = 1;
                }
        if (found) return;
    }
}

void tdm_path_random(const PathGrid *g, uint32_t *seed, Vec3 *out)
{
    int i;
    for (i = 0; i < 100; i++) {
        int cx = tdm_rng_int(seed, TDM_PATH_DIM);
        int cz = tdm_rng_int(seed, TDM_PATH_DIM);
        if (tdm_path_walkable(g, cx, cz)) {
            *out = tdm_path_world(cx, cz);
            return;
        }
    }
    *out = v3(0, 0, 0);
}
