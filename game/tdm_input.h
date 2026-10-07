#pragma once

#include <stdint.h>

/* Filled by the Android event pump, consumed once per tick. Frame-local
   fields are cleared by tdm_input_clear_frame() after every tick. */

typedef struct InputState {
    float moveX, moveY;     /* left stick, -1..1 (y already forward+) */
    float lookX, lookY;     /* accumulated look delta in radians */
                            /* consumers apply: yaw -= lookX, pitch -= lookY */
    int firing, jump, sprint, reload, ads;
    int weaponSwitch;       /* -1 = none, else 0..4 */
    int cycle;              /* next weapon */
    int grenade;
    int interact;
    int tap;                /* 1 = a screen tap is waiting */
    float tapX, tapY;

    /* stick origins (px) and knob offsets, mirrored into the HUD */
    float stickLX, stickLY, stickLDX, stickLDY;
    float stickRX, stickRY, stickRDX, stickRDY;
    int stickL, stickR;
    uint32_t btnMask;       /* 1 << button id while held */
} InputState;

void tdm_input_init(InputState *in);
void tdm_input_clear_frame(InputState *in);
