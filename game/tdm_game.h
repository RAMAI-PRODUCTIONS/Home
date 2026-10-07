#pragma once

#include <stdint.h>

#include "tdm_config.h"
#include "tdm_effect.h"
#include "tdm_entity.h"
#include "tdm_input.h"
#include "tdm_match.h"
#include "tdm_props.h"
#include "tdm_terrain.h"
#include "tdm_collision.h"
#include "tdm_pathfind.h"

enum { TDM_SCREEN_MENU = 0, TDM_SCREEN_PLAY = 1,
       TDM_SCREEN_DEAD = 2, TDM_SCREEN_OVER = 3 };

/* The whole simulation lives here. One instance, no heap. */
struct Game {
    uint32_t rng;
    int theme;                      /* 0 desert, 1 urban, 2 forest */
    Terrain terrain;
    World world;
    PathGrid grid;
    Vec3 spawns[2][TDM_SPAWNS];

    Player player;
    Bot bots[TDM_BOTS_PER_TEAM * 2];

    Vehicle vehicles[TDM_MAX_VEHICLES];
    Turret turrets[TDM_MAX_TURRETS];
    Pickup pickups[TDM_MAX_PICKUPS];
    Grenade grenades[TDM_MAX_GRENADES];
    int grenadeN;

    Fx fx;
    Match match;
    InputState ui;          /* last frame's input, for HUD widgets */

    float time;
    int screen, started;
    int mapGen;            /* bumped on start so the renderer rebuilds */
    int uiMirrored;        /* 1 = HUD/sticks flipped for left-handed play */
    float fps;
    int gauntlet;          /* 1 = auto-restart, cycle maps endlessly */
    int nextMap;           /* gauntlet: next theme to deploy */
    float overT;           /* seconds spent on over/dead screen */
    float shakeT, shakeAmt;
    float airT, airDropT;
    int airLeft;
    Vec3 airPos;

    /* frame view, written by tdm_game_camera() */
    Vec3 camPos, camShake;
    float camYaw, camPitch, camFov, camRoll;
    float fogStart, fogEnd, fogR, fogG, fogB;
    float clearR, clearG, clearB;
    int screenW, screenH;
};

void tdm_game_init(Game *g);
void tdm_game_start(Game *g, int theme);
void tdm_game_tick(Game *g, float dt, InputState *in);
void tdm_game_camera(Game *g);
int tdm_game_spawn_index(Game *g, int team);
void tdm_game_streak(Game *g, int streak);
