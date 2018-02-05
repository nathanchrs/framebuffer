#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(const char* fbFilePath) {
	// Open frame buffer device
	fileDescriptor = open(fbFilePath, O_RDWR);
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

void FrameBuffer::drawLine(Point p1, Point p2, Color color) {
	long x0 = p1.x;
	long y0 = p1.y;
	long x1 = p2.x;
	long y1 = p2.y;
	long dx = (x1 - x0 > 0 ? x1 - x0 : x0 - x1), sx = x0 < x1 ? 1 : -1;
	long dy = -(y1 - y0 > 0 ? y1 - y0 : y0 - y1), sy = y0 < y1 ? 1 : -1;
	long err = dx + dy, e2; /* error value e_xy */

	while (true) {
		setPixel(Point(x0, y0), color);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		} /* e_xy+e_x > 0 */
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		} /* e_xy+e_y < 0 */
	}
}

/* Helper function for drawing values a working area buffer buf */
void drawLineBuf(uint8_t *buf, long bufWidth, Point p1, Point p2, uint8_t value) {
	long x0 = p1.x;
	long y0 = p1.y;
	long x1 = p2.x;
	long y1 = p2.y;
	long dx = (x1 - x0 > 0 ? x1 - x0 : x0 - x1), sx = x0 < x1 ? 1 : -1;
	long dy = -(y1 - y0 > 0 ? y1 - y0 : y0 - y1), sy = y0 < y1 ? 1 : -1;
	long err = dx + dy, e2; /* error value e_xy */

	while (true) {
		buf[y0*bufWidth + x0] = value;
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		} /* e_xy+e_x > 0 */
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		} /* e_xy+e_y < 0 */
	}
}

