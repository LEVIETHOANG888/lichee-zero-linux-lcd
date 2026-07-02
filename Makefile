.RECIPEPREFIX := >

CC=gcc
CFLAGS=-std=gnu99 -O2 -Wall

APP=lcd_app
PROGRESS=fb_progress

APP_SRC=main.c gfx.c ui.c
PROGRESS_SRC=fb_progress.c

all: $(APP) $(PROGRESS)

$(APP): $(APP_SRC)
>$(CC) $(CFLAGS) -o $(APP) $(APP_SRC)

$(PROGRESS): $(PROGRESS_SRC)
>$(CC) $(CFLAGS) -o $(PROGRESS) $(PROGRESS_SRC)

install: all
>./scripts/install.sh

restart: all
>systemctl restart lcd-app.service

status:
>lcdctl status

check:
>./scripts/check.sh

clean:
>rm -f $(APP) $(PROGRESS) *.o
