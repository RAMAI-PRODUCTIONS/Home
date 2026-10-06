#include "tdm_combat.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"



static uint32_t s_seed = 0x13572468u;

void tdm_fire(Game *g, int shooterIdx, Vec3 origin, Vec3 dir,
              const WeaponDef *def, float mul, float extra, int isPlayer)
{
    int p;
    Entity *sh = tdm_entity(g, shooterIdx);
    uint32_t tint = 0xFF9988u;
    if (shooterIdx == 0) tint = 0x9FD0FFu;
    else if (sh && sh->team == TDM_TEAM_BLUE) tint = 0x88B8FFu;
    for (p = 0; p < def->pellets; p++) {
        Vec3 d = dir, end;
        RayHit wall;
        float sp = def->spread * mul + extra, limit, et = 0.0f, vt = 0.0f;
        int ai = -1, vi = -1, head = 0;

        d.x += (tdm_rng_f(&s_seed) - 0.5f) * sp;
        d.y += (tdm_rng_f(&s_seed) - 0.5f) * sp;
        d.z += (tdm_rng_f(&s_seed) - 0.5f) * sp;
        d = v3_norm(d);

        wall = tdm_world_ray(&g->world, origin, d, def->range);
        limit = wall.hit ? wall.t : def->range;
        tdm_ray_vehicle(g, origin, d, limit, &vi, &vt);
        if (vi >= 0) limit = vt;
        tdm_ray_actor(g, shooterIdx, origin, d, limit, &ai, &et, &head);

        if (ai >= 0) {
            Entity *victim = tdm_entity(g, ai);
            float dmg = def->dmgMin + tdm_rng_f(&s_seed) * (def->dmgMax - def->dmgMin);
            if (head) dmg *= def->headMult;
            end = v3_add(origin, v3_scale(d, et));
            tdm_entity_damage(g, victim, dmg, shooterIdx);
            tdm_fx_spark(&g->fx, end, 0xBB2222u, 5);
            if (isPlayer) g->match.hitMark = 0.15f;
        } else if (vi >= 0) {
            end = v3_add(origin, v3_scale(d, vt));
            tdm_vehicle_damage(g, &g->vehicles[vi], def->dmgMax * 0.5f, shooterIdx);
            tdm_fx_spark(&g->fx, end, 0xFFAA33u, 4);
        } else if (wall.hit) {
            end = wall.point;
            tdm_fx_spark(&g->fx, end, wall.type == 1 ? 0x5B3D24u : 0xD8C16Au, 5);
        } else {
            end = v3_add(origin, v3_scale(d, def->range));
        }
        tdm_fx_tracer(&g->fx, v3_add(origin, v3_scale(dir, 0.4f)), end, tint);
    }
}
