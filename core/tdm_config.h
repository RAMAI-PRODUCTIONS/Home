#pragma once

/* Simulation constants. One place, no magic numbers anywhere else. */

#define TDM_MAP_SIZE 260.0f
#define TDM_HALF 130.0f
#define TDM_BOTS_PER_TEAM 6
#define TDM_ENTITY_COUNT (1 + TDM_BOTS_PER_TEAM * 2)
#define TDM_SCORE_LIMIT 50
#define TDM_MATCH_TIME 600.0f
#define TDM_PLAYER_SPEED 8.0f
#define TDM_SPRINT_SPEED 12.5f
#define TDM_BOT_SPEED 5.5f
#define TDM_BOT_SIGHT 90.0f
#define TDM_BOT_RANGE 28.0f
#define TDM_GRAVITY 22.0f
#define TDM_JUMP_VEL 8.0f
#define TDM_RADIUS 0.55f
#define TDM_EYE 1.55f

#define TDM_MAX_BOXES 512
#define TDM_MAX_RECTS 160
#define TDM_MAX_GRENADES 16
#define TDM_MAX_EFFECTS 192
#define TDM_MAX_TRACERS 48
#define TDM_MAX_FLASH 32
#define TDM_MAX_VEHICLES 4
#define TDM_MAX_TURRETS 4
#define TDM_MAX_PICKUPS 10
#define TDM_MAX_FEED 5
#define TDM_MAX_PATH 384

#define TDM_STATIC_VERTS 64000
#define TDM_DYN_VERTS 48000
#define TDM_HUD_VERTS 12000

#define TDM_PATH_CELL 2.0f
#define TDM_PATH_DIM 130
#define TDM_SPAWNS 3

#define TDM_COLOR_BLUE 0.20f, 0.40f, 1.0f
#define TDM_COLOR_RED 1.0f, 0.20f, 0.20f

/* Toggleable modes (runtime flags live in Game) */
#define TDM_UI_MIRRORED 0   /* mirror touch HUD for left-handed play */
#define TDM_FPS_MODE 1      /* show frames-per-second overlay */
#define TDM_GAUNTLET 0      /* auto-restart match, cycle maps endlessly */
