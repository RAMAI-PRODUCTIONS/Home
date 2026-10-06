#include "tdm_player.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

void tdm_player_spot(Game *g, float dt)
{
    Player *p = &g->player;
    int i;
    p->spotT -= dt;
    if (p->spotT > 0.0f) return;
    p->spotT = 0.3f;
    for (i = 0; i < TDM_BOTS_PER_TEAM * 2; i++) {
        Bot *b = &g->bots[i];
        if (!b->e.alive || b->e.team == p->e.team) continue;
        if (tdm_dist2d(b->e.pos, p->e.pos) < 50.0f &&
            tdm_world_los(&g->world, tdm_entity_eye(&p->e), tdm_entity_eye(&b->e)))
            p->spotted[i] = g->time;
    }
}
