#ifndef BITMAP_H
#define BITMAP_H
#define HEADER_SIZE 54

#include <string>
#include <cmath>
#include "FrameBuffer.h"

class Bitmap {
public:
	Bitmap(std::string bitmapFilePath);
    ~Bitmap();

    void draw(FrameBuffer &fb, Point<long> position);
    void draw(FrameBuffer &fb, Point<double> position) {
        Point<long> roundedPosition(round(position.x), round(position.y));
        draw(fb, roundedPosition);
    }

    uint32_t fileSize;
    uint32_t dataOffset;
    uint32_t width;
    uint32_t height;
    uint16_t depth;
    bool transparentBackground = false;

private:
	char header[54];
    char *image;
};

#endif
