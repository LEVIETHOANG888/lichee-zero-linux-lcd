# Snapshot V1 - Framebuffer UI Stable

## Date

Manual project snapshot before LVGL migration.

## Status

This version is the first stable custom framebuffer UI.

## Confirmed working

- Build with Makefile
- lcd_app builds successfully
- fb_progress builds successfully
- systemd service starts on boot
- lcdctl controls UI state
- lcdctl progress works
- State persists after reboot
- Progress persists after reboot
- check.sh passes
- install.sh works
- README and docs are prepared

## Runtime status

Expected:

    service: active
    fb0: OK
    app process: running

## Commands tested

    make clean
    make
    make restart
    make status

    lcdctl reset
    lcdctl start
    lcdctl progress 75
    lcdctl info
    lcdctl status

## Deferred

- Touch input
- SSH/RNDIS network
- LVGL version
