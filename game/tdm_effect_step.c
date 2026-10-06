#include "tdm_effect.h"

#include "tdm_math.h"
#include "tdm_rng.h"
#include "tdm_simd.h"

static void drop_particle(Fx *fx, int i)
{
    int last = --fx->pN, j;
    if (i == last) return;
    for (j = 0; j < 6; j++) fx->pv[i][j] = fx->pv[last][j];
    fx->pLife[i] = fx->pLife[last];
    fx->pMax[i] = fx->pMax[last];
    fx->pSize[i] = fx->pSize[last];
    fx->pTint[i] = fx->pTint[last];
}

static void drop_tracer(Fx *fx, int i)
{
    int last = --fx->tN;
    if (i == last) return;
    fx->tA[i] = fx->tA[last];
    fx->tB[i] = fx->tB[last];
    fx->tLife[i] = fx->tLife[last];
    fx->tTint[i] = fx->tTint[last];
}

static void drop_flash(Fx *fx, int i)
{
    int last = --fx->fN;
    if (i == last) return;
    fx->fPos[i] = fx->fPos[last];
    fx->fLife[i] = fx->fLife[last];
    fx->fMax[i] = fx->fMax[last];
    fx->fSize[i] = fx->fSize[last];
    fx->fTint[i] = fx->fTint[last];
}

void tdm_fx_step(Fx *fx, float dt, float gravity)
{
    int i;
    if (fx->pN > 0)
        tdm_particle_step_impl(&fx->pv[0][0], (unsigned)fx->pN, dt, gravity);
    for (i = 0; i < fx->pN; i++) fx->pLife[i] -= dt;
    for (i = 0; i < fx->tN; i++) fx->tLife[i] -= dt;
    for (i = 0; i < fx->fN; i++) fx->fLife[i] -= dt;

    for (i = 0; i < fx->pN;)
        if (fx->pLife[i] > 0.0f) i++;
        else drop_particle(fx, i);
    for (i = 0; i < fx->tN;)
        if (fx->tLife[i] > 0.0f) i++;
        else drop_tracer(fx, i);
    for (i = 0; i < fx->fN;)
        if (fx->fLife[i] > 0.0f) i++;
        else drop_flash(fx, i);
}
