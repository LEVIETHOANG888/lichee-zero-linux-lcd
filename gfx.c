#include "gfx.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

unsigned int gfx_rgb(unsigned char r, unsigned char g, unsigned char b) {
    return ((unsigned int)r << 16) | ((unsigned int)g << 8) | b;
}

int gfx_open(Gfx *g, const char *fbdev) {
    struct fb_var_screeninfo v;
    struct fb_fix_screeninfo f;

    g->fd = open(fbdev, O_RDWR);
    if (g->fd < 0) {
        perror("open fb");
        return -1;
    }

    if (ioctl(g->fd, FBIOGET_VSCREENINFO, &v) < 0) {
        perror("FBIOGET_VSCREENINFO");
        return -1;
    }

    if (ioctl(g->fd, FBIOGET_FSCREENINFO, &f) < 0) {
        perror("FBIOGET_FSCREENINFO");
        return -1;
    }

    g->width = v.xres;
    g->height = v.yres;
    g->bpp = v.bits_per_pixel;
    g->line_length = f.line_length;
    g->size = f.smem_len;

    g->fb = mmap(0, g->size, PROT_READ | PROT_WRITE, MAP_SHARED, g->fd, 0);
    if (g->fb == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    printf("fb %dx%d bpp=%d line=%d size=%d\n",
           g->width, g->height, g->bpp, g->line_length, g->size);

    return 0;
}

void gfx_close(Gfx *g) {
    munmap(g->fb, g->size);
    close(g->fd);
}

void gfx_pixel(Gfx *g, int x, int y, unsigned int c) {
    if (x < 0 || y < 0 || x >= g->width || y >= g->height) return;

    unsigned char *p = g->fb + y * g->line_length + x * 4;

    p[0] = c & 0xff;
    p[1] = (c >> 8) & 0xff;
    p[2] = (c >> 16) & 0xff;
    p[3] = 0;
}

void gfx_rect(Gfx *g, int x, int y, int w, int h, unsigned int c) {
    int xx, yy;

    for (yy = y; yy < y + h; yy++) {
        for (xx = x; xx < x + w; xx++) {
            gfx_pixel(g, xx, yy, c);
        }
    }
}

void gfx_clear(Gfx *g, unsigned int c) {
    gfx_rect(g, 0, 0, g->width, g->height, c);
}

void gfx_border(Gfx *g, int x, int y, int w, int h, int t, unsigned int c) {
    gfx_rect(g, x, y, w, t, c);
    gfx_rect(g, x, y + h - t, w, t, c);
    gfx_rect(g, x, y, t, h, c);
    gfx_rect(g, x + w - t, y, t, h, c);
}

static const unsigned char *glyph(char ch) {
    static unsigned char g[5];

    if (ch >= 'a' && ch <= 'z') ch -= 32;

    switch (ch) {
        case 'A': { unsigned char t[5]={0x7e,0x11,0x11,0x11,0x7e}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'B': { unsigned char t[5]={0x7f,0x49,0x49,0x49,0x36}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'C': { unsigned char t[5]={0x3e,0x41,0x41,0x41,0x22}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'D': { unsigned char t[5]={0x7f,0x41,0x41,0x22,0x1c}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'E': { unsigned char t[5]={0x7f,0x49,0x49,0x49,0x41}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'F': { unsigned char t[5]={0x7f,0x09,0x09,0x09,0x01}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'G': { unsigned char t[5]={0x3e,0x41,0x49,0x49,0x7a}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'H': { unsigned char t[5]={0x7f,0x08,0x08,0x08,0x7f}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'I': { unsigned char t[5]={0x00,0x41,0x7f,0x41,0x00}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'K': { unsigned char t[5]={0x7f,0x08,0x14,0x22,0x41}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'L': { unsigned char t[5]={0x7f,0x40,0x40,0x40,0x40}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'M': { unsigned char t[5]={0x7f,0x02,0x0c,0x02,0x7f}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'N': { unsigned char t[5]={0x7f,0x04,0x08,0x10,0x7f}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'O': { unsigned char t[5]={0x3e,0x41,0x41,0x41,0x3e}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'P': { unsigned char t[5]={0x7f,0x09,0x09,0x09,0x06}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'R': { unsigned char t[5]={0x7f,0x09,0x19,0x29,0x46}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'S': { unsigned char t[5]={0x46,0x49,0x49,0x49,0x31}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'T': { unsigned char t[5]={0x01,0x01,0x7f,0x01,0x01}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'U': { unsigned char t[5]={0x3f,0x40,0x40,0x40,0x3f}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'V': { unsigned char t[5]={0x1f,0x20,0x40,0x20,0x1f}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'X': { unsigned char t[5]={0x63,0x14,0x08,0x14,0x63}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case 'Y': { unsigned char t[5]={0x07,0x08,0x70,0x08,0x07}; for(int i=0;i<5;i++)g[i]=t[i]; break; }

        case '0': { unsigned char t[5]={0x3e,0x51,0x49,0x45,0x3e}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '1': { unsigned char t[5]={0x00,0x42,0x7f,0x40,0x00}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '2': { unsigned char t[5]={0x42,0x61,0x51,0x49,0x46}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '3': { unsigned char t[5]={0x21,0x41,0x45,0x4b,0x31}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '4': { unsigned char t[5]={0x18,0x14,0x12,0x7f,0x10}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '5': { unsigned char t[5]={0x27,0x45,0x45,0x45,0x39}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '6': { unsigned char t[5]={0x3c,0x4a,0x49,0x49,0x30}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '7': { unsigned char t[5]={0x01,0x71,0x09,0x05,0x03}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '8': { unsigned char t[5]={0x36,0x49,0x49,0x49,0x36}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '9': { unsigned char t[5]={0x06,0x49,0x49,0x29,0x1e}; for(int i=0;i<5;i++)g[i]=t[i]; break; }

        case ' ': { unsigned char t[5]={0x00,0x00,0x00,0x00,0x00}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        case '-': { unsigned char t[5]={0x08,0x08,0x08,0x08,0x08}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
        default:  { unsigned char t[5]={0x00,0x00,0x00,0x00,0x00}; for(int i=0;i<5;i++)g[i]=t[i]; break; }
    }

    return g;
}

static void draw_char(Gfx *g, int x, int y, char ch, int scale, unsigned int color) {
    const unsigned char *data = glyph(ch);

    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 7; row++) {
            if (data[col] & (1 << row)) {
                gfx_rect(g, x + col * scale, y + row * scale, scale, scale, color);
            }
        }
    }
}

void gfx_text(Gfx *g, int x, int y, const char *s, int scale, unsigned int color) {
    while (*s) {
        draw_char(g, x, y, *s, scale, color);
        x += 6 * scale;
        s++;
    }
}
