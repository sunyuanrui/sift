CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -Werror -Wextra
LDFLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_image -lSDL2_gfx