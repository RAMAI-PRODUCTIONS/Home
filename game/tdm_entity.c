#include "tdm_entity.h"

#include <stdio.h>
#include <string.h>

#include "tdm_game.h"
#include "tdm_math.h"

Entity *tdm_entity(Game *g, int index)
{
    if (index == 0) return &g->player.e;
    if (index >= 1 && index <= TDM_BOTS_PER_TEAM * 2)
        return &g->bots[index - 1].e;
    return 0;
}

int tdm_entity_index(const Game *g, const Entity *e)
{
    int i;
    if (e == &g->player.e) return 0;
    /* Bot embeds Entity as its first member: compare addresses, never
       subtract Entity pointers (the array stride is sizeof(Bot)). */
    for (i = 0; i < TDM_BOTS_PER_TEAM * 2; i++)
        if (e == &g->bots[i].e) return i + 1;
    return -1;
}

int tdm_entity_team_count(const Game *g, int team)
{
    int i, n = 0;
    for (i = 0; i < TDM_BOTS_PER_TEAM * 2; i++)
        if (g->bots[i].e.alive && g->bots[i].e.team == team) n++;
    return n;
}

Vec3 tdm_entity_eye(const Entity *e)
{
    return v3(e->pos.x, e->pos.y + TDM_EYE, e->pos.z);
}

void tdm_entity_hitboxes(const Entity *e, Vec3 *centers, float *radii)
{
    centers[0] = v3(e->pos.x, e->pos.y + 0.9f, e->pos.z);
    radii[0] = 0.55f;
    centers[1] = v3(e->pos.x, e->pos.y + 1.6f, e->pos.z);
    radii[1] = 0.30f;
}
