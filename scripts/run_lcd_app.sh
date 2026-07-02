#!/bin/sh

dmesg -n 1
echo 0 > /proc/sys/kernel/printk 2>/dev/null
echo 0 > /sys/class/graphics/fbcon/cursor_blink 2>/dev/null
setterm -cursor off > /dev/tty0 2>/dev/null

for c in /sys/class/vtconsole/vtcon*; do
    if grep -qi "frame buffer" "$c/name" 2>/dev/null; then
        echo 0 > "$c/bind" 2>/dev/null
    fi
done

## V li progress sau khi app chh 
 render UI
(
    sleep 1
    if [ -f /var/lib/lcd-ui/progress ] && [ -x /usr/local/bin/fb_progress ]; then
        /usr/local/bin/fb_progress "$(cat /var/lib/lcd-ui/progress)" 2>/dev/null
    fi
) &

cd /root/lcd-ui || exit 1
exec ./lcd_app
