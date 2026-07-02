#!/bin/sh

echo "=== LCD UI Project Check ==="

echo
echo "[1] Files"
for f in VERSION README.md main.c gfx.c gfx.h ui.c ui.h fb_progress.c Makefile scripts/lcdctl scripts/run_lcd_app.sh scripts/install.sh scripts/check.sh systemd/lcd-app.service docs/status.md docs/commands.md docs/troubleshooting.md docs/storage-resize.md docs/backup-restore.md docs/snapshot-v1.md; do
    if [ -f "$f" ]; then
        echo "OK      $f"
    else
        echo "MISSING $f"
    fi
done

echo
echo "[2] Normalize timestamps"
find . -exec touch {} \;
echo "OK      timestamps"

echo
echo "[3] Build"
make

if [ -f lcd_app ]; then
    echo "OK      lcd_app"
else
    echo "MISSING lcd_app"
fi

if [ -f fb_progress ]; then
    echo "OK      fb_progress"
else
    echo "MISSING fb_progress"
fi

echo
echo "[4] Device"
if [ -e /dev/fb0 ]; then
    echo "OK      /dev/fb0"
else
    echo "MISSING /dev/fb0"
fi

echo
echo "[5] Service"
systemctl is-active lcd-app.service

echo
echo "[6] Runtime status"
lcdctl status

echo
echo "=== Check Done ==="
