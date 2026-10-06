#include "tdm_player.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

void tdm_player_tick(Game *g, InputState *in, float dt)
{
    Player *p = &g->player;

    p->fireCooldown -= dt;
    if (!p->e.alive) {
        p->deadTimer -= dt;
        g->camRoll = tdm_lerpf(g->camRoll, 0.55f, tdm_clampf(dt * 3.0f, 0.0f, 1.0f));
        if (p->deadTimer <= 0.0f) tdm_player_respawn(g);
        return;
    }
    g->camRoll = tdm_lerpf(g->camRoll, 0.0f, tdm_clampf(dt * 6.0f, 0.0f, 1.0f));
    tdm_player_enter_exit(g, in);
    if (in->weaponSwitch >= 0) {
        tdm_player_apply_weapon(g, in->weaponSwitch);
        in->weaponSwitch = -1;
    }
    if (in->cycle) {
        tdm_player_apply_weapon(g, (p->weapon + 1) % TDM_W_COUNT);
        in->cycle = 0;
    }
    if (in->grenade) tdm_player_grenade(g, in);
    tdm_player_reload_tick(g, dt);
    p->recoil = tdm_clampf(p->recoil - dt * 5.0f, 0.0f, 1.0f);
    tdm_player_nearby(g);

    if (p->seatKind) {
        tdm_player_seated(g, in, dt);
        return;
    }
    tdm_player_on_foot(g, in, dt);
    tdm_player_fire(g, in, dt);
    if (in->reload) {
        in->reload = 0;
        tdm_player_reload(g);
    }
    tdm_player_spot(g, dt);
    g->camPos = v3(p->e.pos.x, p->e.pos.y + 1.7f, p->e.pos.z);
}
