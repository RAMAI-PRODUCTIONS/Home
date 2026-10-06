#pragma once

#include "tdm_config.h"
#include "tdm_types.h"
#include "tdm_weapons.h"

/* Shared actor state. Index 0 is always the player, 1..12 are bots. */

typedef struct Game Game;

typedef struct Entity {
    Vec3 pos, vel;
    float yaw;
    float health;
    float lastDamage;
    int alive, team;
    char name[12];
} Entity;

typedef struct Player {
    Entity e;
    float pitch, vy;
    float fireCooldown, recoil, deadTimer;
    float reloadT;
    int grounded, weapon, grenades, streak, reloading, ads;
    int mags[TDM_W_COUNT], reserves[TDM_W_COUNT];
    int seatKind;   /* 0 on foot, 1 vehicle, 2 turret */
    int seatIdx;
    int nearKind;   /* 0 none, 1 vehicle, 2 turret */
    int nearIdx;
    float spotT;
    char killerName[12];
    float spotted[TDM_BOTS_PER_TEAM * 2];
    Vec3 gunBob;
} Player;

typedef struct Bot {
    Entity e;
    int state;      /* 0 patrol, 1 chase, 2 attack */
    int targetIdx;
    Vec3 path[TDM_MAX_PATH];
    int pathN, pathI, stuck;
    float nextThink, nextPath, fireDelay, lastShot;
    float strafeT, strafeDir, respawnT, lastGrenade;
    float accuracy;
    int weapon, mag, moving;
} Bot;

Entity *tdm_entity(Game *g, int index);
int tdm_entity_index(const Game *g, const Entity *e);
int tdm_entity_team_count(const Game *g, int team);
void tdm_entity_damage(Game *g, Entity *victim, float dmg, int killerIdx);
void tdm_entity_hitboxes(const Entity *e, Vec3 *centers, float *radii);
Vec3 tdm_entity_eye(const Entity *e);
void tdm_entity_die(Game *g, Entity *victim, int killerIdx);