void FrameBuffer::drawPath(Point topLeftPosition, std::vector<PathSegment> segments, Color fillColor, Color strokeColor) {
	if (segments.size() <= 0) return;

	// Determine bounding box widths (assumes polygon points are relative to (0, 0))
	long boundingBoxWidth = segments[0].start.x;
	long boundingBoxHeight = segments[0].start.y;
	for (size_t i = 0; i < segments.size(); i++) {
		if (segments[i].start.x > boundingBoxWidth) boundingBoxWidth = segments[i].start.x;
		if (segments[i].start.y > boundingBoxHeight) boundingBoxHeight = segments[i].start.y;
		if (segments[i].end.x > boundingBoxWidth) boundingBoxWidth = segments[i].end.x;
		if (segments[i].end.y > boundingBoxHeight) boundingBoxHeight = segments[i].end.y;
	}
	boundingBoxWidth++;
	boundingBoxHeight++;	
	long boundingBoxSize = boundingBoxWidth * boundingBoxHeight;

	// Assign temporary drawing area (buf) and initialize
	uint8_t *buf = new uint8_t[boundingBoxSize];
	for (long i = 0; i < boundingBoxSize; i++) {
		buf[i] = 0;
	}

	// buf format: for each pixel,
	// - Least significant bit (0x01) used to store whether this pixel contains a stroke
	// - Next 2 bits are used to count number of horizontal fills
	// - Next 2 bits are used to count number of vertical fills
	const uint8_t STROKE_MASK = 0b00000001;
	const uint8_t HORIZONTAL_FILL_COUNT_OFFSET = 1;
	const uint8_t HORIZONTAL_FILL_COUNT_MASK = 0b00000110;
	const uint8_t VERTICAL_FILL_COUNT_OFFSET = 3;
	const uint8_t VERTICAL_FILL_COUNT_MASK = 0b00011000;

	// Draw polygon strokes on buf
	for (size_t i = 0; i < segments.size(); i++) {
		drawLineBuf(buf, boundingBoxWidth, segments[i].start, segments[i].end, STROKE_MASK);
	}

	// Left->right raster fill
	for (long r = 0; r < boundingBoxHeight; r++) {
		long c = 0;
		bool doFill = false;
		while (c < boundingBoxWidth) {
			if (buf[r * boundingBoxWidth + c] & STROKE_MASK) {
				while (c < boundingBoxWidth && (buf[r * boundingBoxWidth + c] & STROKE_MASK)) {
					c++;
				}
				doFill = !doFill;
			}
			if (c >= boundingBoxWidth) break;
			if (doFill) buf[r * boundingBoxWidth + c] += (1 << HORIZONTAL_FILL_COUNT_OFFSET);
			c++;
		}
	}

	// Right->left raster fill
	for (long r = 0; r < boundingBoxHeight; r++) {
		long c = boundingBoxWidth - 1;
		bool doFill = false;
		while (c >= 0) {
			if (buf[r * boundingBoxWidth + c] & STROKE_MASK) {
				while (c >= 0 && (buf[r * boundingBoxWidth + c] & STROKE_MASK)) {
					c--;
				}
				doFill = !doFill;
			}
			if (c < 0) break;
			if (doFill) buf[r * boundingBoxWidth + c] += (1 << HORIZONTAL_FILL_COUNT_OFFSET);
			c--;
		}
	}

	// Top->bottom raster fill
	for (long c = 0; c < boundingBoxWidth; c++) {
		long r = 0;
		bool doFill = false;
		while (r < boundingBoxHeight) {
			if (buf[r * boundingBoxWidth + c] & STROKE_MASK) {
				while (r < boundingBoxHeight && (buf[r * boundingBoxWidth + c] & STROKE_MASK)) {
					r++;
				}
				doFill = !doFill;
			}
			if (r >= boundingBoxHeight) break;
			if (doFill) buf[r * boundingBoxWidth + c] += (1 << VERTICAL_FILL_COUNT_OFFSET);
			r++;
		}
	}

	// Bottom->top raster fill
	for (long c = 0; c < boundingBoxWidth; c++) {
		long r = boundingBoxHeight - 1;
		bool doFill = false;
		while (r >= 0) {
			if (buf[r * boundingBoxWidth + c] & STROKE_MASK) {
				while (r >= 0 && (buf[r * boundingBoxWidth + c] & STROKE_MASK)) {
					r--;
				}
				doFill = !doFill;
			}
			if (r < 0) break;
			if (doFill) buf[r * boundingBoxWidth + c] += (1 << VERTICAL_FILL_COUNT_OFFSET);
			r--;
		}
	}

	// Draw stroke pixels and pixels with fill count == 3 in buf to framebuffer
	for (long r = 0; r < boundingBoxHeight; r++) {
		for (long c = 0; c < boundingBoxWidth; c++) {
			if (buf[r * boundingBoxWidth + c] & STROKE_MASK) {
				setPixel(Point(c + topLeftPosition.x, r + topLeftPosition.y), strokeColor);
			} else {
				uint8_t verticalFillCount = (buf[r * boundingBoxWidth + c] & VERTICAL_FILL_COUNT_MASK) >> VERTICAL_FILL_COUNT_OFFSET;
				uint8_t horizontalFillCount = (buf[r * boundingBoxWidth + c] & HORIZONTAL_FILL_COUNT_MASK) >> HORIZONTAL_FILL_COUNT_OFFSET;
				if ((verticalFillCount >= 2 && horizontalFillCount >= 1) || (verticalFillCount >= 1 && horizontalFillCount >= 2)) {
					setPixel(Point(c + topLeftPosition.x, r + topLeftPosition.y), fillColor);
				} else if (verticalFillCount == 1 && horizontalFillCount == 1) {
					// TODO: handle special case, perhaps see neighboring pixels
				}
			}
		}
	}

	// Free temporary drawing area
	delete[] buf;
}

void FrameBuffer::drawText(Point topLeftPosition, std::string text, Font &font, Color fillColor, Color strokeColor) {
	const long LETTER_SPACING = 6;
	long accX = topLeftPosition.x;
	for (size_t i = 0; i < text.length(); i++) {
		drawPath(Point(accX, topLeftPosition.y), font.getCharacterPath(text[i]), fillColor, strokeColor);
		accX += font.getCharacterWidth(text[i]) + LETTER_SPACING;
	}
}