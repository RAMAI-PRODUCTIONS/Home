#pragma once

#include "tdm_draw2d.h"

typedef struct Game Game;
typedef struct InputState InputState;

enum {
    TDM_BTN_NONE = 0,
    TDM_BTN_FIRE = 1, TDM_BTN_JUMP, TDM_BTN_RELOAD,
    TDM_BTN_WPN, TDM_BTN_GREN, TDM_BTN_USE,
    TDM_BTN_MAP0 = 10, TDM_BTN_MAP1, TDM_BTN_MAP2,
    TDM_BTN_REPLAY = 20
};

typedef struct HudBtn {
    float x, y, w, h;
    int id;
} HudBtn;

void tdm_hud_build(const Game *g, DrawCtx *c);
int tdm_hud_buttons(const Game *g, HudBtn *out, int max);
int tdm_hud_hit(const Game *g, float x, float y);
void tdm_hud_taps(Game *g, InputState *in);

/* sub-renderers, split across files to honour the word limit */
void tdm_hud_match(const Game *g, DrawCtx *c);
void tdm_hud_touch(const Game *g, DrawCtx *c);
void tdm_hud_overlays(const Game *g, DrawCtx *c);
void tdm_hud_minimap(const Game *g, DrawCtx *c, float x, float y, float s);
void tdm_hud_banner(const Game *g, DrawCtx *c);
void tdm_hud_killfeed(const Game *g, DrawCtx *c);
void tdm_hud_vitals(const Game *g, DrawCtx *c);
void tdm_hud_crosshair(const Game *g, DrawCtx *c);
void tdm_hud_popups(const Game *g, DrawCtx *c);
void tdm_hud_prompt(const Game *g, DrawCtx *c);
void tdm_hud_menu_panel(const Game *g, DrawCtx *c);
void tdm_hud_menu_dead(const Game *g, DrawCtx *c);
void tdm_hud_menu_over(const Game *g, DrawCtx *c);
void tdm_hud_layout_touch(const Game *g, HudBtn *out, int max, int *n);
void tdm_hud_layout_menu(const Game *g, HudBtn *out, int max, int *n);
void tdm_hud_btn_add(HudBtn *out, int *n, int max, float x, float y, float w, float h, int id);
