#include "tdm_weapons.h"

#include "tdm_rng.h"

const WeaponDef tdm_weapons[TDM_W_COUNT] = {
    { "PISTOL", 20, 28, 0.22f, 0.008f, 2.2f, 1.1f, 100, 0,
      12, 72, 1, 0 },
    { "SMG", 9, 14, 0.075f, 0.022f, 1.5f, 1.6f, 80, 0,
      32, 224, 1, 1 },
    { "RIFLE", 15, 24, 0.13f, 0.013f, 1.8f, 1.5f, 120, 0,
      30, 180, 1, 1 },
    { "SHOTGUN", 5, 9, 0.85f, 0.09f, 1.2f, 2.3f, 26, 0,
      6, 36, 9, 0 },
    { "SNIPER", 85, 105, 1.35f, 0.0018f, 2.5f, 2.4f, 240, 32,
      5, 30, 1, 0 }
};

const WeaponDef *tdm_weapon(int index)
{
    if (index < 0 || index >= TDM_W_COUNT) index = 0;
    return &tdm_weapons[index];
}

int tdm_weapon_pick(uint32_t *seed)
{
    return tdm_rng_int(seed, TDM_W_COUNT);
}
