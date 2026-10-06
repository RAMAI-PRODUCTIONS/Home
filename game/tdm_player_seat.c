#include "tdm_player.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_input.h"
#include "tdm_math.h"
#include "tdm_props.h"
#include "tdm_weapons.h"

void tdm_player_enter_exit(Game *g, InputState *in)
{
    Player *p = &g->player;
    int i;
    if (!in->interact) return;
    in->interact = 0;
    if (p->seatKind == 1) {
        Vehicle *v = &g->vehicles[p->seatIdx];
        v->driverIdx = -1;
        tdm_player_place(g, v->pos.x + 2.5f, v->pos.z + 2.5f);
        p->seatKind = 0;
        return;
    }
    if (p->seatKind == 2) {
        Turret *t = &g->turrets[p->seatIdx];
        t->gunnerIdx = -1;
        tdm_player_place(g, t->pos.x + 2.5f, t->pos.z + 2.5f);
        p->seatKind = 0;
        return;
    }
    for (i = 0; i < TDM_MAX_VEHICLES; i++) {
        Vehicle *v = &g->vehicles[i];
        if (!v->alive || v->driverIdx >= 0) continue;
        if (tdm_dist2d(v->pos, p->e.pos) < 4.0f) {
            v->driverIdx = 0;
            p->seatKind = 1;
            p->seatIdx = i;
            return;
        }
    }
    for (i = 0; i < TDM_MAX_TURRETS; i++) {
        Turret *t = &g->turrets[i];
        if (t->gunnerIdx >= 0) continue;
        if (tdm_dist2d(t->pos, p->e.pos) < 3.5f) {
            t->gunnerIdx = 0;
            p->seatKind = 2;
            p->seatIdx = i;
            return;
        }
    }
}
