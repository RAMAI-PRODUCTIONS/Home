#include "tdm_scene.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_simd.h"
#include "tdm_weapons.h"

typedef struct GunSpec {
    float recvW, recvH, recvL, barrelL;
} GunSpec;

static const GunSpec s_guns[TDM_W_COUNT] = {
    { 0.09f, 0.12f, 0.24f, 0.16f },
    { 0.10f, 0.13f, 0.34f, 0.20f },
    { 0.10f, 0.13f, 0.46f, 0.36f },
    { 0.11f, 0.14f, 0.52f, 0.46f },
    { 0.10f, 0.13f, 0.62f, 0.60f }
};

static Mat4 at(Vec3 p)
{
    Mat4 m = m4_identity();
    m.m[12] = p.x;
    m.m[13] = p.y;
    m.m[14] = p.z;
    return m;
}

void tdm_scene_viewmodel(Geo *g, Game *game)
{
    const Player *p = &game->player;
    const GunSpec *sp;
    Mat4 a, b, m;
    Vec3 o;
    int start, count;
    float bob, speed;

    if (!p->e.alive || p->seatKind) return;
    if (game->screen != TDM_SCREEN_PLAY && game->screen != TDM_SCREEN_DEAD) return;
    sp = &s_guns[p->weapon];
    speed = sqrtf(p->e.vel.x * p->e.vel.x + p->e.vel.z * p->e.vel.z) /
            TDM_SPRINT_SPEED;
    bob = sinf(game->time * 11.0f) * 0.02f * speed;
    o = v3(0.38f, -0.30f + bob, -0.70f + p->recoil * 0.10f);
    start = *g->n;

    tdm_geo_box(g, o, v3(sp->recvW, sp->recvH, sp->recvL), 0.0f, 0x23272Eu);
    tdm_geo_box(g, v3(o.x, o.y + 0.02f, o.z - sp->recvL * 0.5f - sp->barrelL * 0.5f),
                v3(0.035f, 0.035f, sp->barrelL), 0.0f, 0x8B93A0u);
    tdm_geo_box(g, v3(o.x, o.y - 0.17f, o.z + 0.05f), v3(0.07f, 0.13f, 0.07f),
                0.0f, 0x6B4427u);
    tdm_geo_box(g, v3(o.x - 0.01f, o.y - 0.13f, o.z - 0.12f),
                v3(0.05f, 0.08f, 0.05f), 0.0f, 0x23272Eu);
    if (p->weapon == TDM_W_SNIPER)
        tdm_geo_box(g, v3(o.x, o.y + 0.12f, o.z - 0.05f),
                    v3(0.05f, 0.05f, 0.18f), 0.0f, 0x2B2F33u);

    count = *g->n - start;
    if (count <= 0) return;

    a = at(game->camPos);
    b = tdm_scene_rot_y(game->camYaw);
    a = m4_mul(&a, &b);
    b = tdm_scene_rot_x(game->camPitch);
    m = m4_mul(&a, &b);
    tdm_xform_impl(m.m, (float *)(g->v + start), (float *)(g->v + start),
                   (unsigned)count, 9, 9);
}
