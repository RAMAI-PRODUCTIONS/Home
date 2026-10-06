#pragma once

#include "tdm_types.h"

/* Hitscan and splash damage, shared by player, bots, vehicles and turrets. */

typedef struct Game Game;
typedef struct WeaponDef WeaponDef;

/* mul scales the weapon spread, extra adds an absolute inaccuracy term. */
void tdm_fire(Game *g, int shooterIdx, Vec3 origin, Vec3 dir,
              const WeaponDef *def, float mul, float extra, int isPlayer);

void tdm_explosion(Game *g, Vec3 pos, float radius, float maxDmg, int ownerIdx);

int tdm_ray_actor(Game *g, int shooterIdx, Vec3 origin, Vec3 dir,
                  float maxDist, int *outIdx, float *outT, int *outHead);

void tdm_camera_shake(Game *g, float amount);
int tdm_ray_vehicle(Game *g, Vec3 origin, Vec3 dir, float maxT, int *outIdx, float *outT);
float tdm_ray_sphere(Vec3 o, Vec3 d, Vec3 c, float r, float maxT);
