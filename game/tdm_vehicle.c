#include "tdm_props.h"

#include <math.h>

#include "tdm_combat.h"
#include "tdm_game.h"
#include "tdm_math.h"

void tdm_vehicle_drive(Game *g, Vehicle *v, float moveX, float moveY, float dt)
{
    float target = moveY * 20.0f;
    int i;
    v->speed = tdm_lerpf(v->speed, target,
                         tdm_clampf(dt * (fabsf(moveY) > 0.05f ? 2.2f : 2.5f), 0.0f, 1.0f));
    if (fabsf(v->speed) > 0.4f)
        v->yaw += -moveX * 2.1f * dt * (v->speed >= 0.0f ? 1.0f : -1.0f);

    {
        Vec3 dir = v3(sinf(v->yaw), 0.0f, cosf(v->yaw));
        float nx = v->pos.x - dir.x * v->speed * dt;
        float nz = v->pos.z - dir.z * v->speed * dt;
        if (!tdm_world_blocked(&g->world, nx, v->pos.z, 2.0f, -1.0f, -1.0f)) v->pos.x = nx;
        else v->speed *= 0.25f;
        if (!tdm_world_blocked(&g->world, v->pos.x, nz, 2.0f, -1.0f, -1.0f)) v->pos.z = nz;
        else v->speed *= 0.25f;
    }
    v->pos.x = tdm_clampf(v->pos.x, -TDM_HALF + 2.0f, TDM_HALF - 2.0f);
    v->pos.z = tdm_clampf(v->pos.z, -TDM_HALF + 2.0f, TDM_HALF - 2.0f);
    v->pos.y = tdm_terrain_height(&g->terrain, v->pos.x, v->pos.z);

    if (fabsf(v->speed) > 6.0f) {
        for (i = 0; i < TDM_ENTITY_COUNT; i++) {
            Entity *e = tdm_entity(g, i);
            if (!e || !e->alive) continue;
            if (i == v->driverIdx) continue;
            if (v->driverIdx >= 0 && e->team == tdm_entity(g, v->driverIdx)->team) continue;
            if (tdm_dist2d(e->pos, v->pos) < 2.0f && g->time - v->hitT[i] > 1.0f) {
                v->hitT[i] = g->time;
                tdm_entity_damage(g, e, 55.0f, v->driverIdx);
            }
        }
    }
}
