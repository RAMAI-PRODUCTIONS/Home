#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"
#include "tdm_weapons.h"

#define C_BLUE 0xFF3B7BFFu
#define C_RED 0xFFF05050u
#define C_WHITE 0xFFFFFFFFu
#define C_GOLD 0xFFFFD54Au
#define C_DIM 0xB0000000u

void tdm_hud_banner(const Game *g, DrawCtx *c)
{
    char line[48], clock[16];
    int m = (int)(g->match.timeLeft / 60.0f);
    int s = (int)g->match.timeLeft % 60;
    snprintf(line, sizeof line, "BLUE %d : %d RED", g->match.blue, g->match.red);
    snprintf(clock, sizeof clock, "%d:%02d", m, s);
    tdm2_rect(c, (float)c->w * 0.5f - 130.0f, 8.0f, 260.0f, 30.0f, C_DIM);
    tdm2_text_c(c, (float)c->w * 0.5f, 14.0f, 2.0f, C_WHITE, line);
    tdm2_text_c(c, (float)c->w * 0.5f, 44.0f, 1.5f, C_GOLD, clock);
    if (g->match.uav > 0.0f)
        tdm2_text_c(c, (float)c->w * 0.5f, 66.0f, 1.2f, 0xFF80FF80u, "UAV ONLINE");
}

void tdm_hud_killfeed(const Game *g, DrawCtx *c)
{
    int i;
    for (i = 0; i < TDM_MAX_FEED; i++) {
        char line[40];
        float a;
        uint32_t col;
        if (g->match.feedLife[i] <= 0.0f) continue;
        a = g->match.feedLife[i] > 1.0f ? 1.0f : g->match.feedLife[i];
        col = (uint32_t)(a * 180.0f) << 24;
        col |= g->match.feedTeam[i] == 0 ? 0x3B7BFFu
             : (g->match.feedTeam[i] == 1 ? 0xF05050u : 0xCCCCCCu);
        snprintf(line, sizeof line, "%s > %s", g->match.feed[i][0], g->match.feed[i][1]);
        tdm2_text(c, (float)c->w - tdm2_text_w(line, 1.4f) - 16.0f,
                  14.0f + (float)i * 22.0f, 1.4f, col, line);
    }
}
