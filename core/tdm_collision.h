#pragma once

#include "tdm_config.h"
#include "tdm_types.h"

/* Static world: axis-aligned boxes with a type tag, plus the rectangle list
   the minimap needs. Built once per map, only read during play. */

typedef struct World {
    Aabb boxes[TDM_MAX_BOXES];
    int count;
    MapRect rects[TDM_MAX_RECTS];
    int rectCount;
} World;

void tdm_world_reset(World *w);
Aabb *tdm_world_add(World *w, Vec3 mn, Vec3 mx, int type);

/* Circle sweep. h < 0 disables the vertical test (pure 2D). Boxes whose
   top is below y + 0.4 are stepped over, matching the prototype. */
int tdm_world_blocked(const World *w, float x, float z, float r, float y, float h);

RayHit tdm_world_ray(const World *w, Vec3 o, Vec3 d, float maxDist);
int tdm_world_los(const World *w, Vec3 a, Vec3 b);
