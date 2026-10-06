#include "tdm_map.h"

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_rng.h"

static void place_props(Game *g)
{
    static const float vp[TDM_MAX_VEHICLES][3] = {
        { -40, -95, 0 }, { 40, -95, 0 },
        { -40, 95, 3.14159265f }, { 40, 95, 3.14159265f }
    };
    static const float tp[TDM_MAX_TURRETS][3] = {
        { 55, 0, 1.57079633f }, { -55, 0, -1.57079633f },
        { 0, 55, 3.14159265f }, { 0, -55, 0 }
    };
    int i, k;
    for (i = 0; i < TDM_MAX_VEHICLES; i++) {
        Vehicle *v = &g->vehicles[i];
        float x = vp[i][0], z = vp[i][1];
        v->spawnPos = v3(x, tdm_terrain_height(&g->terrain, x, z), z);
        v->pos = v->spawnPos;
        v->yaw = v->spawnYaw = vp[i][2];
        v->speed = 0.0f;
        v->maxHealth = 220.0f;
        v->health = 220.0f;
        v->alive = 1;
        v->driverIdx = -1;
        v->respawnT = 0.0f;
        for (k = 0; k < TDM_ENTITY_COUNT; k++) v->hitT[k] = -99.0f;
    }
    for (i = 0; i < TDM_MAX_TURRETS; i++) {
        Turret *t = &g->turrets[i];
        float x = tp[i][0], z = tp[i][1];
        t->pos = v3(x, tdm_terrain_height(&g->terrain, x, z), z);
        t->baseYaw = tp[i][2];
        t->gunnerIdx = -1;
    }
}

static void place_pickups(Game *g)
{
    int i, made = 0;
    for (i = 0; i < 400 && made < TDM_MAX_PICKUPS; i++) {
        float x = tdm_rng_range(&g->rng, -110.0f, 110.0f);
        float z = tdm_rng_range(&g->rng, -110.0f, 110.0f);
        Pickup *p;
        if (tdm_world_blocked(&g->world, x, z, 2.0f, -1.0f, -1.0f)) continue;
        if (tdm_terrain_is_lake(&g->terrain, x, z, 3.0f)) continue;
        p = &g->pickups[made];
        p->pos = v3(x, tdm_terrain_height(&g->terrain, x, z), z);
        p->kind = made % 2;
        p->active = 1;
        p->cooldown = 0.0f;
        made++;
    }
}

void tdm_map_props(Game *g)
{
    place_props(g);
    place_pickups(g);
}
