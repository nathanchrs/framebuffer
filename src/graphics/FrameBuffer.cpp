#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(const char* fbPath) {
	// Open frame buffer device
	fileDescriptor = open(fbPath, O_RDWR);
	if (!fileDescriptor) {
		std::cerr << "Error: can't open framebuffer" << std::endl;
		return;
	}

	// Write settings to variable screen info, then re-read
	ioctl(fileDescriptor, FBIOGET_VSCREENINFO, &vinfo);
	vinfo.grayscale = 0;
	vinfo.bits_per_pixel = 32;
	vinfo.xoffset = 0;
	vinfo.yoffset = 0;
	ioctl(fileDescriptor, FBIOPUT_VSCREENINFO, &vinfo);
	ioctl(fileDescriptor, FBIOGET_VSCREENINFO, &vinfo);

	// Read fixed screen info
	ioctl(fileDescriptor, FBIOGET_FSCREENINFO, &finfo);

	long screenMemorySize = vinfo.yres_virtual * finfo.line_length;

	// Memory map framebuffer
	address = (uint8_t*) mmap(0, screenMemorySize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, (off_t) 0);

	// Allocate memory for back buffer
	backBuffer = new uint8_t[screenMemorySize];
}

FrameBuffer::~FrameBuffer() {
	delete[] backBuffer;
}

void FrameBuffer::printInfo() {
	std::cout << "xres: " << vinfo.xres << std::endl;
	std::cout << "yres: " << vinfo.yres << std::endl;
	std::cout << "xres_virtual: " << vinfo.xres_virtual << std::endl;
	std::cout << "yres_virtual: " << vinfo.yres_virtual << std::endl;
	std::cout << "xoffset: " << vinfo.xoffset << std::endl;
	std::cout << "yoffset: " << vinfo.yoffset << std::endl;
	std::cout << "bits_per_pixel: " << vinfo.bits_per_pixel << std::endl;
	std::cout << "line_length: " << finfo.line_length << std::endl;
}

void FrameBuffer::output() {
	memcpy((uint8_t*) address, backBuffer, getScreenMemorySize());
}

void FrameBuffer::setPixel(Point point, Color color) {
	if (point.x >= 0 && point.x < vinfo.xres && point.y >= 0 && point.y < vinfo.yres) {
		long addressOffset = point.x * (vinfo.bits_per_pixel / 8) + (point.y * finfo.line_length);

		uint32_t colorValue = (color.r << vinfo.red.offset)
			| (color.g << vinfo.green.offset)
			| (color.b << vinfo.blue.offset);

		*((uint32_t*)(backBuffer + addressOffset)) = colorValue;
	}
}

void FrameBuffer::clear(Color color) {
	for (long x = 0; x < getWidth(); x++) {
		for (long y = 0; y < getHeight(); y++) {
			setPixel(Point(x, y), color);
		}
	}
}

void FrameBuffer::drawRectangle(Point p1, Point p2, Color color) {
	for (long x = p1.x; x < p2.x; x++) {
		for (long y = p1.y; y < p2.y; y++) {
			setPixel(Point(x, y), color);
		}
	}
}

void FrameBuffer::drawCircle(Point center, double r, Color color) {
	for (long x = center.x - (long) r; x <= center.x + (long) r; x++) {
		for (long y = center.y - (long) r; y <= center.y + (long) r; y++) {
			long dx = center.x - x;
			long dy = center.y - y;
			if (dx*dx + dy*dy <= (long) (r*r)) {
				setPixel(Point(x, y), color);
			}
		}
	}	
}

void FrameBuffer::drawCircleOutline(Point center, double r, double thickness, Color color) {
	for (long x = center.x - (long) r; x <= center.x + (long) r; x++) {
		for (long y = center.y - (long) r; y <= center.y + (long) r; y++) {
			long dx = center.x - x;
			long dy = center.y - y;
			long innerRadius = r - thickness;
			if ((dx*dx + dy*dy <= (long) (r*r)) && (dx*dx + dy*dy > (long) (innerRadius*innerRadius))) {
				setPixel(Point(x, y), color);
			}
		}
	}	
}
