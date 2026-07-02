# Commands

Common commands for the Lichee Pi Zero LCD framebuffer UI project.

## Go to project

    cd /root/lcd-ui

## Build

    make

Clean and rebuild:

    make clean
    make

## Install

    ./scripts/install.sh

## Check project

    ./scripts/check.sh

## LCD control

Set UI state:

    lcdctl ready
    lcdctl start
    lcdctl stop
    lcdctl info

Set progress:

    lcdctl progress 55

Show status:

    lcdctl status

Run demo:

    lcdctl demo

Repaint progress:

    lcdctl repaint

Reset UI:

    lcdctl reset

## Service

Check service:

    systemctl status lcd-app.service --no-pager

Restart service:

    systemctl restart lcd-app.service

Stop service:

    systemctl stop lcd-app.service

Start service:

    systemctl start lcd-app.service

Enable autostart:

    systemctl enable lcd-app.service

## Runtime files

State file:

    /var/lib/lcd-ui/state

Progress file:

    /var/lib/lcd-ui/progress

Command trigger file:

    /tmp/lcd_cmd

## Framebuffer

Check framebuffer:

    ls -l /dev/fb0
    cat /proc/fb

## Input

Check input devices:

    ls -l /dev/input
    cat /proc/bus/input/devices

Current touch note:

    /dev/input/event0 exists as ns2009_ts, but it is noisy and not used yet.

## Debug app manually

Stop service first:

    systemctl stop lcd-app.service

Run app manually:

    cd /root/lcd-ui
    ./lcd_app

Start service again:

    systemctl start lcd-app.service
