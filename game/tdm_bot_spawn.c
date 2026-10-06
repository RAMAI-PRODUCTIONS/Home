#include "tdm_bot.h"

#include <stdio.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"

void tdm_bot_spawn(Game *g, int index)
{
    Bot *b = &g->bots[index];
    int team = index < TDM_BOTS_PER_TEAM ? TDM_TEAM_BLUE : TDM_TEAM_RED;
    int num = (index % TDM_BOTS_PER_TEAM) + 1;
    Vec3 s = g->spawns[team][tdm_game_spawn_index(g, team)];

    b->e.pos = v3(s.x, tdm_terrain_height(&g->terrain, s.x, s.z), s.z);
    b->e.vel = v3(0, 0, 0);
    b->e.yaw = team == TDM_TEAM_BLUE ? 0.78f : -2.35f;
    b->e.health = 100.0f;
    b->e.alive = 1;
    b->e.team = team;
    b->e.lastDamage = -99.0f;
    snprintf(b->e.name, sizeof b->e.name, "%c%d", team == TDM_TEAM_BLUE ? 'B' : 'R', num);
    b->state = 0;
    b->targetIdx = -1;
    b->pathN = b->pathI = 0;
    b->nextThink = tdm_rng_f(&g->rng) * 0.4f;
    b->nextPath = 0.0f;
    b->weapon = tdm_weapon_pick(&g->rng);
    b->mag = tdm_weapons[b->weapon].mag;
    b->fireDelay = tdm_weapons[b->weapon].automatic
                   ? 0.08f + tdm_rng_f(&g->rng) * 0.06f
                   : tdm_weapons[b->weapon].fireRate * (0.8f + tdm_rng_f(&g->rng) * 0.4f);
    b->accuracy = 0.4f + tdm_rng_f(&g->rng) * 0.3f;
    b->strafeDir = tdm_rng_f(&g->rng) < 0.5f ? -1.0f : 1.0f;
    b->strafeT = 0.0f;
    b->respawnT = 0.0f;
    b->lastGrenade = -99.0f;
    b->stuck = 0;
    b->moving = 0;
    tdm_bot_patrol(g, b);
}

void tdm_bot_tick(Game *g, Bot *b, float dt)
{
    if (!b->e.alive) {
        b->respawnT -= dt;
        if (b->respawnT <= 0.0f) tdm_bot_spawn(g, (int)(b - g->bots));
        return;
    }
    tdm_bot_think(g, b, dt);
    tdm_bot_act(g, b, dt);
}
