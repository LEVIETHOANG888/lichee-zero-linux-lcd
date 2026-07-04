#!/bin/sh
set -e

BUILDROOT_DIR="$HOME/lichee-buildroot/buildroot-2025.02.15"
DEBIAN_BACKUP="/mnt/f/debian-v1-full-backup.img"
OUT_DIR="/mnt/f/lichee-buildroot-final"
OUT_IMG="$OUT_DIR/lichee-zero-buildroot-lcd-app-ssh-v1-final.img"

mkdir -p "$OUT_DIR"

cd "$BUILDROOT_DIR"

sudo mkdir -p /mnt/debian-v1-boot
if ! mountpoint -q /mnt/debian-v1-boot; then
    sudo mount -o ro,loop,offset=$((2048*512)) "$DEBIAN_BACKUP" /mnt/debian-v1-boot
fi

cp output/images/sdcard.img "$OUT_IMG"

dd if=/mnt/debian-v1-boot/u-boot-sunxi-with-spl.bin of="$OUT_IMG" bs=1024 seek=8 conv=notrunc

sudo mkdir -p /mnt/br-final-boot
sudo mount -o loop,offset=$((2048*512)) "$OUT_IMG" /mnt/br-final-boot

sudo cp /mnt/debian-v1-boot/boot.scr /mnt/br-final-boot/boot.scr
sudo cp /mnt/debian-v1-boot/sun8i-v3s-licheepi-zero.dtb /mnt/br-final-boot/sun8i-v3s-licheepi-zero.dtb

sync
sudo umount /mnt/br-final-boot

cd "$OUT_DIR"
sha256sum "$(basename "$OUT_IMG")" > "$(basename "$OUT_IMG").sha256"

ls -lh "$OUT_IMG" "$OUT_IMG.sha256"
