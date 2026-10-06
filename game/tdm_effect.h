#pragma once

#include <stdint.h>

#include "tdm_config.h"
#include "tdm_types.h"

/* Structure of arrays so the NEON particle kernel can step every spark in
   one pass. Tracers are lines, flashes are billboards (muzzle/explosion). */

typedef struct Fx {
    float pv[TDM_MAX_EFFECTS][6];
    float pLife[TDM_MAX_EFFECTS];
    float pMax[TDM_MAX_EFFECTS];
    float pSize[TDM_MAX_EFFECTS];
    uint32_t pTint[TDM_MAX_EFFECTS];
    int pN;

    Vec3 tA[TDM_MAX_TRACERS], tB[TDM_MAX_TRACERS];
    float tLife[TDM_MAX_TRACERS];
    uint32_t tTint[TDM_MAX_TRACERS];
    int tN;

    Vec3 fPos[TDM_MAX_FLASH];
    float fLife[TDM_MAX_FLASH], fMax[TDM_MAX_FLASH], fSize[TDM_MAX_FLASH];
    uint32_t fTint[TDM_MAX_FLASH];
    int fN;
} Fx;

void tdm_fx_reset(Fx *fx);
void tdm_fx_particle(Fx *fx, Vec3 pos, Vec3 vel, float life, float size, uint32_t tint);
void tdm_fx_tracer(Fx *fx, Vec3 a, Vec3 b, uint32_t tint);
void tdm_fx_flash(Fx *fx, Vec3 pos, float life, float size, uint32_t tint);
void tdm_fx_spark(Fx *fx, Vec3 pos, uint32_t tint, int n);
void tdm_fx_explosion(Fx *fx, Vec3 pos);
void tdm_fx_step(Fx *fx, float dt, float gravity);
