# Storage Resize

## Problem

The microSD card is 29.7GB, but the root filesystem was only using about 1GB.

Before resize:

    /dev/root  1022M  888M  65M  94% /

Partition layout before resize:

    /dev/mmcblk0p1  16M  FAT32
    /dev/mmcblk0p2   1G  Linux

## Goal

Expand /dev/mmcblk0p2 to use the available microSD card space.

Important:

    Keep start sector of /dev/mmcblk0p2 as 34816.

## Steps Used

Open fdisk:

    fdisk /dev/mmcblk0

Inside fdisk:

    p
    d
    2
    n
    p
    2
    34816
    Enter for last sector
    n if asked to remove signature
    w

Reboot:

    sync
    reboot

Resize ext4 filesystem:

    resize2fs /dev/mmcblk0p2

## Result

After resize:

    /dev/root  30G  891M  28G  4% /

## Verification

Framebuffer V1 still works after resize:

    cd /root/lcd-ui
    ./scripts/check.sh

Expected:

    service: active
    fb0: OK
    app process: running
