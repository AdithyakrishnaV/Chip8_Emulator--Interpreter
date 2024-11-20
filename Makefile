FLAG= $(shell sdl2-config --cflags --libs)

all:
	gcc main.c -o chip8 $(FLAG)


