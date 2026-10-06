#include "tdm_game.h"

#include "tdm_bot.h"
#include "tdm_combat.h"
#include "tdm_hud.h"
#include "tdm_input.h"
#include "tdm_map.h"
#include "tdm_math.h"
#include "tdm_player.h"
#include "tdm_props.h"
#include "tdm_rng.h"
#include "tdm_weapons.h"

static void airstrike(Game *g, float dt)
{
    Vec3 p;
    if (g->airLeft <= 0) return;
    g->airDropT -= dt;
    if (g->airDropT > 0.0f) return;
    g->airDropT = 0.35f;
    g->airLeft--;
    p = v3(g->airPos.x + tdm_rng_range(&g->rng, -6.0f, 6.0f), 20.0f,
           g->airPos.z + tdm_rng_range(&g->rng, -6.0f, 6.0f));
    p.y = tdm_terrain_height(&g->terrain, p.x, p.z) + 20.0f;
    tdm_explosion(g, p, 11.0f, 150.0f, 0);
}

void tdm_game_camera(Game *g)
{
    g->camShake = v3(0, 0, 0);
    if (g->shakeT <= 0.0f) return;
    {
        float s = g->shakeAmt * (g->shakeT / 0.35f);
        g->camShake = v3(tdm_rng_range(&g->rng, -s, s) * 0.5f,
                         tdm_rng_range(&g->rng, -s, s) * 0.5f,
                         tdm_rng_range(&g->rng, -s, s) * 0.5f);
    }
}

static void compact_grenades(Game *g)
{
    int i;
    for (i = 0; i < g->grenadeN;)
        if (!g->grenades[i].dead) i++;
        else g->grenades[i] = g->grenades[--g->grenadeN];
}

void tdm_game_tick(Game *g, float dt, InputState *in)
{
    InputState idle;
    int i;

    if (!g->started) {
        tdm_hud_taps(g, in);
        g->ui = *in;
        tdm_input_clear_frame(in);
        return;
    }
    tdm_hud_taps(g, in);
    g->time += dt;
    if (g->shakeT > 0.0f) g->shakeT -= dt;
    tdm_match_tick(&g->match, dt);
    if (g->screen == TDM_SCREEN_OVER) {
        tdm_input_init(&idle);
        in = &idle;
    }
    g->ui = *in;

    tdm_player_tick(g, in, dt);
    for (i = 0; i < TDM_BOTS_PER_TEAM * 2; i++) tdm_bot_tick(g, &g->bots[i], dt);
    for (i = 0; i < TDM_MAX_VEHICLES; i++) tdm_vehicle_update(g, &g->vehicles[i], dt);
    for (i = 0; i < TDM_MAX_PICKUPS; i++) tdm_pickup_update(g, &g->pickups[i], dt);
    for (i = 0; i < g->grenadeN; i++) tdm_grenade_update(g, &g->grenades[i], dt);
    compact_grenades(g);
    tdm_fx_step(&g->fx, dt, 9.0f);
    airstrike(g, dt);
    if (g->match.timeLeft <= 0.0f) tdm_match_end(g);
    tdm_game_camera(g);
    tdm_input_clear_frame(in);
}
