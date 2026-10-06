#include "tdm_pathfind.h"

#include <math.h>

#include "tdm_math.h"

#define OPEN_MAX 4096

static int16_t s_open[OPEN_MAX];
static int s_n;

static int pop_min(const PathGrid *g, int ex, int ez)
{
    int i, bi = 0;
    float bf = 1e30f;
    for (i = 0; i < s_n; i++) {
        int idx = s_open[i];
        float f = g->cost[idx] + tdm_astar_h(idx, ex, ez);
        if (f < bf) { bf = f; bi = i; }
    }
    {
        int pick = s_open[bi];
        s_open[bi] = s_open[--s_n];
        return pick;
    }
}

int tdm_path_find(PathGrid *g, Vec3 from, Vec3 to, Vec3 *out, int maxOut)
{
    int sx, sz, ex, ez, i, limit = 1100, found = 0;
    int start, goal, n = 0, m = 0;
    int chain[TDM_MAX_PATH];

    tdm_path_cell(from, &sx, &sz);
    tdm_path_cell(to, &ex, &ez);
    if (!tdm_path_walkable(g, sx, sz)) tdm_path_nearest(g, &sx, &sz);
    if (!tdm_path_walkable(g, ex, ez)) tdm_path_nearest(g, &ex, &ez);
    if (!tdm_path_walkable(g, sx, sz) || !tdm_path_walkable(g, ex, ez)) return 0;

    for (i = 0; i < TDM_PATH_DIM * TDM_PATH_DIM; i++) {
        g->cost[i] = 1e30f;
        g->closed[i] = 0;
    }
    start = sz * TDM_PATH_DIM + sx;
    goal = ez * TDM_PATH_DIM + ex;
    g->cost[start] = 0.0f;
    g->parent[start] = -1;
    s_open[0] = (int16_t)start;
    s_n = 1;

    while (s_n && limit-- > 0) {
        int cur = pop_min(g, ex, ez);
        if (g->closed[cur]) continue;
        g->closed[cur] = 1;
        if (cur == goal) { found = 1; break; }
        for (i = 0; i < 8; i++) {
            int r = tdm_astar_relax(g, cur, i, cur % TDM_PATH_DIM,
                                    cur / TDM_PATH_DIM);
            if (r >= 0 && s_n < OPEN_MAX) s_open[s_n++] = (int16_t)r;
        }
    }
    if (!found) return 0;

    for (i = goal; i != -1 && n < TDM_MAX_PATH; i = g->parent[i]) chain[n++] = i;
    for (i = n - 1; i >= 0 && m < maxOut; i--)
        out[m++] = tdm_path_world(chain[i] % TDM_PATH_DIM, chain[i] / TDM_PATH_DIM);
    return m;
}
