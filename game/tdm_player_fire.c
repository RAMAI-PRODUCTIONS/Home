#include "tdm_player.h"

#include <math.h>

#include "tdm_combat.h"
#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

void tdm_player_fire(Game *g, InputState *in, float dt)
{
    Player *p = &g->player;
    const WeaponDef *def = tdm_weapon(p->weapon);
    float mul;
    (void)dt;
    if (!in->firing || p->fireCooldown > 0.0f || p->reloading) return;
    if (p->mags[p->weapon] <= 0) {
        tdm_player_reload(g);
        return;
    }
    p->fireCooldown = def->fireRate;
    p->mags[p->weapon]--;
    p->recoil = tdm_clampf(p->recoil + (def->pellets > 1 ? 0.6f : 0.4f), 0.0f, 1.0f);
    g->camPitch += 0.006f * (def->pellets > 1 ? 1.6f : 1.0f);
    mul = in->ads ? (def->zoomFov > 0.0f ? 0.15f : 0.4f) : 1.0f;
    tdm_fire(g, 0, g->camPos, tdm_look_dir(g), def, mul, 0.0f, 1);
    tdm_fx_flash(&g->fx, v3_add(g->camPos, v3_scale(tdm_look_dir(g), 0.8f)),
                 0.05f, 0.25f, 0xFFAA22u);
    if (p->mags[p->weapon] <= 0) tdm_player_reload(g);
}

void tdm_player_grenade(Game *g, InputState *in)
{
    Player *p = &g->player;
    in->grenade = 0;
    if (p->grenades <= 0 || p->seatKind || !p->e.alive) return;
    p->grenades--;
    tdm_grenade_throw(g, 0, tdm_look_dir(g), 24.0f, 6.0f);
}
