#include "Bitmap.h"
#include "Color.h"
#include <iostream>
#include <fstream>

Bitmap::Bitmap(std::string bitmapFilePath) {
    std::ifstream fin(bitmapFilePath, std::ios::binary);
    fin.read(header, HEADER_SIZE);

    fileSize = *reinterpret_cast<uint32_t *>(&header[2]);
    dataOffset = *reinterpret_cast<uint32_t *>(&header[10]);
    width = *reinterpret_cast<uint32_t *>(&header[18]);
    height = *reinterpret_cast<uint32_t *>(&header[22]);
    depth = *reinterpret_cast<uint16_t *>(&header[28]);

    uint32_t paddingSize = dataOffset - HEADER_SIZE;
    char *padding = new char[paddingSize];
    fin.read(padding, paddingSize);
    delete[] padding;

    uint32_t imageSize = ((width * 3 + 3) & (~3)) * height;
    image = new char[imageSize];
    fin.read(image, imageSize);

    std::cout << "width: " << width << std::endl;
}

Bitmap::~Bitmap() {
    delete[] image;
}

void Bitmap::draw(FrameBuffer &fb, Point<long> position) {
    Color mask(image[2], image[1], image[0]);
    for (long y = 0; y < height; y++) {
        for (long x = 0; x < width; x++) {
            Color color(
                image[(y * (width+1) + x) * 3 + 2],
                image[(y * (width+1) + x) * 3 + 1],
                image[(y * (width+1) + x) * 3]
            );
            if (!transparentBackground || color.r != mask.r || color.g != mask.g || color.b != mask.b) {
                fb.setPixel(position + Point<long>(x, height-y-1), color);
            }
        }
    }
}
