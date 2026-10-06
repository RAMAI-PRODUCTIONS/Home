#include "tdm_android.h"

#include <android/keycodes.h>

#include "tdm_game.h"

int32_t tdm_android_key(struct android_app *app, AInputEvent *ev)
{
    Game *g = g_tdm_game();
    int32_t code = AKeyEvent_getKeyCode(ev);
    (void)app;
    if (code != AKEYCODE_BACK || AKeyEvent_getAction(ev) != AKEY_EVENT_ACTION_UP)
        return 0;
    if (g->screen == TDM_SCREEN_PLAY || g->screen == TDM_SCREEN_DEAD) {
        g->started = 0;
        g->screen = TDM_SCREEN_MENU;
        return 1;
    }
    return 0;
}

int32_t tdm_android_input(struct android_app *app, AInputEvent *ev)
{
    if (AInputEvent_getType(ev) == AINPUT_EVENT_TYPE_KEY)
        return tdm_android_key(app, ev);
    return tdm_input_event(app, ev);
}
