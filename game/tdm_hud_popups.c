#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"
#include "tdm_weapons.h"

#define C_BLUE 0xFF3B7BFFu
#define C_RED 0xFFF05050u
#define C_WHITE 0xFFFFFFFFu
#define C_GOLD 0xFFFFD54Au
#define C_DIM 0xB0000000u

void tdm_hud_popups(const Game *g, DrawCtx *c)
{
    float cx = (float)c->w * 0.5f;
    if (g->match.dmgFlash > 0.0f) {
        float a = g->match.dmgFlash * 1.6f;
        if (a > 0.55f) a = 0.55f;
        tdm2_rect(c, 0, 0, (float)c->w, (float)c->h,
                  ((uint32_t)(a * 255.0f) << 24) | 0xFF0000u);
    }
    if (g->match.xpPop > 0.0f)
        tdm2_text_c(c, cx, (float)c->h * 0.40f, 2.4f, C_GOLD, g->match.xpText);
    if (g->match.streakPop > 0.0f)
        tdm2_text_c(c, cx, (float)c->h * 0.24f, 2.6f, 0xFFFF5A3Cu, g->match.streakText);
}

void tdm_hud_prompt(const Game *g, DrawCtx *c)
{
    const Player *p = &g->player;
    const char *msg = 0;
    if (p->seatKind == 1) msg = "USE TO EXIT VEHICLE";
    else if (p->seatKind == 2) msg = "USE TO EXIT TURRET";
    else if (p->nearKind == 1) msg = "USE TO ENTER VEHICLE";
    else if (p->nearKind == 2) msg = "USE TO MAN TURRET";
    if (msg) tdm2_text_c(c, (float)c->w * 0.5f, (float)c->h - 360.0f, 1.6f, C_WHITE, msg);
}
