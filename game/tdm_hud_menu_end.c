#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"

void tdm_hud_menu_dead(const Game *g, DrawCtx *c)
{
    char txt[40];
    float cx = (float)c->w * 0.5f, H = (float)c->h;
    tdm2_rect(c, 0, 0, (float)c->w, H, 0x66400000u);
    tdm2_text_c(c, cx, H * 0.30f, 4.5f, 0xFFFFFFFFu, "YOU DIED");
    snprintf(txt, sizeof txt, "KILLED BY %s", g->player.killerName);
    tdm2_text_c(c, cx, H * 0.30f + 56.0f, 1.8f, 0xFFFF8080u, txt);
    snprintf(txt, sizeof txt, "RESPAWNING IN %d", (int)(g->player.deadTimer + 0.999f));
    tdm2_text_c(c, cx, H * 0.30f + 92.0f, 1.8f, 0xFFFFFFFFu, txt);
}

void tdm_hud_menu_over(const Game *g, DrawCtx *c)
{
    char txt[48];
    float cx = (float)c->w * 0.5f, H = (float)c->h;
    int w = g->match.winner;
    tdm2_rect(c, 0, 0, (float)c->w, H, 0xCC05070Cu);
    tdm2_text_c(c, cx, H * 0.22f, 5.0f, w == 0 ? 0xFF3B7BFFu
                : (w == 1 ? 0xFFF05050u : 0xFFFFFFFFu),
                w == 2 ? "DRAW" : (w == 0 ? "VICTORY" : "DEFEAT"));
    snprintf(txt, sizeof txt, "BLUE %d : %d RED", g->match.blue, g->match.red);
    tdm2_text_c(c, cx, H * 0.22f + 70.0f, 2.4f, 0xFFFFFFFFu, txt);
    tdm2_text_c(c, cx, H * 0.68f + 30.0f, 1.8f, 0xFFFFD54Au, "TAP TO PLAY AGAIN");
}
