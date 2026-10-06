#include "tdm_player.h"

#include <math.h>

#include "tdm_combat.h"
#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

Vec3 tdm_look_dir(const Game *g)
{
    float cp = cosf(g->camPitch), sp = sinf(g->camPitch);
    float cy = cosf(g->camYaw), sy = sinf(g->camYaw);
    return v3(-sy * cp, sp, -cy * cp);
}

void tdm_player_apply_weapon(Game *g, int weapon)
{
    Player *p = &g->player;
    if (weapon < 0 || weapon >= TDM_W_COUNT || weapon == p->weapon) return;
    p->weapon = weapon;
    p->reloading = 0;
}

void tdm_player_reload(Game *g)
{
    Player *p = &g->player;
    const WeaponDef *def = tdm_weapon(p->weapon);
    if (p->reloading || p->mags[p->weapon] >= def->mag) return;
    if (p->reserves[p->weapon] <= 0) return;
    p->reloading = 1;
    p->reloadT = def->reloadTime;
}

void tdm_player_reload_tick(Game *g, float dt)
{
    Player *p = &g->player;
    const WeaponDef *def;
    int mag, take;
    if (!p->reloading) return;
    p->reloadT -= dt;
    if (p->reloadT > 0.0f) return;
    def = tdm_weapon(p->weapon);
    mag = p->mags[p->weapon];
    take = def->mag - mag;
    if (take > p->reserves[p->weapon]) take = p->reserves[p->weapon];
    p->mags[p->weapon] = mag + take;
    p->reserves[p->weapon] -= take;
    p->reloading = 0;
}
