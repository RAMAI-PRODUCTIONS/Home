#include "tdm_draw2d.h"

#include <math.h>
#include <string.h>

#include "tdm_font.h"

void tdm2_put(DrawCtx *c, float x, float y, float u, float v, uint32_t argb)
{
    Vtx *t;
    if (*c->n >= c->max) return;
    t = &c->v[(*c->n)++];
    t->px = x;
    t->py = y;
    t->pz = 0.0f;
    t->u = u;
    t->v = v;
    t->r = (float)((argb >> 16) & 255) / 255.0f;
    t->g = (float)((argb >> 8) & 255) / 255.0f;
    t->b = (float)(argb & 255) / 255.0f;
    t->a = (float)(argb >> 24) / 255.0f;
}

void tdm2_quad(DrawCtx *c, float x, float y, float w, float h,
               const float uv[4], uint32_t argb)
{
    tdm2_put(c, x, y, uv[0], uv[1], argb);
    tdm2_put(c, x + w, y, uv[2], uv[1], argb);
    tdm2_put(c, x + w, y + h, uv[2], uv[3], argb);
    tdm2_put(c, x, y, uv[0], uv[1], argb);
    tdm2_put(c, x + w, y + h, uv[2], uv[3], argb);
    tdm2_put(c, x, y + h, uv[0], uv[3], argb);
}

void tdm2_rect(DrawCtx *c, float x, float y, float w, float h, uint32_t argb)
{
    float uv[4];
    tdm_font_uv(TDM_FONT_WHITE, uv);
    tdm2_quad(c, x, y, w, h, uv, argb);
}

float tdm2_text_w(const char *s, float scale)
{
    return (float)strlen(s) * 6.0f * scale;
}

void tdm2_text(DrawCtx *c, float x, float y, float scale, uint32_t argb,
               const char *s)
{
    float uv[4];
    for (; *s; s++) {
        if (*s != ' ') {
            tdm_font_uv((unsigned char)*s, uv);
            tdm2_quad(c, x, y, 5.0f * scale, 7.0f * scale, uv, argb);
        }
        x += 6.0f * scale;
    }
}

void tdm2_text_c(DrawCtx *c, float cx, float y, float scale, uint32_t argb,
                 const char *s)
{
    tdm2_text(c, cx - tdm2_text_w(s, scale) * 0.5f, y, scale, argb, s);
}
