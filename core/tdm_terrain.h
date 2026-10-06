#pragma once

#include "tdm_types.h"

/* Analytic terrain: rolling hills flattened along the two cross roads and
   inside the lake basin. Height is identical on CPU (simulation) and mesh
   generation, so entities never float or sink. */

#define TDM_LAKE_X 75.0f
#define TDM_LAKE_Z -70.0f
#define TDM_LAKE_R 16.0f
#define TDM_LAKE_H -2.4f

typedef struct Terrain {
    float seed;
} Terrain;

void tdm_terrain_init(Terrain *t, float seed);
float tdm_terrain_height(const Terrain *t, float x, float z);
int tdm_terrain_is_road(const Terrain *t, float x, float z);
int tdm_terrain_is_lake(const Terrain *t, float x, float z, float pad);
