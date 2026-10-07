#include "tdm_player.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

void tdm_player_seated(Game *g, InputState *in, float dt)
{
    Player *p = &g->player;
    float cy, sy;
    p->e.yaw -= in->lookX;
    p->pitch = tdm_clampf(p->pitch + in->lookY,
                          p->seatKind == 2 ? -0.5f : -1.45f,
                          p->seatKind == 2 ? 0.5f : 1.45f);
    g->camFov = tdm_lerpf(g->camFov, 75.0f, tdm_clampf(dt * 10.0f, 0.0f, 1.0f));
    cy = cosf(p->e.yaw);
    sy = sinf(p->e.yaw);
    if (p->seatKind == 1) {
        Vehicle *v = &g->vehicles[p->seatIdx];
        tdm_vehicle_drive(g, v, in->moveX, in->moveY, dt);
        p->e.pos = v->pos;
        g->camPos = v3(v->pos.x + 0.1f * sy, v->pos.y + 1.7f, v->pos.z + 0.1f * cy);
        g->camYaw = p->e.yaw;
        g->camPitch = p->pitch;
        if (in->firing) tdm_player_vehicle_fire(g, dt);
    } else {
        Turret *t = &g->turrets[p->seatIdx];
        float rel = p->e.yaw - t->baseYaw;
        while (rel > 3.14159265f) rel -= 6.2831853f;
        while (rel < -3.14159265f) rel += 6.2831853f;
        rel = tdm_clampf(rel, -1.3f, 1.3f);
        p->e.yaw = t->baseYaw + rel;
        p->e.pos = t->pos;
        g->camPos = v3(t->pos.x, t->pos.y + 1.15f, t->pos.z);
        g->camYaw = p->e.yaw;
        g->camPitch = p->pitch;
        if (in->firing) tdm_player_turret_fire(g, dt);
    }
}
