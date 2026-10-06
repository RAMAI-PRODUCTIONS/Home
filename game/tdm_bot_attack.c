#include "tdm_bot.h"

#include <math.h>

#include "tdm_combat.h"
#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"

static void shoot(Game *g, Bot *b, Entity *target)
{
    const WeaponDef *def = tdm_weapon(b->weapon);
    Vec3 origin = tdm_entity_eye(&b->e);
    Vec3 dir = v3_norm(v3_sub(tdm_entity_eye(target), origin));
    int idx = (int)(b - g->bots) + 1;

    if (b->mag <= 0) {
        b->mag = def->mag;
        return;
    }
    b->mag--;
    tdm_fire(g, idx, origin, dir, def, 1.0f, (1.0f - b->accuracy) * 0.06f, 0);
    tdm_fx_flash(&g->fx, v3_add(origin, v3_scale(dir, 0.7f)), 0.05f, 0.25f, 0xFFAA22u);
}

void tdm_bot_face(Game *g, Bot *b, float want, float dt)
{
    float diff = want - b->e.yaw;
    (void)g;
    while (diff > 3.14159265f) diff -= 6.2831853f;
    while (diff < -3.14159265f) diff += 6.2831853f;
    b->e.yaw += diff * tdm_clampf(dt * 6.0f, 0.0f, 1.0f);
}

void tdm_bot_attack(Game *g, Bot *b, Entity *target, float dt, float *mvx, float *mvz)
{
    float dx = target->pos.x - b->e.pos.x;
    float dz = target->pos.z - b->e.pos.z;
    float dist = sqrtf(dx * dx + dz * dz);
    int idx = (int)(b - g->bots) + 1;

    tdm_bot_face(g, b, atan2f(dx, dz), dt);
    b->strafeT -= dt;
    if (b->strafeT <= 0.0f) {
        b->strafeDir = -b->strafeDir;
        b->strafeT = 0.7f + tdm_rng_f(&g->rng);
    }
    *mvx = cosf(b->e.yaw) * b->strafeDir * 0.5f;
    *mvz = -sinf(b->e.yaw) * b->strafeDir * 0.5f;

    if (g->time - b->lastShot > b->fireDelay) {
        b->lastShot = g->time;
        shoot(g, b, target);
    }
    if (g->time - b->lastGrenade > 9.0f && dist > 8.0f && dist < 22.0f &&
        tdm_rng_f(&g->rng) < 0.02f) {
        b->lastGrenade = g->time;
        tdm_grenade_throw(g, idx,
                          v3_norm(v3_sub(tdm_entity_eye(target), tdm_entity_eye(&b->e))),
                          16.0f, 7.0f);
    }
}
