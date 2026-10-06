#pragma once

#include <stdint.h>

#include "tdm_config.h"

/* Score, timer, killfeed and killstreak state for one round. */

typedef struct Game Game;

typedef struct Match {
    float timeLeft;
    int blue, red;
    int state;          /* 0 running, 1 finished */
    int winner;         /* 0 blue, 1 red, 2 draw */
    char feed[TDM_MAX_FEED][2][12];
    uint8_t feedTeam[TDM_MAX_FEED];
    float feedLife[TDM_MAX_FEED];
    float uav;          /* seconds of radar remaining */
    float hitMark, dmgFlash;
    float xpPop;
    char xpText[16];
    float streakPop;
    char streakText[32];
} Match;

void tdm_match_reset(Match *m);
void tdm_match_tick(Match *m, float dt);
void tdm_match_push(Match *m, const char *a, const char *b, int team);
void tdm_match_kill(Game *g, int killerIdx, int victimIdx);
void tdm_match_end(Game *g);
