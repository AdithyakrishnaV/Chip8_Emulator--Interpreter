FLAG= $(shell sdl2-config --cflags --libs)

all:
	gcc main.c -g -o chip8 $(FLAG)


