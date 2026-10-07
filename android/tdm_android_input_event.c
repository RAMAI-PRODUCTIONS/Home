#include "tdm_android.h"

#include "tdm_game.h"
#include "tdm_hud.h"
#include "tdm_input.h"

int32_t tdm_input_event(struct android_app *app, AInputEvent *ev)
{
    int32_t type = AInputEvent_getType(ev);
    int action, raw, idx, id, s, i;
    float W, x, y;

    if (type != AINPUT_EVENT_TYPE_MOTION) return 0;
    if (AInputEvent_getSource(ev) != AINPUT_SOURCE_TOUCHSCREEN) return 0;
    raw = AMotionEvent_getAction(ev);
    action = raw & AMOTION_EVENT_ACTION_MASK;
    idx = (raw & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
          AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    id = AMotionEvent_getPointerId(ev, idx);
    W = (float)(app->contentRect.right - app->contentRect.left);
    x = AMotionEvent_getX(ev, idx);
    y = AMotionEvent_getY(ev, idx);

    if (g_tdm_game()->screen != TDM_SCREEN_PLAY &&
        g_tdm_game()->screen != TDM_SCREEN_DEAD) {
        if (action == AMOTION_EVENT_ACTION_DOWN) {
            tdm_in.tap = 1;
            tdm_in.tapX = x;
            tdm_in.tapY = y;
        }
        return 1;
    }

    if (action == AMOTION_EVENT_ACTION_DOWN ||
        action == AMOTION_EVENT_ACTION_POINTER_DOWN) {
        int btn = tdm_hud_hit(g_tdm_game(), x, y);
        s = tdm_slot_free();
        if (s < 0) return 1;
        s_pid[s] = id;
        if (btn > 0) {
            s_role[s] = 3;
            s_bid[s] = btn;
            tdm_ptr_press(btn);
        } else if (g_tdm_game()->uiMirrored ? (x > W * 0.55f) : (x < W * 0.45f)) {
            s_role[s] = 1;
            tdm_in.stickL = 1;
            tdm_in.stickLX = x;
            tdm_in.stickLY = y;
        } else {
            s_role[s] = 2;
            tdm_in.stickR = 1;
            tdm_in.stickRX = x;
            tdm_in.stickRY = y;
            tdm_in.firing = 1;
        }
        return 1;
    }
    if (action == AMOTION_EVENT_ACTION_MOVE) {
        int n = AMotionEvent_getPointerCount(ev);
        for (i = 0; i < n; i++) {
            int pid = AMotionEvent_getPointerId(ev, i);
            s = tdm_slot_of(pid);
            if (s < 0 || s_role[s] == 3) continue;
            tdm_stick_move(AMotionEvent_getX(ev, i), AMotionEvent_getY(ev, i),
                           s_role[s],
                           s_role[s] == 1 ? tdm_in.stickLX : tdm_in.stickRX,
                           s_role[s] == 1 ? tdm_in.stickLY : tdm_in.stickRY,
                           s_role[s] == 1 ? &tdm_in.stickLDX : &tdm_in.stickRDX,
                           s_role[s] == 1 ? &tdm_in.stickLDY : &tdm_in.stickRDY);
        }
        return 1;
    }
    if (action == AMOTION_EVENT_ACTION_UP ||
        action == AMOTION_EVENT_ACTION_POINTER_UP ||
        action == AMOTION_EVENT_ACTION_CANCEL) {
        s = tdm_slot_of(id);
        if (s < 0) return 1;
        if (s_role[s] == 3) tdm_ptr_release(s_bid[s]);
        else tdm_stick_end(s_role[s]);
        s_role[s] = 0;
    }
    return 1;
}
