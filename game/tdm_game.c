#include "tdm_game.h"

#include <stdio.h>
#include <string.h>

#include "tdm_bot.h"
#include "tdm_combat.h"
#include "tdm_hud.h"
#include "tdm_input.h"
#include "tdm_map.h"
#include "tdm_math.h"
#include "tdm_player.h"
#include "tdm_props.h"
#include "tdm_rng.h"
#include "tdm_simd.h"
#include "tdm_weapons.h"

static void reset_round(Game *g)
{
    int i;
    g->time = 0.0f;
    g->overT = 0.0f;
    g->grenadeN = 0;
    g->shakeT = g->shakeAmt = 0.0f;
    g->airT = g->airDropT = 0.0f;
    g->airLeft = 0;
    tdm_fx_reset(&g->fx);
    tdm_match_reset(&g->match);
    for (i = 0; i < TDM_BOTS_PER_TEAM * 2; i++) tdm_bot_spawn(g, i);
}

static void init_player(Game *g)
{
    int k;
    memset(&g->player, 0, sizeof g->player);
    g->player.e.team = TDM_TEAM_BLUE;
    snprintf(g->player.e.name, sizeof g->player.e.name, "YOU");
    g->player.seatIdx = -1;
    for (k = 0; k < TDM_W_COUNT; k++) {
        g->player.mags[k] = tdm_weapons[k].mag;
        g->player.reserves[k] = tdm_weapons[k].reserveMax;
    }
    g->player.weapon = TDM_W_RIFLE;
}

void tdm_game_init(Game *g)
{
    memset(g, 0, sizeof *g);
    g->rng = 0xC0FFEE01u;
    tdm_simd_init();
    init_player(g);
    g->screen = TDM_SCREEN_MENU;
    g->camFov = 75.0f;
    g->screenW = 1920;   /* landscape defaults */
    g->screenH = 1080;
    g->uiMirrored = TDM_UI_MIRRORED;
    g->gauntlet = TDM_GAUNTLET;
    g->nextMap = 0;
    g->overT = 0.0f;
}

void tdm_game_start(Game *g, int theme)
{
    tdm_map_build(g, theme);
    reset_round(g);
    init_player(g);
    tdm_player_respawn(g);
    g->started = 1;
    g->mapGen++;
    g->screen = TDM_SCREEN_PLAY;
}

int tdm_game_spawn_index(Game *g, int team)
{
    (void)team;
    return tdm_rng_int(&g->rng, TDM_SPAWNS);
}

void tdm_game_streak(Game *g, int streak)
{
    Match *m = &g->match;
    if (streak == 3) {
        snprintf(m->streakText, sizeof m->streakText, "KILLING SPREE");
        m->streakPop = 1.6f;
        m->uav = 12.0f;
    } else if (streak == 5 || streak == 8) {
        snprintf(m->streakText, sizeof m->streakText, "AIRSTRIKE INCOMING");
        m->streakPop = 1.6f;
        m->uav = 15.0f;
        g->airT = 1.2f;
        g->airLeft = 4;
        g->airPos = v3(tdm_rng_range(&g->rng, -60.0f, 60.0f), 0.0f,
                       tdm_rng_range(&g->rng, -60.0f, 60.0f));
    }
}
