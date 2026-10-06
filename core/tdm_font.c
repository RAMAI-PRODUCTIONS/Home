#include "tdm_font.h"

#include <string.h>

extern const uint8_t tdm_font_lo[48][7];
extern const uint8_t tdm_font_hi[48][7];

static const uint8_t s_blank[7];

const uint8_t *tdm_font_glyph(int ch)
{
    int i = ch - 32;
    if (i < 0 || i >= 96) return s_blank;
    return i < 48 ? tdm_font_lo[i] : tdm_font_hi[i - 48];
}

void tdm_font_build(uint8_t *px)
{
    int ch;
    memset(px, 0, TDM_FONT_W * TDM_FONT_H);
    for (ch = 32; ch < 128; ch++) {
        const uint8_t *g = tdm_font_glyph(ch);
        int i = ch - 32, cx = (i % 16) * 8, cy = (i / 16) * 8, y, x;
        for (y = 0; y < 7; y++)
            for (x = 0; x < 5; x++)
                if (g[y] & (1 << (4 - x)))
                    px[(cy + y) * TDM_FONT_W + cx + x] = 255;
    }
    for (ch = 0; ch < 64; ch++) {
        int wy = (TDM_FONT_WHITE - 32) / 16 * 8 + ch / 8;
        int wx = (TDM_FONT_WHITE - 32) % 16 * 8 + ch % 8;
        px[wy * TDM_FONT_W + wx] = 255;
    }
}
