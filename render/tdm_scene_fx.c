#include "tdm_scene.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_math.h"

void tdm_scene_fx(Geo *g, Game *game)
{
    const Fx *fx = &game->fx;
    Vec3 right = v3(cosf(game->camYaw), 0.0f, -sinf(game->camYaw));
    Vec3 up = v3(0.0f, 1.0f, 0.0f);
    int i;

    for (i = 0; i < fx->pN; i++) {
        float k = fx->pLife[i] / (fx->pMax[i] > 0.0f ? fx->pMax[i] : 1.0f);
        Vec3 p = v3(fx->pv[i][0], fx->pv[i][1], fx->pv[i][2]);
        tdm_geo_billboard(g, p, fx->pSize[i] * (0.4f + 0.6f * k),
                          fx->pTint[i], right, up);
    }
    for (i = 0; i < fx->tN; i++)
        tdm_geo_strip(g, fx->tA[i], fx->tB[i], 0.055f, fx->tTint[i]);
    for (i = 0; i < fx->fN; i++) {
        float k = 1.0f - fx->fLife[i] / (fx->fMax[i] > 0.0f ? fx->fMax[i] : 1.0f);
        tdm_geo_billboard(g, fx->fPos[i],
                          fx->fSize[i] * (1.0f + k * 7.0f), fx->fTint[i],
                          right, up);
    }
}
