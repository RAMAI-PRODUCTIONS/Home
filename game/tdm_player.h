#pragma once

#include "tdm_types.h"

typedef struct Game Game;
typedef struct InputState InputState;

Vec3 tdm_look_dir(const Game *g);

void tdm_player_tick(Game *g, InputState *in, float dt);
void tdm_player_respawn(Game *g);

/* gunplay, split out to keep both files small */
void tdm_player_fire(Game *g, InputState *in, float dt);
void tdm_player_reload(Game *g);
void tdm_player_reload_tick(Game *g, float dt);
void tdm_player_grenade(Game *g, InputState *in);
void tdm_player_vehicle_fire(Game *g, float dt);
void tdm_player_turret_fire(Game *g, float dt);
void tdm_player_apply_weapon(Game *g, int weapon);

/* on-foot, seated and spotting passes, split out the same way */
void tdm_player_place(Game *g, float x, float z);
void tdm_player_nearby(Game *g);
void tdm_player_enter_exit(Game *g, InputState *in);
void tdm_player_seated(Game *g, InputState *in, float dt);
void tdm_player_on_foot(Game *g, InputState *in, float dt);
void tdm_player_spot(Game *g, float dt);
