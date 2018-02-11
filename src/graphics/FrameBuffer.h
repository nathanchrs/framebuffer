#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <linux/fb.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstring>
#include <string>
#include "Color.h"
#include "Point.h"
#include "Path.h"
#include "VectorSprite.h"
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
	Color getPixel(Point<long> point) const;
	void setPixel(Point<long> point, const Color &color);
	void clear(const Color &color);

	void drawCircle(Point<double> center, double r, const Color &color);
	void drawCircleOutline(Point<double> center, double r, double thickness, const Color &color);

	void drawLine(Point<double> p1, Point<double> p2, const Color &color);
	void drawPath(Point<double> topLeftPosition, Path path, const Color &fillColor, const Color &strokeColor);
	void drawText(Point<double> topLeftPosition, const std::string &text, const Font &font, double size, const Color &fillColor, const Color &strokeColor);
	void drawVectorSprite(Point<double> topLeftPosition, const VectorSprite &sprite);

  private:
	bool antiAliasing;
	int fileDescriptor;
	uint8_t *address;
	uint8_t *backBuffer;
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
};

#endif
