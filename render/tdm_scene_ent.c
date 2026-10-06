#include "tdm_scene.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"

static void turret(Geo *g, const Turret *t)
{
    int i;
    for (i = 0; i < 4; i++) {
        float ox = (i & 1) ? 0.7f : -0.7f;
        float oz = (i & 2) ? 0.7f : -0.7f;
        tdm_geo_box(g, v3(t->pos.x + ox, t->pos.y + 0.35f, t->pos.z + oz),
                    v3(0.45f, 0.35f, 0.45f), 0.0f, 0x8A7A55u);
    }
    tdm_geo_box(g, v3(t->pos.x, t->pos.y + 0.75f, t->pos.z), v3(0.4f, 0.25f, 0.4f),
                t->baseYaw, 0x2A2A2Au);
    tdm_geo_box(g, v3(t->pos.x, t->pos.y + 1.1f, t->pos.z),
                v3(0.08f, 0.08f, 0.7f), t->baseYaw, 0x2A2A2Au);
}

void tdm_scene_entities(Geo *g, Game *game)
{
    int i;
    for (i = 0; i < TDM_BOTS_PER_TEAM * 2; i++)
        if (game->bots[i].e.alive) tdm_scene_bot(g, &game->bots[i], game->time);
    for (i = 0; i < TDM_MAX_VEHICLES; i++)
        if (game->vehicles[i].alive) tdm_scene_vehicle(g, &game->vehicles[i]);
    for (i = 0; i < TDM_MAX_TURRETS; i++) turret(g, &game->turrets[i]);
    for (i = 0; i < TDM_MAX_PICKUPS; i++) {
        const Pickup *p = &game->pickups[i];
        if (!p->active) continue;
        tdm_geo_box(g, v3(p->pos.x, p->pos.y + 0.6f +
                           sinf(game->time * 2.0f) * 0.1f, p->pos.z),
                    v3(0.35f, 0.35f, 0.35f), game->time * 1.5f,
                    p->kind == 1 ? 0xFF4444u : 0xFFD54Au);
    }
    for (i = 0; i < game->grenadeN; i++) {
        const Grenade *n = &game->grenades[i];
        tdm_geo_box(g, n->pos, v3(0.14f, 0.14f, 0.14f), game->time * 4.0f, 0x2F4A2Fu);
    }
}
