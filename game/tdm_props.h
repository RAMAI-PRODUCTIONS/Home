#pragma once

#include "tdm_config.h"
#include "tdm_types.h"

typedef struct Game Game;

typedef struct Vehicle {
    Vec3 pos, spawnPos;
    float yaw, spawnYaw, speed;
    float health, maxHealth, respawnT;
    int alive, driverIdx;           /* -1 = empty */
    float hitT[TDM_ENTITY_COUNT];   /* ram-damage cooldown per actor */
} Vehicle;

typedef struct Turret {
    Vec3 pos;
    float baseYaw;
    int gunnerIdx;                  /* -1 = empty */
} Turret;

typedef struct Pickup {
    Vec3 pos;
    float cooldown;
    int active, kind;               /* 0 ammo, 1 health */
} Pickup;

typedef struct Grenade {
    Vec3 pos, vel;
    float fuse;
    int ownerIdx, dead;
} Grenade;

void tdm_vehicle_drive(Game *g, Vehicle *v, float moveX, float moveY, float dt);
void tdm_vehicle_update(Game *g, Vehicle *v, float dt);
void tdm_vehicle_damage(Game *g, Vehicle *v, float dmg, int attackerIdx);
void tdm_vehicle_destroy(Game *g, Vehicle *v, int attackerIdx);
void tdm_pickup_update(Game *g, Pickup *p, float dt);
void tdm_grenade_throw(Game *g, int ownerIdx, Vec3 dir, float speed, float up);
void tdm_grenade_update(Game *g, Grenade *n, float dt);
