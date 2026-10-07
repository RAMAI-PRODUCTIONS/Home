#include "tdm_hud.h"

#include "tdm_game.h"
#include "tdm_input.h"

#define BTN 76.0f

void tdm_hud_btn_add(HudBtn *out, int *n, int max, float x, float y, float w, float h, int id)
{
    if (*n >= max) return;
    out[*n].x = x;
    out[*n].y = y;
    out[*n].w = w;
    out[*n].h = h;
    out[*n].id = id;
    (*n)++;
}

void tdm_hud_layout_touch(const Game *g, HudBtn *out, int max, int *n)
{
    float W = (float)g->screenW, H = (float)g->screenH;
    float x0, x1, x2;
    if (g->uiMirrored) { x0 = 84.0f; x1 = x0 + BTN + 8.0f; x2 = x1 + BTN + 8.0f;
    } else { x0 = W - 84.0f - BTN; x1 = x0 - BTN - 8.0f; x2 = x1 - BTN - 8.0f; }
    float y0 = H - 100.0f - BTN, y1 = y0 - BTN - 8.0f;
    tdm_hud_btn_add(out, n, max, x0, y0, BTN, BTN, TDM_BTN_FIRE);
    tdm_hud_btn_add(out, n, max, x1, y0, BTN, BTN, TDM_BTN_JUMP);
    tdm_hud_btn_add(out, n, max, x2, y0, BTN, BTN, TDM_BTN_GREN);
    tdm_hud_btn_add(out, n, max, x0, y1, BTN, BTN, TDM_BTN_RELOAD);
    tdm_hud_btn_add(out, n, max, x1, y1, BTN, BTN, TDM_BTN_USE);
    tdm_hud_btn_add(out, n, max, x2, y1, BTN, BTN, TDM_BTN_WPN);
}

void tdm_hud_layout_menu(const Game *g, HudBtn *out, int max, int *n)
{
    float W = (float)g->screenW, H = (float)g->screenH;
    float bw = 300.0f, bh = 170.0f, gap = 24.0f;
    int i;
    if (W >= (bw + gap) * 3.0f) {
        float sx = (W - (bw * 3.0f + gap * 2.0f)) * 0.5f;
        for (i = 0; i < 3; i++)
            tdm_hud_btn_add(out, n, max, sx + (float)i * (bw + gap), H * 0.55f, bw, bh,
                TDM_BTN_MAP0 + i);
    } else {
        float sx = (W - bw) * 0.5f;
        for (i = 0; i < 3; i++)
            tdm_hud_btn_add(out, n, max, sx, H * 0.40f + (float)i * (bh + 12.0f), bw, bh,
                TDM_BTN_MAP0 + i);
    }
}
