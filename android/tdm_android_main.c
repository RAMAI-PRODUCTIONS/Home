#include "tdm_android.h"

#include <android/keycodes.h>
#include <android/log.h>
#include <time.h>

#include "tdm_game.h"
#include "tdm_render.h"
#include "tdm_scene.h"
#include "tdm_simd.h"

static Game s_game;
static Renderer s_ren;
static int64_t s_last;
static float s_fpsTimer;
static int s_frameCount;

struct Game *g_tdm_game(void)
{
    return &s_game;
}

static int64_t now_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

static void on_cmd(struct android_app *app, int32_t cmd)
{
    switch (cmd) {
    case APP_CMD_INIT_WINDOW:
    case APP_CMD_WINDOW_RESIZED:
    case APP_CMD_WINDOW_REDRAW_NEEDED:
    case APP_CMD_CONFIG_CHANGED:
        if (!app->window) break;
        if (s_ren.valid) {
            tdm_vk_resize(&s_ren, app->window);
        } else {
            tdm_vk_destroy(&s_ren);
            if (!tdm_vk_create(&s_ren, app->window))
                __android_log_print(ANDROID_LOG_WARN, "TDM", "vulkan init deferred");
        }
        s_last = now_ns();
        break;
    case APP_CMD_TERM_WINDOW:
        if (s_ren.dev) tdm_vk_resize(&s_ren, NULL);
        break;
    case APP_CMD_GAINED_FOCUS:
        s_last = now_ns();
        break;
    default:
        break;
    }
}

void android_main(struct android_app *app)
{
    app->onAppCmd = on_cmd;
    app->onInputEvent = tdm_android_input;
    tdm_game_init(&s_game);
    {
        float worst = 0.0f;
        if (!tdm_simd_selftest(&worst))
            __android_log_print(ANDROID_LOG_ERROR, "TDM",
                                "simd selftest failed (%.6f)", worst);
    }
    tdm_input_reset();
    s_last = now_ns();

    for (;;) {
        struct android_poll_source *src = NULL;
        int ident;
        int timeout = s_ren.valid ? 0 : 50;
        while ((ident = ALooper_pollOnce(timeout, NULL, NULL, (void **)&src)) >= 0) {
            if (src) src->process(app, src);
            if (app->destroyRequested) {
                tdm_vk_destroy(&s_ren);
                return;
            }
            timeout = 0;
        }
        if (!app->window) continue;
        if (!s_ren.valid) {
            tdm_vk_destroy(&s_ren);
            tdm_vk_create(&s_ren, app->window);
            s_last = now_ns();
            continue;
        }
        {
            int64_t now = now_ns();
            float dt = (float)((now - s_last) / 1e9);
            s_last = now;
            if (dt > 0.05f) dt = 0.05f;
            if (dt < 0.0f) dt = 0.0f;
            tdm_input_look_step(dt);
            tdm_game_tick(&s_game, dt, &tdm_in);
            s_frameCount++;
            s_fpsTimer += dt;
            if (s_fpsTimer >= 1.0f) {
                s_game.fps = (float)s_frameCount / s_fpsTimer;
                s_frameCount = 0; s_fpsTimer = 0.0f;
            }
        }
        if (s_ren.builtGen != s_game.mapGen)
            tdm_scene_build_static(&s_ren, &s_game);
        tdm_scene_render(&s_ren, &s_game);
        tdm_vk_frame(&s_ren, &s_game);
    }
}
