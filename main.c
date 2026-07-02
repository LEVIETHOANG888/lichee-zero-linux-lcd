#include "gfx.h"
#include "ui.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

static struct termios old_term;

static void input_init(void) {
    struct termios t;

    tcgetattr(0, &old_term);
    t = old_term;
    t.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(0, TCSANOW, &t);
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}

static void input_restore(void) {
    tcsetattr(0, TCSANOW, &old_term);
}



static int parse_state_name(const char *buf) {
    if (strncmp(buf, "start", 5) == 0) return 1;
    if (strncmp(buf, "stop", 4) == 0) return 2;
    if (strncmp(buf, "info", 4) == 0) return 3;
    if (strncmp(buf, "ready", 5) == 0) return 0;
    return -1;
}

static int read_state_file(void) {
    FILE *fp;
    char buf[64];

    fp = fopen("/var/lib/lcd-ui/state", "r");
    if (!fp) return 0;

    if (!fgets(buf, sizeof(buf), fp)) {
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return parse_state_name(buf);
}

static int read_cmd_file(void) {
    FILE *fp;
    char buf[64];

    fp = fopen("/tmp/lcd_cmd", "r");
    if (!fp) return -1;

    if (!fgets(buf, sizeof(buf), fp)) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    unlink("/tmp/lcd_cmd");

    if (strncmp(buf, "start", 5) == 0) return 1;
    if (strncmp(buf, "stop", 4) == 0) return 2;
    if (strncmp(buf, "info", 4) == 0) return 3;
    if (strncmp(buf, "ready", 5) == 0) return 0;

    return -1;
}

static int read_key(void) {
    unsigned char c;
    int n = read(0, &c, 1);
    if (n == 1) return c;
    return -1;
}

static void draw_screen(Gfx *g, int state) {
    unsigned int bg     = gfx_rgb(18, 22, 28);
    unsigned int header = gfx_rgb(30, 150, 210);
    unsigned int panel  = gfx_rgb(35, 45, 58);
    unsigned int white  = gfx_rgb(245, 245, 245);
    unsigned int green  = gfx_rgb(40, 200, 150);
    unsigned int red    = gfx_rgb(230, 100, 110);
    unsigned int yellow = gfx_rgb(240, 210, 70);
    unsigned int dark   = gfx_rgb(10, 14, 20);
    unsigned int blue   = gfx_rgb(60, 120, 230);

    Button btn_start = { 90,  295, 180, 80, "START", green,  white, white };
    Button btn_stop  = { 310, 295, 180, 80, "STOP",  red,    white, white };
    Button btn_info  = { 530, 295, 180, 80, "INFO",  yellow, bg,    white };

    if (state == 1) btn_start.border = blue;
    if (state == 2) btn_stop.border  = blue;
    if (state == 3) btn_info.border  = blue;

    gfx_clear(g, bg);

    gfx_rect(g, 0, 0, g->width, 70, header);
    gfx_text(g, 30, 22, "LICHEE UI DEMO", 3, white);

    gfx_rect(g, 40, 110, g->width - 80, g->height - 160, panel);
    gfx_border(g, 40, 110, g->width - 80, g->height - 160, 4, white);

    gfx_text(g, 70, 140, "FRAMEBUFFER OK", 3, green);
    gfx_text(g, 70, 185, "KEY 1 2 3 TO CONTROL", 2, white);

    gfx_rect(g, 70, 230, 660, 36, dark);

    if (state == 1) {
        gfx_text(g, 85, 238, "STATUS RUNNING", 2, green);
    } else if (state == 2) {
        gfx_text(g, 85, 238, "STATUS STOPPED", 2, red);
    } else if (state == 3) {
        gfx_text(g, 85, 238, "STATUS INFO PAGE", 2, yellow);
    } else {
        gfx_text(g, 85, 238, "STATUS READY", 2, white);
    }

    ui_button_draw(g, &btn_start);
    ui_button_draw(g, &btn_stop);
    ui_button_draw(g, &btn_info);
}

int main(void) {
    Gfx g;
    int state = read_state_file();
    if (state < 0) state = 0;
    int key;

    if (gfx_open(&g, "/dev/fb0") != 0) {
        return 1;
    }

    input_init();
    draw_screen(&g, state);

    while (1) {
        int cmd = read_cmd_file();
        if (cmd >= 0) {
            state = cmd;
            draw_screen(&g, state);
        }

        key = read_key();

        if (key == '1') {
            state = 1;
            draw_screen(&g, state);
        } else if (key == '2') {
            state = 2;
            draw_screen(&g, state);
        } else if (key == '3') {
            state = 3;
            draw_screen(&g, state);
        } else if (key == 'q' || key == 'Q') {
            break;
        }

        usleep(30000);
    }

    input_restore();
    gfx_close(&g);
    return 0;
}
