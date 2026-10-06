#include "tdm_hud.h"

#include <stdio.h>

#include "tdm_game.h"
#include "tdm_weapons.h"

#define C_BLUE 0xFF3B7BFFu
#define C_RED 0xFFF05050u
#define C_WHITE 0xFFFFFFFFu
#define C_GOLD 0xFFFFD54Au
#define C_DIM 0xB0000000u

void tdm_hud_vitals(const Game *g, DrawCtx *c)
{
    const Player *p = &g->player;
    const WeaponDef *def = tdm_weapon(p->weapon);
    char txt[40];
    float hp = p->e.health < 0.0f ? 0.0f : (p->e.health > 100.0f ? 100.0f : p->e.health);

    tdm2_rect(c, 18.0f, (float)c->h - 56.0f, 200.0f, 26.0f, C_DIM);
    tdm2_rect(c, 20.0f, (float)c->h - 54.0f, 196.0f * hp / 100.0f, 22.0f, 0xFFD02020u);
    snprintf(txt, sizeof txt, "HP %d", (int)hp);
    tdm2_text_c(c, 118.0f, (float)c->h - 50.0f, 1.4f, C_WHITE, txt);

    snprintf(txt, sizeof txt, "%s", def->name);
    tdm2_text(c, 18.0f, (float)c->h - 96.0f, 1.6f, C_GOLD, txt);
    snprintf(txt, sizeof txt, "%d / %d", p->mags[p->weapon], p->reserves[p->weapon]);
    tdm2_text(c, 18.0f, (float)c->h - 76.0f, 1.6f, C_WHITE, txt);
    snprintf(txt, sizeof txt, "GREN %d", p->grenades);
    tdm2_text(c, 150.0f, (float)c->h - 76.0f, 1.4f, 0xFF9FE89Fu, txt);
    if (p->reloading)
        tdm2_text(c, 18.0f, (float)c->h - 116.0f, 1.4f, C_GOLD, "RELOADING");
}
