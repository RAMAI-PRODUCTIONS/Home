#include "tdm_hud.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_terrain.h"

static float map_x(float wx, float x, float s)
{
    return x + (wx + TDM_HALF) * (s / TDM_MAP_SIZE);
}

static void dot(DrawCtx *c, float px, float py, float s, uint32_t col)
{
    tdm2_rect(c, px - s, py - s, s * 2.0f, s * 2.0f, col);
}

void tdm_hud_minimap(const Game *g, DrawCtx *c, float x, float y, float s)
{
    int i, uav = g->match.uav > 0.0f;
    const Player *p = &g->player;
    float px, py;

    tdm2_rect(c, x - 2.0f, y - 2.0f, s + 4.0f, s + 4.0f, 0xAAFFFFFFu);
    tdm2_rect(c, x, y, s, s, 0x77001808u);

    tdm2_rect(c, map_x(TDM_LAKE_X - TDM_LAKE_R, x, s),
              map_x(TDM_LAKE_Z - TDM_LAKE_R, y, s),
              TDM_LAKE_R * 2.0f * (s / TDM_MAP_SIZE),
              TDM_LAKE_R * 2.0f * (s / TDM_MAP_SIZE), 0xB03070A0u);

    for (i = 0; i < g->world.rectCount; i++) {
        const MapRect *r = &g->world.rects[i];
        tdm2_rect(c, map_x(r->x - r->w * 0.5f, x, s), map_x(r->z - r->d * 0.5f, y, s),
                  r->w * (s / TDM_MAP_SIZE), r->d * (s / TDM_MAP_SIZE),
                  0xC0A0A0A0u);
    }
    for (i = 0; i < TDM_MAX_TURRETS; i++)
        if (g->turrets[i].pos.x != 0.0f || g->turrets[i].pos.z != 0.0f)
            dot(c, map_x(g->turrets[i].pos.x, x, s), map_x(g->turrets[i].pos.z, y, s),
                3.0f, 0xFFFFD54Au);
    for (i = 0; i < TDM_MAX_VEHICLES; i++)
        if (g->vehicles[i].alive)
            dot(c, map_x(g->vehicles[i].pos.x, x, s), map_x(g->vehicles[i].pos.z, y, s),
                3.0f, 0xFF60E080u);

    for (i = 0; i < TDM_BOTS_PER_TEAM * 2; i++) {
        const Bot *b = &g->bots[i];
        if (!b->e.alive) continue;
        if (b->e.team != p->e.team && !uav &&
            g->time - p->spotted[i] > 4.0f) continue;
        dot(c, map_x(b->e.pos.x, x, s), map_x(b->e.pos.z, y, s), 3.0f,
            b->e.team == 0 ? 0xFF3B7BFFu : 0xFFF05050u);
    }

    px = map_x(p->e.pos.x, x, s);
    py = map_x(p->e.pos.z, y, s);
    dot(c, px, py, 4.0f, 0xFFFFFFFFu);
    tdm2_line(c, px, py,
              px - 10.0f * sinf(p->e.yaw), py - 10.0f * cosf(p->e.yaw),
              2.0f, 0xFFFFFFFFu);
}
