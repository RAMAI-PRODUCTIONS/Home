#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"
#include "tdm_weapons.h"

#define C_BLUE 0xFF3B7BFFu
#define C_RED 0xFFF05050u
#define C_WHITE 0xFFFFFFFFu
#define C_GOLD 0xFFFFD54Au
#define C_DIM 0xB0000000u

void tdm_hud_crosshair(const Game *g, DrawCtx *c)
{
    float cx = (float)c->w * 0.5f, cy = (float)c->h * 0.5f;
    float a = 0.9f * 255.0f;
    uint32_t col = (uint32_t)a << 24 | 0xFFFFFFu;
    float gap = g->player.ads && tdm_weapon(g->player.weapon)->zoomFov > 0.0f ? 4.0f : 12.0f;
    tdm2_line(c, cx, cy - gap - 10.0f, cx, cy - gap, 2.0f, col);
    tdm2_line(c, cx, cy + gap, cx, cy + gap + 10.0f, 2.0f, col);
    tdm2_line(c, cx - gap - 10.0f, cy, cx - gap, cy, 2.0f, col);
    tdm2_line(c, cx + gap, cy, cx + gap + 10.0f, cy, 2.0f, col);
    if (g->match.hitMark > 0.0f) {
        uint32_t hm = 0xFFFFFFFFu;
        tdm2_line(c, cx - 9.0f, cy - 9.0f, cx - 3.0f, cy - 3.0f, 3.0f, hm);
        tdm2_line(c, cx + 9.0f, cy - 9.0f, cx + 3.0f, cy - 3.0f, 3.0f, hm);
        tdm2_line(c, cx - 9.0f, cy + 9.0f, cx - 3.0f, cy + 3.0f, 3.0f, hm);
        tdm2_line(c, cx + 9.0f, cy + 9.0f, cx + 3.0f, cy + 3.0f, 3.0f, hm);
    }
}
