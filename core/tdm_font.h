#pragma once

#include <stdint.h>

/* 5x7 bitmap font baked into a 128x64 R8 atlas at startup. Cell 127 is a
   solid block used as the white texel for flat-coloured UI rectangles. */

#define TDM_FONT_W 128
#define TDM_FONT_H 64
#define TDM_FONT_WHITE 127

const uint8_t *tdm_font_glyph(int ch);
void tdm_font_build(uint8_t *pixels);

/* uv = { u0, v0, u1, v1 } inset by a quarter texel for nearest sampling. */
static inline void tdm_font_uv(int ch, float uv[4])
{
    int i = ch - 32;
    float cx, cy;
    if (i < 0 || i >= 96) i = 0;
    cx = (float)((i % 16) * 8);
    cy = (float)((i / 16) * 8);
    uv[0] = (cx + 0.25f) / (float)TDM_FONT_W;
    uv[1] = (cy + 0.25f) / (float)TDM_FONT_H;
    uv[2] = (cx + 4.75f) / (float)TDM_FONT_W;
    uv[3] = (cy + 6.75f) / (float)TDM_FONT_H;
}
