#include "tdm_player.h"

#include <math.h>

#include "tdm_combat.h"
#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

static const WeaponDef s_vehicleGun = {
    "VEHICLE MG", 14, 22, 0.09f, 0.012f, 1.5f, 1.0f, 130, 0, 10000, 10000, 1, 1
};

static const WeaponDef s_turretGun = {
    "TURRET", 10, 17, 0.07f, 0.018f, 1.4f, 1.0f, 95, 0, 10000, 10000, 1, 1
};

void tdm_player_vehicle_fire(Game *g, float dt)
{
    Player *p = &g->player;
    Vehicle *v = &g->vehicles[p->seatIdx];
    float cy, sy;
    Vec3 origin;
    (void)dt;
    if (p->fireCooldown > 0.0f) return;
    p->fireCooldown = s_vehicleGun.fireRate;
    cy = cosf(p->e.yaw);
    sy = sinf(p->e.yaw);
    origin = v3(v->pos.x - 2.2f * sy, v->pos.y + 1.95f, v->pos.z - 2.2f * cy);
    tdm_fire(g, 0, origin, tdm_look_dir(g), &s_vehicleGun, 1.0f, 0.0f, 1);
    tdm_fx_flash(&g->fx, origin, 0.05f, 0.3f, 0xFFAA22u);
}

void tdm_player_turret_fire(Game *g, float dt)
{
    Player *p = &g->player;
    Turret *t = &g->turrets[p->seatIdx];
    Vec3 origin = v3(t->pos.x, t->pos.y + 1.15f, t->pos.z);
    (void)dt;
    if (p->fireCooldown > 0.0f) return;
    p->fireCooldown = s_turretGun.fireRate;
    tdm_fire(g, 0, origin, tdm_look_dir(g), &s_turretGun, 1.0f, 0.0f, 1);
    tdm_fx_flash(&g->fx, origin, 0.05f, 0.3f, 0xFFAA22u);
}
