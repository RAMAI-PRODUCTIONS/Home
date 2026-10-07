#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"

#define C_GREEN 0xFF30FF60u

void tdm_hud_fps(const Game *g, DrawCtx *c)
{
#if TDM_FPS_MODE
    char txt[32];
    float fps = g->fps;
    if (fps <= 0.0f) fps = 0.0f;
    snprintf(txt, sizeof txt, "%.0fFPS", fps);
    tdm2_text(c, (float)c->w - 120.0f, 16.0f, 1.6f, C_GREEN, txt);
#endif
}
