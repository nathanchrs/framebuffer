#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <linux/fb.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstring>
#include <string>
#include <vector>
#include "Color.h"
#include "Point.h"
#include "PathSegment.h"
#include "Font.h"

class FrameBuffer {
public:
	FrameBuffer(const char* fbFilePath);
	~FrameBuffer();

  	void printInfo();

  	long getScreenMemorySize() { return vinfo.yres_virtual * finfo.line_length; }
	long getWidth() { return vinfo.xres; }
	long getHeight() { return vinfo.yres; }

	void output();
	Color getPixel(Point<long> point);
	void setPixel(Point<double> point, Color color);
	void setPixel(Point<long> point, Color color);
	void clear(Color color);

	void drawCircle(Point<long> center, double r, Color color);
	void drawCircleOutline(Point<long> center, double r, double thickness, Color color);

	void drawLine(Point<double> p1, Point<double> p2, Color color);
	void drawPath(Point<double> topLeftPosition, std::vector<PathSegment> segments, Color fillColor, Color strokeColor);
	void drawText(Point<double> topLeftPosition, std::string text, Font &font, Color fillColor, Color strokeColor);

private:
	bool antiAliasing;
	int fileDescriptor;
	uint8_t *address;
	uint8_t *backBuffer;
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;

	void bresenhamDrawLine(Point<long> p1, Point<long> p2, Color color);
	void xwDrawLine(Point<double> p1, Point<double> p2, Color color);
};

#endif
