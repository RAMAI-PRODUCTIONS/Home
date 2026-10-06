#include "tdm_bot.h"

#include <stdio.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"

void tdm_bot_patrol(Game *g, Bot *b)
{
    Vec3 p;
    tdm_path_random(&g->grid, &g->rng, &p);
    tdm_bot_repath(g, b, p);
    b->state = 0;
}

void tdm_bot_repath(Game *g, Bot *b, Vec3 to)
{
    b->pathN = tdm_path_find(&g->grid, b->e.pos, to, b->path, TDM_MAX_PATH);
    b->pathI = 0;
}

int tdm_bot_sees(const Game *g, const Bot *b, const Entity *e)
{
    if (tdm_dist2d(b->e.pos, e->pos) > TDM_BOT_SIGHT) return 0;
    return tdm_world_los(&g->world, tdm_entity_eye(&b->e), tdm_entity_eye(e));
}

void tdm_bot_think(Game *g, Bot *b, float dt)
{
    float bd = 1e9f;
    int i, best = -1;
    b->nextThink -= dt;
    if (b->nextThink > 0.0f) return;
    b->nextThink = 0.25f;

    for (i = 0; i < TDM_ENTITY_COUNT; i++) {
        Entity *e = tdm_entity(g, i);
        float d;
        if (!e || e == &b->e || !e->alive || e->team == b->e.team) continue;
        d = tdm_dist2d(e->pos, b->e.pos);
        if (d < bd && tdm_bot_sees(g, b, e)) {
            bd = d;
            best = i;
        }
    }

    if (best >= 0) {
        b->targetIdx = best;
        b->state = bd < TDM_BOT_RANGE ? 2 : 1;
        if (b->state == 1 && g->time > b->nextPath) {
            tdm_bot_repath(g, b, tdm_entity(g, best)->pos);
            b->nextPath = g->time + 2.0f;
        }
        return;
    }
    if (b->state != 0) {
        Entity *t = tdm_entity(g, b->targetIdx);
        if (t && t->alive && g->time > b->nextPath) {
            b->state = 1;
            tdm_bot_repath(g, b, t->pos);
            b->nextPath = g->time + 2.5f;
        } else if (b->pathN == 0 || b->pathI >= b->pathN) {
            tdm_bot_patrol(g, b);
        }
    }
    if (b->state == 0 && b->pathI >= b->pathN) tdm_bot_patrol(g, b);
}
