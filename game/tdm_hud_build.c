#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"
#include "tdm_weapons.h"

#define C_BLUE 0xFF3B7BFFu
#define C_RED 0xFFF05050u
#define C_WHITE 0xFFFFFFFFu
#define C_GOLD 0xFFFFD54Au
#define C_DIM 0xB0000000u

void tdm_hud_match(const Game *g, DrawCtx *c)
{
    tdm_hud_banner(g, c);
    tdm_hud_killfeed(g, c);
    tdm_hud_vitals(g, c);
    tdm_hud_crosshair(g, c);
    tdm_hud_popups(g, c);
    tdm_hud_prompt(g, c);
    tdm_hud_minimap(g, c, 14.0f, 14.0f, 150.0f);
}

void tdm_hud_build(const Game *g, DrawCtx *c)
{
    if (g->screen == TDM_SCREEN_MENU || g->screen == TDM_SCREEN_OVER) {
        tdm_hud_overlays(g, c);
        return;
    }
    tdm_hud_match(g, c);
    tdm_hud_overlays(g, c);
    tdm_hud_touch(g, c);
}
