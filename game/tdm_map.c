#include "tdm_map.h"

#include "tdm_game.h"
#include "tdm_math.h"

void tdm_map_theme(Game *g, int theme)
{
    static const float fog[3][3] = {
        { 0.76f, 0.70f, 0.50f }, { 0.29f, 0.30f, 0.33f }, { 0.40f, 0.53f, 0.40f }
    };
    static const float sky[3][3] = {
        { 0.29f, 0.44f, 0.60f }, { 0.16f, 0.18f, 0.20f }, { 0.12f, 0.25f, 0.19f }
    };
    g->fogR = fog[theme][0];
    g->fogG = fog[theme][1];
    g->fogB = fog[theme][2];
    g->clearR = sky[theme][0];
    g->clearG = sky[theme][1];
    g->clearB = sky[theme][2];
    g->fogStart = 70.0f;
    g->fogEnd = 220.0f;
}

void tdm_map_spawns(Game *g)
{
    static const float zs[TDM_SPAWNS] = { -90.0f, 0.0f, 90.0f };
    int i;
    for (i = 0; i < TDM_SPAWNS; i++) {
        g->spawns[TDM_TEAM_BLUE][i] = v3(-90.0f, 0.0f, zs[i]);
        g->spawns[TDM_TEAM_RED][i] = v3(90.0f, 0.0f, zs[i]);
    }
}

void tdm_map_build(Game *g, int theme)
{
    g->theme = theme;
    tdm_world_reset(&g->world);
    tdm_map_theme(g, theme);
    tdm_map_spawns(g);
    if (theme == 0) tdm_map_desert(g);
    else if (theme == 1) tdm_map_urban(g);
    else tdm_map_forest(g);
    tdm_path_bake(&g->grid, &g->world);
    tdm_map_props(g);
}
