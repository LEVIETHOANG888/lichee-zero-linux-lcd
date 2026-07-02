#ifndef GFX_H
#define GFX_H

typedef struct {
    int fd;
    unsigned char *fb;
    int width;
    int height;
    int bpp;
    int line_length;
    int size;
} Gfx;

unsigned int gfx_rgb(unsigned char r, unsigned char g, unsigned char b);

int  gfx_open(Gfx *g, const char *fbdev);
void gfx_close(Gfx *g);

void gfx_pixel(Gfx *g, int x, int y, unsigned int color);
void gfx_rect(Gfx *g, int x, int y, int w, int h, unsigned int color);
void gfx_clear(Gfx *g, unsigned int color);
void gfx_border(Gfx *g, int x, int y, int w, int h, int t, unsigned int color);
void gfx_text(Gfx *g, int x, int y, const char *s, int scale, unsigned int color);

#endif
