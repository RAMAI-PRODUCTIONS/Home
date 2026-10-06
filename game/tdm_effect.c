#include "tdm_effect.h"

#include "tdm_math.h"
#include "tdm_rng.h"
#include "tdm_simd.h"

static uint32_t s_seed = 0x9E3779B9u;

void tdm_fx_reset(Fx *fx)
{
    fx->pN = fx->tN = fx->fN = 0;
}

void tdm_fx_particle(Fx *fx, Vec3 pos, Vec3 vel, float life, float size, uint32_t tint)
{
    int i;
    if (fx->pN >= TDM_MAX_EFFECTS) return;
    i = fx->pN++;
    fx->pv[i][0] = pos.x; fx->pv[i][1] = pos.y; fx->pv[i][2] = pos.z;
    fx->pv[i][3] = vel.x; fx->pv[i][4] = vel.y; fx->pv[i][5] = vel.z;
    fx->pLife[i] = life;
    fx->pMax[i] = life;
    fx->pSize[i] = size;
    fx->pTint[i] = tint;
}

void tdm_fx_tracer(Fx *fx, Vec3 a, Vec3 b, uint32_t tint)
{
    int i;
    if (fx->tN >= TDM_MAX_TRACERS) return;
    i = fx->tN++;
    fx->tA[i] = a;
    fx->tB[i] = b;
    fx->tLife[i] = 0.07f;
    fx->tTint[i] = tint;
}

void tdm_fx_flash(Fx *fx, Vec3 pos, float life, float size, uint32_t tint)
{
    int i;
    if (fx->fN >= TDM_MAX_FLASH) return;
    i = fx->fN++;
    fx->fPos[i] = pos;
    fx->fLife[i] = life;
    fx->fMax[i] = life;
    fx->fSize[i] = size;
    fx->fTint[i] = tint;
}

void tdm_fx_spark(Fx *fx, Vec3 pos, uint32_t tint, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        Vec3 v = v3(tdm_rng_range(&s_seed, -5.0f, 5.0f),
                    tdm_rng_range(&s_seed, 0.0f, 4.0f),
                    tdm_rng_range(&s_seed, -5.0f, 5.0f));
        tdm_fx_particle(fx, pos, v, tdm_rng_range(&s_seed, 0.3f, 0.6f), 0.1f, tint);
    }
}

void tdm_fx_explosion(Fx *fx, Vec3 pos)
{
    tdm_fx_flash(fx, pos, 0.4f, 1.0f, 0xFFAA33u);
    tdm_fx_spark(fx, pos, 0x333333u, 8);
    tdm_fx_spark(fx, pos, 0xFF6622u, 10);
}
