#include "tdm_scene.h"

#include "tdm_draw2d.h"
#include "tdm_game.h"
#include "tdm_hud.h"
#include "tdm_map.h"
#include "tdm_math.h"
#include "tdm_render.h"

static uint32_t box_tint(const Game *g, int type)
{
    static const uint32_t build[3] = { 0x9C825Cu, 0x606060u, 0x5C6B4Au };
    switch (type) {
    case TDM_T_TREE: return 0x5B4A32u;
    case TDM_T_BUILDING: return build[g->theme];
    case TDM_T_WALL: return g->theme == 1 ? 0x4A4A4Au : 0x8A7A55u;
    case TDM_T_CAR: return 0x333344u;
    case TDM_T_STAIR: return 0x666666u;
    case TDM_T_VISUAL: return g->theme == 2 ? 0x2F5A22u : 0x4A5A3Au;
    default: return 0x777777u;
    }
}

void tdm_scene_static(Geo *g, Game *game)
{
    int i;
    tdm_scene_terrain(g, game);
    for (i = 0; i < game->world.count; i++) {
        const Aabb *b = &game->world.boxes[i];
        Vec3 c = v3((b->min.x + b->max.x) * 0.5f,
                    (b->min.y + b->max.y) * 0.5f,
                    (b->min.z + b->max.z) * 0.5f);
        Vec3 half = v3((b->max.x - b->min.x) * 0.5f,
                       (b->max.y - b->min.y) * 0.5f,
                       (b->max.z - b->min.z) * 0.5f);
        tdm_geo_box(g, c, half, 0.0f, box_tint(game, b->type));
    }
}

void tdm_scene_dynamic(Geo *g, Game *game)
{
    tdm_scene_entities(g, game);
    tdm_scene_fx(g, game);
    tdm_scene_viewmodel(g, game);
}

void tdm_scene_build_static(Renderer *r, Game *game)
{
    Geo g;
    int n = 0;
    g.v = r->stc;
    g.n = &n;
    g.max = TDM_STATIC_VERTS;
    tdm_scene_static(&g, game);
    r->stcN = n;
    r->builtGen = game->mapGen;
}

void tdm_scene_render(Renderer *r, Game *game)
{
    Geo g;
    DrawCtx dc;
    int n = 0, hn = 0;

    game->screenW = (int)r->ext.width;
    game->screenH = (int)r->ext.height;
    g.v = r->dyn;
    g.n = &n;
    g.max = TDM_DYN_VERTS;
    tdm_scene_dynamic(&g, game);
    r->dynN = n;

    dc.v = (Vtx *)r->dyn + n;
    dc.n = &hn;
    dc.max = TDM_HUD_VERTS;
    dc.w = game->screenW;
    dc.h = game->screenH;
    tdm_hud_build(game, &dc);
    r->hudN = hn;
}
