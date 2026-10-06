#pragma once

#include "tdm_entity.h"
#include "tdm_types.h"

typedef struct Game Game;

void tdm_bot_spawn(Game *g, int index);
void tdm_bot_tick(Game *g, Bot *b, float dt);

/* shared with tdm_bot_move.c */
void tdm_bot_patrol(Game *g, Bot *b);
void tdm_bot_repath(Game *g, Bot *b, Vec3 to);
int tdm_bot_sees(const Game *g, const Bot *b, const Entity *e);
void tdm_bot_act(Game *g, Bot *b, float dt);
void tdm_bot_think(Game *g, Bot *b, float dt);
void tdm_bot_attack(Game *g, Bot *b, Entity *target, float dt, float *mvx, float *mvz);
void tdm_bot_face(Game *g, Bot *b, float want, float dt);
