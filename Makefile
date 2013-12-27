all:
	gcc -std=c11 -Wall -g main.c std.c bitmap.c hsl.c blur.c fisheye.c -lm -o bmp
