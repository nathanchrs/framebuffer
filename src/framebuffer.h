#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#define BACK_BUFFER_CAPACITY 3084000

#define COLOR_BLACK ((Color) { 0, 0, 0 })
#define COLOR_WHITE ((Color) { 0xff, 0xff, 0xff })

#include <linux/fb.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>

typedef struct {
	int fileDescriptor;
	uint8_t *address;
	uint32_t backBuffer[BACK_BUFFER_CAPACITY];
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
} FrameBuffer;

typedef struct {
	char r;
	char g;
	char b;
} Color;

typedef struct {
	long x;
	long y;
} Point;

Color c_make(char r, char g, char b);

Point p_make(long x, long y);

void fb_init(FrameBuffer *fb, char *fbPath);
void fb_printInfo(FrameBuffer *fb);
void fb_output(FrameBuffer *fb);

long fb_getWidth(FrameBuffer *fb);
long fb_getHeight(FrameBuffer *fb);

void fb_clear(FrameBuffer *fb, Color color);
void fb_drawPixel(FrameBuffer *fb, Point point, Color color);
void fb_drawRectangle(FrameBuffer *fb, Point p1, Point p2, Color color);

#endif
