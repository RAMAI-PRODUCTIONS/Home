#include "tdm_combat.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"


void tdm_camera_shake(Game *g, float amount)
{
    if (amount < 0.0f) amount = 0.0f;
    if (amount > g->shakeAmt) g->shakeAmt = amount;
    if (g->shakeT < 0.35f) g->shakeT = 0.35f;
}

void tdm_explosion(Game *g, Vec3 pos, float radius, float maxDmg, int ownerIdx)
{
    int i;
    tdm_fx_explosion(&g->fx, pos);
    {
        float d = v3_len(v3_sub(g->player.e.pos, pos));
        if (d < 30.0f) tdm_camera_shake(g, 1.0f - d / 30.0f);
    }
    for (i = 0; i < TDM_ENTITY_COUNT; i++) {
        Entity *e = tdm_entity(g, i);
        float d;
        if (!e || !e->alive) continue;
        d = v3_len(v3_sub(e->pos, pos));
        if (d < radius) tdm_entity_damage(g, e, maxDmg * (1.0f - d / radius), ownerIdx);
    }
    for (i = 0; i < TDM_MAX_VEHICLES; i++) {
        Vehicle *v = &g->vehicles[i];
        float d;
        if (!v->alive) continue;
        d = v3_len(v3_sub(v->pos, pos));
        if (d < radius + 2.0f)
            tdm_vehicle_damage(g, v, maxDmg * 1.2f * (1.0f - d / (radius + 2.0f)), ownerIdx);
    }
}
