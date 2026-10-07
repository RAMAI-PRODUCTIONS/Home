#include "tdm_hud.h"

#include "tdm_game.h"
#include "tdm_input.h"

#define BTN 76.0f

int tdm_hud_buttons(const Game *g, HudBtn *out, int max)
{
    int n = 0;
    float W = (float)g->screenW, H = (float)g->screenH;
    if (g->screen == TDM_SCREEN_PLAY || g->screen == TDM_SCREEN_DEAD) {
        tdm_hud_layout_touch(g, out, max, &n);
    } else if (g->screen == TDM_SCREEN_MENU) {
        tdm_hud_layout_menu(g, out, max, &n);
    } else {
        tdm_hud_btn_add(out, &n, max, (W - 300.0f) * 0.5f, H * 0.68f, 300.0f, 90.0f, TDM_BTN_REPLAY);
        tdm_hud_btn_add(out, &n, max, (W - 280.0f) * 0.5f, H * 0.82f, 280.0f, 70.0f, TDM_BTN_GAUNTLET);
        tdm_hud_btn_add(out, &n, max, (W - 200.0f) * 0.5f, H * 0.91f, 200.0f, 50.0f, TDM_BTN_MIRROR);
    }
    return n;
}

int tdm_hud_hit(const Game *g, float x, float y)
{
    HudBtn btn[12];
    int n = tdm_hud_buttons(g, btn, 12), i;
    for (i = 0; i < n; i++)
        if (x >= btn[i].x && x <= btn[i].x + btn[i].w &&
            y >= btn[i].y && y <= btn[i].y + btn[i].h)
            return btn[i].id;
    return -1;
}

void tdm_hud_taps(Game *g, InputState *in)
{
    int id;
    if (!in->tap) return;
    id = tdm_hud_hit(g, in->tapX, in->tapY);
    in->tap = 0;
    if (id >= TDM_BTN_MAP0 && id <= TDM_BTN_MAP2 && g->screen == TDM_SCREEN_MENU)
        tdm_game_start(g, id - TDM_BTN_MAP0);
    else if (id == TDM_BTN_REPLAY && g->screen == TDM_SCREEN_OVER) {
        g->started = 0;
        g->screen = TDM_SCREEN_MENU;
    } else if (id == TDM_BTN_GAUNTLET && g->screen == TDM_SCREEN_OVER) {
        g->gauntlet = 1;
        g->nextMap = 0;
        g->overT = 0.0f;
        tdm_game_start(g, 0);
        g->nextMap = 1;
    } else if (id == TDM_BTN_MIRROR) {
        g->uiMirrored = !g->uiMirrored;
    }
}
