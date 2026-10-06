#pragma once

/* Primitive types shared by every module. */

typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Vtx {
    float px, py, pz;
    float u, v;
    float r, g, b, a;
} Vtx;

typedef struct Aabb {
    Vec3 min, max;
    int type; /* 0 solid, 1 tree, 2 building, 3 wall, 4 car, 5 stair */
} Aabb;

typedef struct MapRect {
    float x, z, w, d;
    int type;
} MapRect;

typedef enum Team {
    TDM_TEAM_BLUE = 0,
    TDM_TEAM_RED = 1
} Team;

typedef struct RayHit {
    int hit;
    float t;
    Vec3 point;
    int type;
} RayHit;

#define TDM_SUN_X -0.35f
#define TDM_SUN_Y 0.82f
#define TDM_SUN_Z -0.30f
