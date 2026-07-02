# Project Status

## Working

- Lichee Pi Zero boots Debian LXDE 800x480 image
- UART serial works with PuTTY COM3 115200
- LCD framebuffer `/dev/fb0` works
- Resolution: 800x480
- Framebuffer: 32bpp
- Custom C framebuffer UI works
- systemd autostart works
- fbcon cursor/log overlay disabled
- `lcdctl` controls UI state
- `lcdctl progress 0-100` works
- State persists after reboot
- Progress persists after reboot

## Commands

```sh
lcdctl ready
lcdctl start
lcdctl stop
lcdctl info
lcdctl progress 70
lcdctl status
lcdctl demo
lcdctl repaint
lcdctl reset
cd /root/lcd-ui

cat > scripts/install.sh <<'EOF'
#!/bin/sh
set -e

APP_DIR="/root/lcd-ui"

echo "[1/6] Check project dir"
cd "$APP_DIR"

echo "[2/6] Build lcd_app and fb_progress"
make

if [ -f fb_progress.c ]; then
    gcc -std=gnu99 -O2 -Wall -o fb_progress fb_progress.c
fi

echo "[3/6] Install lcdctl"
cp scripts/lcdctl /usr/local/bin/lcdctl
chmod +x /usr/local/bin/lcdctl

echo "[4/6] Install fb_progress"
if [ -f fb_progress ]; then
    cp fb_progress /usr/local/bin/fb_progress
    chmod +x /usr/local/bin/fb_progress
fi

echo "[5/6] Install boot script and systemd service"
cp scripts/run_lcd_app.sh /root/run_lcd_app.sh
chmod +x /root/run_lcd_app.sh

cp systemd/lcd-app.service /etc/systemd/system/lcd-app.service

echo "[6/6] Enable and restart service"
mkdir -p /var/lib/lcd-ui

systemctl daemon-reload
systemctl enable lcd-app.service
systemctl restart lcd-app.service

echo "Install OK"
lcdctl status
