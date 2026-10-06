#include "tdm_map.h"

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_rng.h"

void tdm_map_box(Game *g, float x, float y, float z,
                 float w, float h, float d, int type)
{
    tdm_world_add(&g->world, v3(x - w * 0.5f, y - h * 0.5f, z - d * 0.5f),
                             v3(x + w * 0.5f, y + h * 0.5f, z + d * 0.5f), type);
    if (type == TDM_T_VISUAL || type == TDM_T_TREE) return;
    if (g->world.rectCount >= TDM_MAX_RECTS) return;
    {
        MapRect *r = &g->world.rects[g->world.rectCount++];
        r->x = x;
        r->z = z;
        r->w = w;
        r->d = d;
        r->type = type;
    }
}

void tdm_map_steps(Game *g, float cx, float cz, float w,
                   float totalH, float totalD, int steps, int type)
{
    float stepH = totalH / (float)steps;
    float stepD = totalD / (float)steps;
    float baseY = tdm_terrain_height(&g->terrain, cx, cz);
    int i;
    for (i = 0; i < steps; i++)
        tdm_map_box(g, cx, baseY + (float)i * stepH + stepH * 0.5f,
                    cz + (float)i * stepD - totalD * 0.5f, w, stepH, stepD, type);
}

void tdm_map_tree(Game *g, float x, float z, float trunkH, float crownH)
{
    float y = tdm_terrain_height(&g->terrain, x, z);
    tdm_map_box(g, x, y + trunkH * 0.5f, z, 0.8f, trunkH, 0.8f, TDM_T_TREE);
    tdm_map_box(g, x, y + trunkH + crownH * 0.5f, z, 3.2f, crownH, 3.2f, TDM_T_VISUAL);
}
