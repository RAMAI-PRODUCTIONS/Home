#include "tdm_input.h"

#include <string.h>

void tdm_input_init(InputState *in)
{
    if (!in) return;
    memset(in, 0, sizeof *in);
    in->weaponSwitch = -1;
}

void tdm_input_clear_frame(InputState *in)
{
    if (!in) return;
    in->lookX = 0.0f;
    in->lookY = 0.0f;
    in->jump = 0;
    in->reload = 0;
    in->grenade = 0;
    in->interact = 0;
    in->cycle = 0;
    in->weaponSwitch = -1;
    in->tap = 0;
}
