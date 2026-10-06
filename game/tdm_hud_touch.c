#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"

static const char *label(int id)
{
    switch (id) {
    case TDM_BTN_FIRE: return "FIRE";
    case TDM_BTN_JUMP: return "JUMP";
    case TDM_BTN_RELOAD: return "RLD";
    case TDM_BTN_WPN: return "WPN";
    case TDM_BTN_GREN: return "GREN";
    case TDM_BTN_USE: return "USE";
    default: return "?";
    }
}

static uint32_t tint(int id)
{
    switch (id) {
    case TDM_BTN_FIRE: return 0xA0FF5030u;
    case TDM_BTN_GREN: return 0xA040C060u;
    case TDM_BTN_USE: return 0xA0D0B030u;
    default: return 0x70FFFFFFu;
    }
}

static void stick(DrawCtx *c, int on, float ox, float oy, float dx, float dy)
{
    if (!on) return;
    tdm2_disc(c, ox, oy, 64.0f, 0x50000000u);
    tdm2_disc(c, ox, oy, 60.0f, 0x40FFFFFFu);
    tdm2_disc(c, ox + dx, oy + dy, 26.0f, 0xC0FFFFFFu);
}

void tdm_hud_touch(const Game *g, DrawCtx *c)
{
    HudBtn btn[12];
    int n = tdm_hud_buttons(g, btn, 12), i;
    const InputState *in = &g->ui;

    stick(c, in->stickL, in->stickLX, in->stickLY, in->stickLDX, in->stickLDY);
    stick(c, in->stickR, in->stickRX, in->stickRY, in->stickRDX, in->stickRDY);

    for (i = 0; i < n; i++) {
        HudBtn *b = &btn[i];
        float cx = b->x + b->w * 0.5f, cy = b->y + b->h * 0.5f;
        uint32_t col = tint(b->id);
        if (in->btnMask & (1u << b->id)) col |= 0x60FFFFFFu;
        tdm2_disc(c, cx, cy, b->w * 0.46f, col);
        tdm2_disc(c, cx, cy, b->w * 0.46f - 2.0f, 0x20000000u);
        tdm2_text_c(c, cx, cy - 6.0f, 1.3f, 0xFFFFFFFFu, label(b->id));
    }
}
