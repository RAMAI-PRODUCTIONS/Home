#include "tdm_bot.h"

#include <math.h>

#include "tdm_combat.h"
#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"

static void separate(Game *g, Bot *b, float *mvx, float *mvz)
{
    int i;
    for (i = 0; i < TDM_ENTITY_COUNT; i++) {
        Entity *e = tdm_entity(g, i);
        float dx, dz, d;
        if (!e || e == &b->e || !e->alive) continue;
        dx = b->e.pos.x - e->pos.x;
        dz = b->e.pos.z - e->pos.z;
        d = sqrtf(dx * dx + dz * dz);
        if (d > 0.0f && d < 1.4f) {
            *mvx += dx / d * 1.5f;
            *mvz += dz / d * 1.5f;
        }
    }
}

static void follow(Game *g, Bot *b, float dt, float *mvx, float *mvz)
{
    Vec3 wp;
    float dx, dz, d;
    if (!b->pathN || b->pathI >= b->pathN) return;
    wp = b->path[b->pathI];
    dx = wp.x - b->e.pos.x;
    dz = wp.z - b->e.pos.z;
    d = sqrtf(dx * dx + dz * dz);
    if (d < 0.8f) {
        b->pathI++;
        return;
    }
    *mvx = dx / d;
    *mvz = dz / d;
    tdm_bot_face(g, b, atan2f(*mvx, *mvz), dt);
}

void tdm_bot_act(Game *g, Bot *b, float dt)
{
    Entity *target = tdm_entity(g, b->targetIdx);
    float mvx = 0.0f, mvz = 0.0f, nx, nz;

    if (b->state == 2 && target && target->alive && tdm_bot_sees(g, b, target))
        tdm_bot_attack(g, b, target, dt, &mvx, &mvz);
    else
        follow(g, b, dt, &mvx, &mvz);
    separate(g, b, &mvx, &mvz);

    nx = b->e.pos.x + mvx * TDM_BOT_SPEED * dt;
    nz = b->e.pos.z + mvz * TDM_BOT_SPEED * dt;
    if (!tdm_world_blocked(&g->world, nx, b->e.pos.z, TDM_RADIUS, b->e.pos.y, 1.8f))
        b->e.pos.x = nx;
    else
        b->stuck++;
    if (!tdm_world_blocked(&g->world, b->e.pos.x, nz, TDM_RADIUS, b->e.pos.y, 1.8f))
        b->e.pos.z = nz;
    else
        b->stuck++;
    if (b->stuck > 40) {
        b->stuck = 0;
        tdm_bot_patrol(g, b);
    }

    b->e.pos.y = tdm_terrain_height(&g->terrain, b->e.pos.x, b->e.pos.z);
    b->moving = fabsf(mvx) + fabsf(mvz) > 0.1f ? 1 : 0;
}
