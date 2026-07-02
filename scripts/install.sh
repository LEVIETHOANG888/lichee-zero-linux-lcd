#!/bin/sh
set -e

APP_DIR="/root/lcd-ui"

echo "[1/6] Check project dir"
cd "$APP_DIR"

echo "[2/6] Build"
make

echo "[3/6] Install lcdctl"
cp scripts/lcdctl /usr/local/bin/lcdctl
chmod +x /usr/local/bin/lcdctl

echo "[4/6] Install fb_progress"
cp fb_progress /usr/local/bin/fb_progress
chmod +x /usr/local/bin/fb_progress

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
