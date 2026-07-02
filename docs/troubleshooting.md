# Troubleshooting

Common issues found during the Lichee Pi Zero LCD framebuffer UI project.

## 1. Makefile missing separator

Error:

    Makefile:7: *** missing separator. Stop.

Cause:

    Makefile command lines require TAB.
    Spaces will cause this error.

Fix used:

    Use .RECIPEPREFIX := > in Makefile.

Example:

    .RECIPEPREFIX := >

    all:
    >gcc -o lcd_app main.c gfx.c ui.c

## 2. Clock skew detected

Error:

    make: Warning: File 'Makefile' has modification time in the future
    make: warning: Clock skew detected

Cause:

    Lichee board time is wrong, often reset to 1970.

Fix:

    cd /root/lcd-ui
    find . -exec touch {} \;
    make clean
    make

## 3. fbcon cursor or logs appear on LCD

Symptoms:

    Blinking dot or console messages appear over framebuffer UI.

Fix used in run_lcd_app.sh:

    dmesg -n 1
    echo 0 > /proc/sys/kernel/printk 2>/dev/null
    echo 0 > /sys/class/graphics/fbcon/cursor_blink 2>/dev/null
    setterm -cursor off > /dev/tty0 2>/dev/null

    for c in /sys/class/vtconsole/vtcon*; do
        if grep -qi "frame buffer" "$c/name" 2>/dev/null; then
            echo 0 > "$c/bind" 2>/dev/null
        fi
    done

## 4. service active but lcdctl status says app not running

Symptom:

    systemctl status lcd-app.service shows active.
    But lcdctl status shows app process: not running.

Cause:

    Process check command in lcdctl was not reliable.

Fix:

    Use pidof lcd_app or ps grep check.

Expected status:

    service: active
    app process: running

## 5. Touch input noisy

Device:

    /dev/input/event0
    ns2009_ts

Symptom:

    Touch emits continuous/stuck coordinates even without valid touch.

Decision:

    Touch is deferred.
    Current UI is controlled by lcdctl command instead.

## 6. Command typo lcdct1

Wrong:

    lcdct1 info

Correct:

    lcdctl info

Note:

    The last character must be letter l, not number 1.

## 7. Check project

Run:

    cd /root/lcd-ui
    ./scripts/check.sh

Expected:

    No MISSING lines
    service active
    fb0 OK
    app process running
