#include "tdm_props.h"

#include "tdm_combat.h"
#include "tdm_game.h"
#include "tdm_math.h"

#define TDM_GRENADE_RADIUS 9.0f
#define TDM_GRENADE_DAMAGE 130.0f
#define TDM_GRENADE_FUSE 2.2f

void tdm_grenade_throw(Game *g, int ownerIdx, Vec3 dir, float speed, float up)
{
    Entity *owner = tdm_entity(g, ownerIdx);
    Grenade *n;
    if (!owner || g->grenadeN >= TDM_MAX_GRENADES) return;
    n = &g->grenades[g->grenadeN++];
    n->pos = v3_add(tdm_entity_eye(owner), v3_scale(dir, 0.6f));
    n->vel = v3_add(v3_scale(dir, speed), v3(0.0f, up, 0.0f));
    n->fuse = TDM_GRENADE_FUSE;
    n->ownerIdx = ownerIdx;
    n->dead = 0;
}

static void bounce(Game *g, Grenade *n, float dt)
{
    float nx = n->pos.x + n->vel.x * dt;
    float nz = n->pos.z + n->vel.z * dt;
    float ny = n->pos.y + n->vel.y * dt;
    float gy = tdm_terrain_height(&g->terrain, nx, nz);

    if (tdm_world_blocked(&g->world, nx, n->pos.z, 0.25f, -1.0f, -1.0f)) n->vel.x *= -0.5f;
    else n->pos.x = nx;
    if (tdm_world_blocked(&g->world, n->pos.x, nz, 0.25f, -1.0f, -1.0f)) n->vel.z *= -0.5f;
    else n->pos.z = nz;

    if (ny <= gy) {
        n->pos.y = gy;
        n->vel.y *= -0.4f;
        n->vel.x *= 0.75f;
        n->vel.z *= 0.75f;
    } else {
        n->pos.y = ny;
    }
}

void tdm_grenade_update(Game *g, Grenade *n, float dt)
{
    if (n->dead) return;
    n->fuse -= dt;
    n->vel.y -= 22.0f * dt;
    bounce(g, n, dt);
    if (n->fuse > 0.0f) return;
    n->dead = 1;
    tdm_explosion(g, n->pos, TDM_GRENADE_RADIUS, TDM_GRENADE_DAMAGE, n->ownerIdx);
}
