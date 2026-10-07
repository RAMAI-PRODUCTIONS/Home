#include "tdm_android.h"

#include <math.h>

#include "tdm_hud.h"
#include "tdm_input.h"

void tdm_stick_move(float px, float py, int role, float ox, float oy,
                    float *kx, float *ky)
{
    float dx = px - ox, dy = py - oy;
    float len = sqrtf(dx * dx + dy * dy);
    if (len > TDM_STICK_R) {
        dx *= TDM_STICK_R / len;
        dy *= TDM_STICK_R / len;
    }
    *kx = dx;
    *ky = dy;
    if (role == 1) {
        /* screen y grows downward; invert so up on screen is forward (+) */
        tdm_in.moveX = dx / TDM_STICK_R;
        tdm_in.moveY = dy / TDM_STICK_R;
        tdm_in.sprint = fabsf(tdm_in.moveX) > 0.92f &&
                        fabsf(tdm_in.moveY) > 0.92f;
    } else {
        /* accumulated look delta is applied as lookX -= in->lookX and
           pitch -= in->lookY, so a downward screen drag must decrease
           pitch (looking down) -> positive dy must produce positive delta. */
        s_lookRX = dx / TDM_STICK_R;
        s_lookRY = dy / TDM_STICK_R;
    }
}

void tdm_stick_end(int role)
{
    if (role == 1) {
        tdm_in.stickL = 0;
        tdm_in.stickLDX = tdm_in.stickLDY = 0.0f;
        tdm_in.moveX = tdm_in.moveY = 0.0f;
        tdm_in.sprint = 0;
    } else {
        tdm_in.stickR = 0;
        tdm_in.stickRDX = tdm_in.stickRDY = 0.0f;
        s_lookRX = s_lookRY = 0.0f;
        if (!(tdm_in.btnMask & (1u << TDM_BTN_FIRE))) tdm_in.firing = 0;
    }
}

void tdm_input_look_step(float dt)
{
    tdm_in.lookX += s_lookRX * dt * 3.2f;
    tdm_in.lookY += s_lookRY * dt * 2.6f;
}
