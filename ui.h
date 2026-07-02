#ifndef UI_H
#define UI_H

#include "gfx.h"

typedef struct {
    int x;
    int y;
    int w;
    int h;
    const char *label;
    unsigned int bg;
    unsigned int fg;
    unsigned int border;
} Button;

void ui_button_draw(Gfx *g, Button *b);

#endif
