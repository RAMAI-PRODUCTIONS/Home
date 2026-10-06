#pragma once

/* Procedural maps. Generators only create collision boxes and minimap
   rectangles; the renderer turns boxes into triangles at load time. */

typedef struct Game Game;

#define TDM_T_SOLID 0
#define TDM_T_TREE 1
#define TDM_T_BUILDING 2
#define TDM_T_WALL 3
#define TDM_T_CAR 4
#define TDM_T_STAIR 5
#define TDM_T_VISUAL 6   /* foliage: drawn, never collides */

void tdm_map_build(Game *g, int theme);
void tdm_map_theme(Game *g, int theme);
void tdm_map_spawns(Game *g);
void tdm_map_props(Game *g);

void tdm_map_box(Game *g, float x, float y, float z,
                 float w, float h, float d, int type);
void tdm_map_steps(Game *g, float cx, float cz, float w,
                   float totalH, float totalD, int steps, int type);
void tdm_map_tree(Game *g, float x, float z, float trunkH, float crownH);

void tdm_map_desert(Game *g);
void tdm_map_urban(Game *g);
void tdm_map_forest(Game *g);
