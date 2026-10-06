#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"

static const char *s_maps[3][2] = {
    { "DUSTY OUTPOST", "OPEN SIGHTLINES" },
    { "URBAN RUINS", "TIGHT CORRIDORS" },
    { "FOREST COMPOUND", "DENSE COVER" }
};

void tdm_hud_menu_panel(const Game *g, DrawCtx *c)
{
    float cx = (float)c->w * 0.5f, W = (float)c->w, H = (float)c->h;
    HudBtn btn[12];
    int n = tdm_hud_buttons(g, btn, 12), i;

    tdm2_rect(c, 0, 0, W, H, 0xE0060A12u);
    tdm2_text_c(c, cx, H * 0.10f, 4.0f, 0xFFFFFFFFu, "PROCEDURAL WARFARE");
    tdm2_text_c(c, cx, H * 0.10f + 42.0f, 1.8f, 0xFFFFD54Au,
                "TEAM DEATHMATCH - FIRST TO 50");
    tdm2_text_c(c, cx, H * 0.10f + 74.0f, 1.4f, 0xFF9AA6B8u,
                "LEFT STICK MOVE - RIGHT STICK LOOK AND FIRE");

    for (i = 0; i < n; i++) {
        HudBtn *b = &btn[i];
        int m = b->id - TDM_BTN_MAP0;
        float mx = b->x + b->w * 0.5f;
        tdm2_rect(c, b->x, b->y, b->w, b->h, 0xC01B2634u);
        tdm2_rect(c, b->x, b->y, b->w, 4.0f, 0xFFFFD54Au);
        tdm2_text_c(c, mx, b->y + 40.0f, 1.7f, 0xFFFFFFFFu, s_maps[m][0]);
        tdm2_text_c(c, mx, b->y + 78.0f, 1.2f, 0xFF9AA6B8u, s_maps[m][1]);
        tdm2_text_c(c, mx, b->y + 118.0f, 1.4f, 0xFFFFD54Au, "DEPLOY");
    }
}

void tdm_hud_overlays(const Game *g, DrawCtx *c)
{
    if (g->screen == TDM_SCREEN_MENU) tdm_hud_menu_panel(g, c);
    else if (g->screen == TDM_SCREEN_DEAD) tdm_hud_menu_dead(g, c);
    else if (g->screen == TDM_SCREEN_OVER) tdm_hud_menu_over(g, c);
}
