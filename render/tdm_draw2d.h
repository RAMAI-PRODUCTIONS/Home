#pragma once

#include <stdint.h>

#include "tdm_types.h"

/* Immediate-mode UI prims. Emits into the dynamic vertex buffer; vertices
   after the 3D batch are drawn with the HUD pipeline and the font atlas. */

typedef struct DrawCtx {
    Vtx *v;
    int *n;
    int max;
    int w, h;
} DrawCtx;

static inline uint32_t tdm_rgb(float r, float g, float b)
{
    return 0xFF000000u | ((uint32_t)(r * 255.0f) << 16) |
           ((uint32_t)(g * 255.0f) << 8) | (uint32_t)(b * 255.0f);
}

void tdm2_put(DrawCtx *c, float x, float y, float u, float v, uint32_t argb);
void tdm2_quad(DrawCtx *c, float x, float y, float w, float h,
               const float uv[4], uint32_t argb);
void tdm2_rect(DrawCtx *c, float x, float y, float w, float h, uint32_t argb);
void tdm2_line(DrawCtx *c, float x0, float y0, float x1, float y1,
               float thick, uint32_t argb);
void tdm2_disc(DrawCtx *c, float cx, float cy, float r, uint32_t argb);
void tdm2_text(DrawCtx *c, float x, float y, float scale, uint32_t argb,
               const char *s);
void tdm2_text_c(DrawCtx *c, float cx, float y, float scale, uint32_t argb,
                 const char *s);
float tdm2_text_w(const char *s, float scale);
