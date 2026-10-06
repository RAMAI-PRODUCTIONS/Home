#include "tdm_combat.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"



float tdm_ray_sphere(Vec3 o, Vec3 d, Vec3 c, float r, float maxT);

int tdm_ray_vehicle(Game *g, Vec3 origin, Vec3 dir, float maxT, int *outIdx, float *outT)
{
    int i, best = -1;
    float bd = maxT;
    for (i = 0; i < TDM_MAX_VEHICLES; i++) {
        Vehicle *v = &g->vehicles[i];
        float t;
        if (!v->alive) continue;
        t = tdm_ray_sphere(origin, dir, v3(v->pos.x, v->pos.y + 1.1f, v->pos.z), 2.3f, bd);
        if (t >= 0.0f && t < bd) { bd = t; best = i; }
    }
    *outIdx = best;
    *outT = bd;
    return best >= 0;
}
