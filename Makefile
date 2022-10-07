# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: image_split

SRC = src/image_split.c
OBJ = src/${SRC:.c=.o}
EXE = src/${SRC:.c=}

image_split: src/image_split.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	rm -rf src/Tiles/*
# END
