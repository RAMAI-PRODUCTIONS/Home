#include "tdm_combat.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"


float tdm_ray_sphere(Vec3 o, Vec3 d, Vec3 c, float r, float maxT)
{
    Vec3 m = v3_sub(o, c);
    float b = v3_dot(m, d), k = v3_dot(m, m) - r * r;
    float disc, t;
    if (k > 0.0f && b > 0.0f) return -1.0f;
    disc = b * b - k;
    if (disc < 0.0f) return -1.0f;
    t = -b - sqrtf(disc);
    if (t < 0.0f) t = 0.0f;
    return t <= maxT ? t : -1.0f;
}

int tdm_ray_actor(Game *g, int shooterIdx, Vec3 origin, Vec3 dir,
                  float maxDist, int *outIdx, float *outT, int *outHead)
{
    Entity *shooter = tdm_entity(g, shooterIdx);
    int i, best = -1, head = 0;
    float bd = maxDist;
    for (i = 0; i < TDM_ENTITY_COUNT; i++) {
        Entity *e = tdm_entity(g, i);
        Vec3 c[2];
        float r[2], t;
        int h;
        if (!e || i == shooterIdx || !e->alive) continue;
        if (shooter && e->team == shooter->team) continue;
        tdm_entity_hitboxes(e, c, r);
        for (h = 0; h < 2; h++) {
            t = tdm_ray_sphere(origin, dir, c[h], r[h], bd);
            if (t >= 0.0f && t < bd) { bd = t; best = i; head = h; }
        }
    }
    *outIdx = best;
    *outT = bd;
    *outHead = head;
    return best >= 0;
}
