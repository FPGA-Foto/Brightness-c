all:
	gcc main.c std.c bitmap.c hsl.c blur.c fisheye.c -lm -o bmp
