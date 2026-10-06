#include "tdm_scene.h"

#include <math.h>

#include "tdm_game.h"
#include "tdm_math.h"
#include "tdm_terrain.h"

uint32_t tdm_ground_tint(const Game *g, float x, float z, float h, Vec3 n)
{
    const float *low, *high;
    static const float desert[3] = { 0x8A, 0x7C, 0x5E };
    static const float desertH[3] = { 0xA6, 0x92, 0x68 };
    static const float urban[3] = { 0x3A, 0x3A, 0x3A };
    static const float forest[3] = { 0x2D, 0x4A, 0x22 };
    static const float forestH[3] = { 0x4A, 0x6B, 0x3A };
    float t = tdm_clampf((h + 3.0f) / 7.0f, 0.0f, 1.0f);
    float r, gg, b, lit;
    int i;

    if (g->theme == 0) { low = desert; high = desertH; }
    else if (g->theme == 1) { low = urban; high = urban; }
    else { low = forest; high = forestH; }

    if (tdm_terrain_is_lake(&g->terrain, x, z, 1.0f))
        return 0xFF2A4A55u;
    if (tdm_terrain_is_road(&g->terrain, x, z))
        return g->theme == 1 ? 0xFF222222u : 0xFF5C503Du;

    r = low[0] + (high[0] - low[0]) * t;
    gg = low[1] + (high[1] - low[1]) * t;
    b = low[2] + (high[2] - low[2]) * t;
    lit = 0.45f + 0.55f * (n.x * TDM_SUN_X + n.y * TDM_SUN_Y + n.z * TDM_SUN_Z);
    if (lit < 0.2f) lit = 0.2f;
    i = (int)(r * lit) << 16 | (int)(gg * lit) << 8 | (int)(b * lit);
    return 0xFF000000u | (uint32_t)i;
}
