#include "tdm_props.h"

#include <math.h>

#include "tdm_combat.h"
#include "tdm_game.h"
#include "tdm_math.h"

void tdm_vehicle_damage(Game *g, Vehicle *v, float dmg, int attackerIdx)
{
    if (!v->alive) return;
    v->health -= dmg;
    if (v->health <= 0.0f) tdm_vehicle_destroy(g, v, attackerIdx);
}

void tdm_vehicle_destroy(Game *g, Vehicle *v, int attackerIdx)
{
    v->alive = 0;
    v->health = 0.0f;
    v->respawnT = 25.0f;
    tdm_explosion(g, v3(v->pos.x, v->pos.y + 1.0f, v->pos.z), 10.0f, 90.0f, attackerIdx);
    if (v->driverIdx >= 0) {
        Entity *d = tdm_entity(g, v->driverIdx);
        if (d) tdm_entity_damage(g, d, 60.0f, attackerIdx);
        if (v->driverIdx == 0) {
            g->player.seatKind = 0;
            g->player.seatIdx = -1;
        }
        v->driverIdx = -1;
    }
}

void tdm_vehicle_update(Game *g, Vehicle *v, float dt)
{
    (void)g;
    if (v->alive) return;
    v->respawnT -= dt;
    if (v->respawnT > 0.0f) return;
    v->pos = v->spawnPos;
    v->yaw = v->spawnYaw;
    v->health = v->maxHealth;
    v->alive = 1;
    v->speed = 0.0f;
}
