#include "ui.h"

void ui_button_draw(Gfx *g, Button *b) {
    int text_scale = 3;
    int char_w = 6 * text_scale;
    int text_w = 0;
    const char *s = b->label;

    while (*s) {
        text_w += char_w;
        s++;
    }

    int text_h = 7 * text_scale;
    int tx = b->x + (b->w - text_w) / 2;
    int ty = b->y + (b->h - text_h) / 2;

    gfx_rect(g, b->x, b->y, b->w, b->h, b->bg);
    gfx_border(g, b->x, b->y, b->w, b->h, 4, b->border);
    gfx_text(g, tx, ty, b->label, text_scale, b->fg);
}
