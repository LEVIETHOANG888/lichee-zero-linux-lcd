# Buildroot LCD App SSH Final

## Final Image

Final working image:

F:\lichee-buildroot-final\lichee-zero-buildroot-lcd-app-ssh-v1-final.img

SHA256 file:

F:\lichee-buildroot-final\lichee-zero-buildroot-lcd-app-ssh-v1-final.img.sha256

## Hardware

- Lichee Pi Zero / Allwinner V3s
- 800x480 RGB LCD
- microSD card
- USB TTL serial adapter
- RJ45 Ethernet direct connection to Windows PC

## Login

Serial:

115200 baud
login: root
password: root1234

SSH:

ssh root@192.168.10.2

Password:

root1234

## Network

Lichee static IP:

192.168.10.2/24

Windows Ethernet static IP:

IP address: 192.168.10.1
Subnet mask: 255.255.255.0
Gateway: empty
DNS: empty

## Verify System

Run on Lichee:

cat /etc/os-release
ls /dev/fb0
ip addr show eth0
ps | grep sshd
ps | grep lcd_app
lcdctl status

Expected:

/dev/fb0 exists
eth0 has 192.168.10.2
sshd running
lcd_app running
fb0: OK
app process: running

## LCD Commands

lcdctl ready
lcdctl start
lcdctl stop
lcdctl info
lcdctl progress 30
lcdctl progress 80
lcdctl status
lcdctl repaint

## Final Status

Working:

- Buildroot boots successfully
- LCD framebuffer works
- /dev/fb0 exists
- LCD UI starts automatically
- Progress update works
- OpenSSH server runs
- Static Ethernet IP works
- SSH from Windows works

## Important Note

This final image uses Buildroot kernel/rootfs with Debian working U-Boot, boot.scr, and DTB because the Debian bootloader initializes the LCD correctly.

Do not delete the final image unless another verified working backup exists.
