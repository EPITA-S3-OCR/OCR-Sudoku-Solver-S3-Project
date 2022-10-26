# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: image_split

SRC = image_split.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

image_split: image_split.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	rm -rf Tiles/*
# END
