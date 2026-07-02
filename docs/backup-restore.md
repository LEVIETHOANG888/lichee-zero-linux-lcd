# Backup and Restore

## Backup files

Current project backups:

    /root/backups/lcd-ui-v1-framebuffer.tar.gz
    /root/backups/lcd-ui-lvgl-workspace-v0.1.tar.gz

## Backup type

These are project-level backups.

They are not full microSD card image backups.

## Restore framebuffer V1 project

Stop service first:

    systemctl stop lcd-app.service

Restore project:

    cd /root
    rm -rf /root/lcd-ui
    tar -xzf /root/backups/lcd-ui-v1-framebuffer.tar.gz -C /root

Install again:

    cd /root/lcd-ui
    ./scripts/install.sh

Check:

    ./scripts/check.sh

## Restore LVGL workspace

    cd /root
    rm -rf /root/lcd-ui-lvgl
    tar -xzf /root/backups/lcd-ui-lvgl-workspace-v0.1.tar.gz -C /root

Check:

    cd /root/lcd-ui-lvgl
    ./scripts/check.sh

## Notes

If the whole microSD card image is broken, these backups are not enough.

For full protection, copy these backup files to PC or push the final project to GitHub later.
