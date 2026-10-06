#include "tdm_player.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

void tdm_player_on_foot(Game *g, InputState *in, float dt)
{
    Player *p = &g->player;
    const WeaponDef *def = tdm_weapon(p->weapon);
    Vec3 move;
    float speed, ground, mul, cy, sy;

    p->e.yaw -= in->lookX;
    p->pitch = tdm_clampf(p->pitch - in->lookY, -1.45f, 1.45f);
    g->camYaw = p->e.yaw;
    g->camPitch = p->pitch;
    g->camPos = v3(p->e.pos.x, p->e.pos.y + 1.7f, p->e.pos.z);
    g->camFov = tdm_lerpf(g->camFov,
                          (in->ads && def->zoomFov > 0.0f) ? def->zoomFov : 75.0f,
                          tdm_clampf(dt * 10.0f, 0.0f, 1.0f));
    p->ads = in->ads;

    mul = in->ads ? 0.55f : 1.0f;
    speed = (in->sprint && !in->ads ? TDM_SPRINT_SPEED : TDM_PLAYER_SPEED) * mul;
    move = v3(in->moveX, 0.0f, -in->moveY);
    if (move.x * move.x + move.z * move.z > 1.0f) move = v3_norm(move);
    cy = cosf(p->e.yaw);
    sy = sinf(p->e.yaw);
    p->e.vel.x = tdm_lerpf(p->e.vel.x, (move.x * cy - move.z * sy) * speed,
                           tdm_clampf(dt * 10.0f, 0.0f, 1.0f));
    p->e.vel.z = tdm_lerpf(p->e.vel.z, (-move.x * sy - move.z * cy) * speed,
                           tdm_clampf(dt * 10.0f, 0.0f, 1.0f));

    {
        float nx = p->e.pos.x + p->e.vel.x * dt;
        float nz = p->e.pos.z + p->e.vel.z * dt;        if (!tdm_world_blocked(&g->world, nx, p->e.pos.z, TDM_RADIUS, p->e.pos.y, 1.8f))
            p->e.pos.x = nx;
        if (!tdm_world_blocked(&g->world, p->e.pos.x, nz, TDM_RADIUS, p->e.pos.y, 1.8f))
            p->e.pos.z = nz;
    }
    p->e.pos.x = tdm_clampf(p->e.pos.x, -TDM_HALF + 1.0f, TDM_HALF - 1.0f);
    p->e.pos.z = tdm_clampf(p->e.pos.z, -TDM_HALF + 1.0f, TDM_HALF - 1.0f);

    ground = tdm_terrain_height(&g->terrain, p->e.pos.x, p->e.pos.z);
    if (in->jump && p->grounded) {
        p->vy = TDM_JUMP_VEL;
        p->grounded = 0;
    }
    p->vy -= TDM_GRAVITY * dt;
    p->e.pos.y += p->vy * dt;
    if (p->e.pos.y <= ground) {
        p->e.pos.y = ground;
        p->vy = 0.0f;
        p->grounded = 1;
    }
    if (g->time - p->e.lastDamage > 3.0f && p->e.health < 100.0f) {
        p->e.health += 6.0f * dt;
        if (p->e.health > 100.0f) p->e.health = 100.0f;
    }
}
