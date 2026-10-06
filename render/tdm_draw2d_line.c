#include "tdm_draw2d.h"

#include <math.h>

#include "tdm_font.h"

void tdm2_line(DrawCtx *c, float x0, float y0, float x1, float y1,
               float thick, uint32_t argb)
{
    float dx = x1 - x0, dy = y1 - y0;
    float len = sqrtf(dx * dx + dy * dy);
    float nx, ny;
    if (len < 0.001f) return;
    nx = -dy / len * thick * 0.5f;
    ny = dx / len * thick * 0.5f;
    {
        float uv[4];
        tdm_font_uv(TDM_FONT_WHITE, uv);
        tdm2_put(c, x0 + nx, y0 + ny, uv[0], uv[1], argb);
        tdm2_put(c, x1 + nx, y1 + ny, uv[2], uv[1], argb);
        tdm2_put(c, x1 - nx, y1 - ny, uv[2], uv[3], argb);
        tdm2_put(c, x0 + nx, y0 + ny, uv[0], uv[1], argb);
        tdm2_put(c, x1 - nx, y1 - ny, uv[2], uv[3], argb);
        tdm2_put(c, x0 - nx, y0 - ny, uv[0], uv[3], argb);
    }
}

void tdm2_disc(DrawCtx *c, float cx, float cy, float r, uint32_t argb)
{
    float uv[4];
    int i;
    tdm_font_uv(TDM_FONT_WHITE, uv);
    for (i = 0; i < 16; i++) {
        float a0 = (float)i * 0.39269908f;
        float a1 = a0 + 0.39269908f;
        tdm2_put(c, cx, cy, uv[0], uv[1], argb);
        tdm2_put(c, cx + cosf(a0) * r, cy + sinf(a0) * r, uv[2], uv[3], argb);
        tdm2_put(c, cx + cosf(a1) * r, cy + sinf(a1) * r, uv[2], uv[3], argb);
    }
}
