#include "tdm_player.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

void tdm_player_place(Game *g, float x, float z)
{
    g->player.e.pos = v3(x, tdm_terrain_height(&g->terrain, x, z), z);
}

void tdm_player_nearby(Game *g)
{
    Player *p = &g->player;
    int i;
    p->nearKind = 0;
    p->nearIdx = -1;
    if (p->seatKind) return;
    for (i = 0; i < TDM_MAX_VEHICLES; i++) {
        Vehicle *v = &g->vehicles[i];
        if (!v->alive || v->driverIdx >= 0) continue;
        if (tdm_dist2d(v->pos, p->e.pos) < 4.0f) {
            p->nearKind = 1;
            p->nearIdx = i;
            return;
        }
    }
    for (i = 0; i < TDM_MAX_TURRETS; i++) {
        Turret *t = &g->turrets[i];
        if (t->gunnerIdx >= 0) continue;
        if (tdm_dist2d(t->pos, p->e.pos) < 3.5f) {
            p->nearKind = 2;
            p->nearIdx = i;
            return;
        }
    }
}

void tdm_player_respawn(Game *g)
{
    Player *p = &g->player;
    Vec3 s = g->spawns[TDM_TEAM_BLUE][tdm_game_spawn_index(g, TDM_TEAM_BLUE)];
    int k;
    tdm_player_place(g, s.x, s.z);
    p->e.health = 100.0f;
    p->e.alive = 1;
    p->e.vel = v3(0, 0, 0);
    p->vy = 0.0f;
    p->grounded = 1;
    p->reloading = 0;
    p->seatKind = 0;
    p->seatIdx = -1;
    p->grenades = 3;
    p->streak = 0;
    for (k = 0; k < TDM_W_COUNT; k++)
        p->mags[k] = tdm_weapons[k].mag;
    g->screen = TDM_SCREEN_PLAY;
}
