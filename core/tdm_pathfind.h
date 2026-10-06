#pragma once

#include <stdint.h>

#include "tdm_collision.h"
#include "tdm_config.h"
#include "tdm_types.h"

/* 2 m navigation grid baked from the collision world. Node arrays are kept
   inside the grid so searches need no heap and no globals. */

typedef struct PathGrid {
    unsigned char blocked[TDM_PATH_DIM * TDM_PATH_DIM];
    unsigned char closed[TDM_PATH_DIM * TDM_PATH_DIM];
    float cost[TDM_PATH_DIM * TDM_PATH_DIM];
    int16_t parent[TDM_PATH_DIM * TDM_PATH_DIM];
} PathGrid;

void tdm_path_bake(PathGrid *g, const World *w);
int tdm_path_walkable(const PathGrid *g, int cx, int cz);
void tdm_path_cell(Vec3 world, int *cx, int *cz);
Vec3 tdm_path_world(int cx, int cz);
void tdm_path_nearest(const PathGrid *g, int *cx, int *cz);
void tdm_path_random(const PathGrid *g, uint32_t *seed, Vec3 *out);

/* A* search. Returns waypoint count written into out (0 = unreachable). */
int tdm_path_find(PathGrid *g, Vec3 from, Vec3 to, Vec3 *out, int maxOut);

/* A* internals, split out to keep every file under the word limit */
float tdm_astar_h(int idx, int ex, int ez);
int tdm_astar_relax(PathGrid *g, int cur, int i, int cx, int cz);
