#include "tdm_entity.h"

#include <stdio.h>
#include <string.h>

#include "tdm_game.h"
#include "tdm_math.h"

void tdm_entity_damage(Game *g, Entity *victim, float dmg, int killerIdx)
{
    if (!victim || !victim->alive) return;
    victim->health -= dmg;
    victim->lastDamage = g->time;
    if (victim == &g->player.e) g->match.dmgFlash = 0.35f;
    if (victim->health <= 0.0f) tdm_entity_die(g, victim, killerIdx);
}

static void release_seat(Game *g, Entity *victim)
{
    Player *p = &g->player;
    if (victim != &p->e) return;
    if (p->seatKind == 1 && p->seatIdx >= 0)
        g->vehicles[p->seatIdx].driverIdx = -1;
    if (p->seatKind == 2 && p->seatIdx >= 0)
        g->turrets[p->seatIdx].gunnerIdx = -1;
    p->seatKind = 0;
    p->seatIdx = -1;
}

void tdm_entity_die(Game *g, Entity *victim, int killerIdx)
{
    int idx = tdm_entity_index(g, victim);
    Entity *killer = tdm_entity(g, killerIdx);
    if (!victim->alive || idx < 0) return;
    victim->alive = 0;
    victim->health = 0.0f;
    release_seat(g, victim);

    if (idx == 0) {
        Player *p = &g->player;
        p->deadTimer = 3.0f;
        p->streak = 0;
        snprintf(p->killerName, sizeof p->killerName, "%s",
                 killer ? killer->name : "WORLD");
        g->screen = TDM_SCREEN_DEAD;
    } else {
        g->bots[idx - 1].respawnT = 5.0f;
        g->bots[idx - 1].pathN = 0;
    }
    tdm_match_kill(g, killerIdx, idx);
}
