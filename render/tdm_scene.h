#pragma once

#include <stdint.h>

#include "tdm_entity.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_types.h"

typedef struct Game Game;
typedef struct Renderer Renderer;

/* Vertex sink used by every geometry helper. */
typedef struct Geo {
    Vtx *v;
    int *n;
    int max;
} Geo;

uint32_t tdm_geo_shade(uint32_t tint, Vec3 n);
void tdm_geo_vert(Geo *g, Vec3 p, uint32_t col);
Vec3 tdm_geo_rot_y(Vec3 p, float cs, float sn);
void tdm_geo_quad(Geo *g, Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 normal,
                  uint32_t tint);
void tdm_geo_box(Geo *g, Vec3 c, Vec3 half, float yaw, uint32_t tint);
void tdm_geo_billboard(Geo *g, Vec3 c, float size, uint32_t tint,
                       Vec3 right, Vec3 up);
void tdm_geo_strip(Geo *g, Vec3 a, Vec3 b, float width, uint32_t tint);

void tdm_scene_static(Geo *g, Game *game);
void tdm_scene_dynamic(Geo *g, Game *game);
void tdm_scene_terrain(Geo *g, Game *game);
void tdm_scene_entities(Geo *g, Game *game);
void tdm_scene_fx(Geo *g, Game *game);
void tdm_scene_viewmodel(Geo *g, Game *game);
Mat4 tdm_scene_rot_y(float a);
Mat4 tdm_scene_rot_x(float a);
void tdm_scene_bot(Geo *g, const Bot *b, float t);
void tdm_scene_vehicle(Geo *g, const Vehicle *v);
uint32_t tdm_ground_tint(const Game *g, float x, float z, float h, Vec3 n);

/* Fills the renderer's buffers; called once per frame. */
void tdm_scene_render(Renderer *r, Game *game);
void tdm_scene_build_static(Renderer *r, Game *game);
