#pragma once

#include <stdint.h>

/* Five weapons, ported one-for-one from the reference prototype. */

typedef struct WeaponDef {
    const char *name;
    float dmgMin, dmgMax, fireRate, spread;
    float headMult, reloadTime, range, zoomFov;
    int mag, reserveMax, pellets, automatic;
} WeaponDef;

enum {
    TDM_W_PISTOL = 0,
    TDM_W_SMG = 1,
    TDM_W_RIFLE = 2,
    TDM_W_SHOTGUN = 3,
    TDM_W_SNIPER = 4,
    TDM_W_COUNT = 5
};

extern const WeaponDef tdm_weapons[TDM_W_COUNT];

const WeaponDef *tdm_weapon(int index);
int tdm_weapon_pick(uint32_t *seed);
