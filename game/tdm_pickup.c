#include "tdm_props.h"

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_weapons.h"

static void refill(int isPlayer, Game *g)
{
    int k;
    if (!isPlayer) return;
    for (k = 0; k < TDM_W_COUNT; k++) {
        g->player.mags[k] = tdm_weapons[k].mag;
        g->player.reserves[k] = tdm_weapons[k].reserveMax;
    }
    g->player.grenades = 3;
}

void tdm_pickup_update(Game *g, Pickup *p, float dt)
{
    int i;
    if (!p->active) {
        p->cooldown -= dt;
        if (p->cooldown <= 0.0f) {
            p->active = 1;
            p->cooldown = 0.0f;
        }
        return;
    }
    for (i = 0; i < TDM_ENTITY_COUNT; i++) {
        Entity *e = tdm_entity(g, i);
        int used = 0;
        if (!e || !e->alive) continue;
        if (v3_len(v3_sub(e->pos, p->pos)) > 1.6f) continue;
        if (p->kind == 1 && e->health < 100.0f) {
            e->health = e->health + 55.0f > 100.0f ? 100.0f : e->health + 55.0f;
            used = 1;
        }
        if (p->kind == 0) {
            refill(e == &g->player.e, g);
            used = 1;
        }
        if (used) {
            p->active = 0;
            p->cooldown = 22.0f;
            return;
        }
    }
}
