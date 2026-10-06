#include "tdm_match.h"

#include <stdio.h>
#include <string.h>

#include "tdm_game.h"

void tdm_match_reset(Match *m)
{
    memset(m, 0, sizeof *m);
    m->timeLeft = TDM_MATCH_TIME;
}

void tdm_match_tick(Match *m, float dt)
{
    int i;
    if (m->state) return;
    m->timeLeft -= dt;
    if (m->timeLeft < 0.0f) m->timeLeft = 0.0f;
    if (m->uav > 0.0f) m->uav -= dt;
    if (m->hitMark > 0.0f) m->hitMark -= dt;
    if (m->dmgFlash > 0.0f) m->dmgFlash -= dt;
    if (m->xpPop > 0.0f) m->xpPop -= dt;
    if (m->streakPop > 0.0f) m->streakPop -= dt;
    for (i = 0; i < TDM_MAX_FEED; i++)
        if (m->feedLife[i] > 0.0f) m->feedLife[i] -= dt;
}

void tdm_match_push(Match *m, const char *a, const char *b, int team)
{
    int i;
    for (i = TDM_MAX_FEED - 1; i > 0; i--) {
        memcpy(m->feed[i], m->feed[i - 1], sizeof m->feed[i]);
        m->feedTeam[i] = m->feedTeam[i - 1];
        m->feedLife[i] = m->feedLife[i - 1];
    }
    snprintf(m->feed[0][0], sizeof m->feed[0][0], "%s", a);
    snprintf(m->feed[0][1], sizeof m->feed[0][1], "%s", b);
    m->feedTeam[0] = (uint8_t)team;
    m->feedLife[0] = 5.0f;
}

void tdm_match_end(Game *g)
{
    Match *m = &g->match;
    if (m->state) return;
    m->state = 1;
    m->winner = m->blue > m->red ? 0 : (m->red > m->blue ? 1 : 2);
    g->screen = TDM_SCREEN_OVER;
}

void tdm_match_kill(Game *g, int killerIdx, int victimIdx)
{
    Match *m = &g->match;
    Entity *killer = tdm_entity(g, killerIdx);
    Entity *victim = tdm_entity(g, victimIdx);
    if (!victim) return;

    if (victim->team == TDM_TEAM_BLUE) m->red++;
    else m->blue++;
    tdm_match_push(m, killer ? killer->name : "WORLD", victim->name,
                   killer ? killer->team : 2);

    if (killer == &g->player.e) {
        Player *p = &g->player;
        snprintf(m->xpText, sizeof m->xpText, "+100 XP");
        m->xpPop = 0.9f;
        p->streak++;
        tdm_game_streak(g, p->streak);
    }
    if (m->blue >= TDM_SCORE_LIMIT || m->red >= TDM_SCORE_LIMIT)
        tdm_match_end(g);
}
