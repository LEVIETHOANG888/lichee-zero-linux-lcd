# Lichee Pi Zero LCD Framebuffer UI

Custom Linux framebuffer UI for Lichee Pi Zero / Allwinner V3s.

The app draws directly to /dev/fb0 without X11, LXDE, Qt, or GTK.

## Hardware

- Lichee Pi Zero / Sipeed Lichee Zero
- Allwinner V3s
- 800x480 RGB LCD
- USB TTL serial adapter
- microSD card

## Current Status

Working:

- Debian LXDE 800x480 image boots successfully
- UART serial works with PuTTY at 115200
- LCD framebuffer /dev/fb0 works
- Resolution: 800x480
- Framebuffer format: 32bpp
- Custom C framebuffer UI works
- systemd autostart works
- fbcon cursor/log overlay disabled
- lcdctl controls UI state
- Progress bar works
- State and progress persist after reboot
- install.sh works
- check.sh works

Deferred:

- SSH/RNDIS network
- Touch input calibration
- LVGL migration

## Commands

UI state:

    lcdctl ready
    lcdctl start
    lcdctl stop
    lcdctl info

Progress:

    lcdctl progress 55

Utility:

    lcdctl status
    lcdctl demo
    lcdctl repaint
    lcdctl reset

## Build

    make

Build outputs:

    lcd_app
    fb_progress

## Install

    ./scripts/install.sh

## Check

    ./scripts/check.sh

## Service

    systemctl status lcd-app.service
    systemctl restart lcd-app.service

## Project Structure

    Makefile
    README.md
    main.c
    gfx.c
    gfx.h
    ui.c
    ui.h
    fb_progress.c
    scripts/lcdctl
    scripts/run_lcd_app.sh
    scripts/install.sh
    scripts/check.sh
    systemd/lcd-app.service
    docs/status.md

## Runtime Files

    /var/lib/lcd-ui/state
    /var/lib/lcd-ui/progress
    /tmp/lcd_cmd

## Next Plan

1. Finish stable framebuffer UI
2. Clean project structure
3. Push to GitHub
4. Start LVGL version
