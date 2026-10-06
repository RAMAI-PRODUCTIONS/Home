#include "tdm_android.h"

#include "tdm_hud.h"
#include "tdm_input.h"

InputState tdm_in;

int s_pid[TDM_MAX_PTR], s_role[TDM_MAX_PTR], s_bid[TDM_MAX_PTR];
float s_lookRX, s_lookRY;

void tdm_input_reset(void)
{
    int i;
    tdm_input_init(&tdm_in);
    for (i = 0; i < TDM_MAX_PTR; i++) s_role[i] = 0;
    s_lookRX = s_lookRY = 0.0f;
}

int tdm_slot_free(void)
{
    int i;
    for (i = 0; i < TDM_MAX_PTR; i++)
        if (!s_role[i]) return i;
    return -1;
}

int tdm_slot_of(int id)
{
    int i;
    for (i = 0; i < TDM_MAX_PTR; i++)
        if (s_role[i] && s_pid[i] == id) return i;
    return -1;
}

void tdm_ptr_press(int id)
{
    tdm_in.btnMask |= 1u << id;
    if (id == TDM_BTN_FIRE) tdm_in.firing = 1;
    else if (id == TDM_BTN_JUMP) tdm_in.jump = 1;
    else if (id == TDM_BTN_RELOAD) tdm_in.reload = 1;
    else if (id == TDM_BTN_WPN) tdm_in.cycle = 1;
    else if (id == TDM_BTN_GREN) tdm_in.grenade = 1;
    else if (id == TDM_BTN_USE) tdm_in.interact = 1;
}

void tdm_ptr_release(int id)
{
    tdm_in.btnMask &= ~(1u << id);
    if (id == TDM_BTN_FIRE && !(tdm_in.btnMask & (1u << TDM_BTN_FIRE)))
        tdm_in.firing = 0;
}
