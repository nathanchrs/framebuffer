#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <linux/fb.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstring>
#include "Color.h"
#include "Point.h"

class FrameBuffer {
public:
	FrameBuffer(const char* fbPath);
	~FrameBuffer();

  	void printInfo();

  	long getScreenMemorySize() { return vinfo.yres_virtual * finfo.line_length; }
	long getWidth() { return vinfo.xres; }
	long getHeight() { return vinfo.yres; }

	void output();
	void setPixel(Point point, Color color);
	void clear(Color color);

	void drawRectangle(Point p1, Point p2, Color color);
	void drawCircle(Point center, double r, Color color);
	void drawCircleOutline(Point center, double r, double thickness, Color color);

private:
	int fileDescriptor;
	uint8_t *address;
	uint8_t *backBuffer;
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
};

#endif
