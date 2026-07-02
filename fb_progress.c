#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static unsigned char *fbp;
static long screensize;

static uint32_t color_rgb(int r, int g, int b) {
    uint32_t c = 0;
    c |= ((r >> (8 - vinfo.red.length)) << vinfo.red.offset);
    c |= ((g >> (8 - vinfo.green.length)) << vinfo.green.offset);
    c |= ((b >> (8 - vinfo.blue.length)) << vinfo.blue.offset);
    return c;
}

static void fill_rect(int x, int y, int w, int h, uint32_t color) {
    int xx, yy;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + w > (int)vinfo.xres) w = vinfo.xres - x;
    if (y + h > (int)vinfo.yres) h = vinfo.yres - y;

    for (yy = y; yy < y + h; yy++) {
        for (xx = x; xx < x + w; xx++) {
            long location = xx * (vinfo.bits_per_pixel / 8) + yy * finfo.line_length;
            *((uint32_t *)(fbp + location)) = color;
        }
    }
}

int main(int argc, char **argv) {
    int fbfd;
    int progress;
    int x, y, w, h, inner_w;

    if (argc < 2) {
        fprintf(stderr, "Usage: fb_progress 0-100\n");
        return 1;
    }

    progress = atoi(argv[1]);
    if (progress < 0) progress = 0;
    if (progress > 100) progress = 100;

    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd < 0) {
        perror("open /dev/fb0");
        return 1;
    }

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) < 0) {
        perror("FBIOGET_FSCREENINFO");
        close(fbfd);
        return 1;
    }

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("FBIOGET_VSCREENINFO");
        close(fbfd);
        return 1;
    }

    screensize = finfo.smem_len;
    fbp = (unsigned char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((long)fbp == -1) {
        perror("mmap");
        close(fbfd);
        return 1;
    }

    w = vinfo.xres * 3 / 4;
    h = 28;
    x = (vinfo.xres - w) / 2;
    y = vinfo.yres - 70;

    fill_rect(x - 4, y - 4, w + 8, h + 8, color_rgb(220, 220, 220));
    fill_rect(x, y, w, h, color_rgb(40, 40, 40));

    inner_w = (w * progress) / 100;
    fill_rect(x, y, inner_w, h, color_rgb(0, 180, 90));

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
