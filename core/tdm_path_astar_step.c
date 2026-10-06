#include "tdm_pathfind.h"

#include <math.h>

#include "tdm_math.h"

float tdm_astar_h(int idx, int ex, int ez)
{
    float dx = (float)(idx % TDM_PATH_DIM - ex);
    float dz = (float)(idx / TDM_PATH_DIM - ez);
    return sqrtf(dx * dx + dz * dz);
}

/* Returns the improved node index, or -1 when nothing changed. The caller
   owns the open list so the search state stays in one translation unit. */
int tdm_astar_relax(PathGrid *g, int cur, int i, int cx, int cz)
{
    static const int8_t DX[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };
    static const int8_t DZ[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    int dx = DX[i], dz = DZ[i];
    int nx = cx + dx, nz = cz + dz;
    int ni;
    float step = (dx && dz) ? 1.41421f : 1.0f, ng;

    if (!tdm_path_walkable(g, nx, nz)) return -1;
    if (dx && dz && (!tdm_path_walkable(g, cx + dx, cz) ||
                     !tdm_path_walkable(g, cx, cz + dz))) return -1;
    ni = nz * TDM_PATH_DIM + nx;
    if (g->closed[ni]) return -1;
    ng = g->cost[cur] + step;
    if (ng >= g->cost[ni]) return -1;
    g->cost[ni] = ng;
    g->parent[ni] = (int16_t)cur;
    return ni;
}
