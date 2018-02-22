#include "FrameBuffer.h"
#include <iostream>
#define EPS 1e-9

#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

int computeOutCode(double x, double y, double xmin, double ymin, double xmax, double ymax)
{
  int code;
  code = INSIDE;

  if (x < xmin) {
    code |= LEFT;
  }
	else if (x > xmax) {
	  code |= RIGHT;
	}
  if (y < ymin) {
    code |= TOP;
  }
  else if (y > ymax) {
    code |= BOTTOM;
  }

  return code;
}

bool clipLine(Point<double> &p0, Point<double> &p1, Point<double> topLeft, Point<double> bottomRight) {
  double xmin = topLeft.x;
  double ymin = topLeft.y;
  double xmax = bottomRight.x;
  double ymax = bottomRight.y;  
  
	double x0 = p0.x;
	double y0 = p0.y;
	double x1 = p1.x;
	double y1 = p1.y;
	
	int outcode0 = computeOutCode(x0, y0, xmin, ymin, xmax, ymax);
	int outcode1 = computeOutCode(x1, y1, xmin, ymin, xmax, ymax);
	bool accept = false;
	
	while (true) {
		if (!(outcode0 | outcode1)) {
			//std::cout << "Trivial accept" << std::endl;
			accept = true;
			break;
		} else if (outcode0 & outcode1) {
			//std::cout << "Trivial reject" << std::endl;
			break;
		} else {
			double x, y;
			int outcodeOut = outcode0 ? outcode0 : outcode1;
			if (outcodeOut & BOTTOM) {
			  //std::cout << "It's in Bottom" << std::endl;
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			} else if (outcodeOut & TOP) {
			  //std::cout << "It's in Top" << std::endl;
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			} else if (outcodeOut & RIGHT) {
			  //std::cout << "It's in Right" << std::endl;
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			} else if (outcodeOut & LEFT) {
			  //std::cout << "It's in Left" << std::endl;
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}

			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = computeOutCode(x0, y0, xmin, ymin, xmax, ymax);
        //std::cout << "P0 changed to (" << x0 << "," << y0 << "), outcode is " << outcode0 << std::endl;
			} else {
				x1 = x;
				y1 = y;
				outcode1 = computeOutCode(x1, y1, xmin, ymin, xmax, ymax);
        //std::cout << "P1 changed to (" << x1 << "," << y1 << "), outcode is " << outcode1 << std::endl;
			}
		}
	}
	if (accept) {
		//std::cout << "accept" << std::endl;
		p0.x = x0;
		p0.y = y0;
		p1.x = x1;
		p1.y = y1;
		return true;
	}
	else {
    //std::cout << "reject" << std::endl;
	  return false;
	}
}

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

	// Memory map framebuffer, allocate back buffer
	long screenMemorySize = vinfo.yres_virtual * finfo.line_length;
	address = (uint8_t*) mmap(0, screenMemorySize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, (off_t) 0);
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

Color FrameBuffer::getPixel(Point<long> point) const {
	if (point.x >= 0 && point.x < vinfo.xres && point.y >= 0 && point.y < vinfo.yres) {
		long addressOffset = point.x * (vinfo.bits_per_pixel / 8) + (point.y * finfo.line_length);
		uint32_t colorValue = *((uint32_t *)(backBuffer + addressOffset));
		return Color(
			(colorValue >> vinfo.red.offset) & 0xff,
			(colorValue >> vinfo.green.offset) & 0xff,
			(colorValue >> vinfo.blue.offset) & 0xff
		);
	} else {
		return Color(0, 0, 0, 0);
	}
}

void FrameBuffer::setPixel(Point<long> point, const Color &color) {
	if (point.x >= 0 && point.x < vinfo.xres && point.y >= 0 && point.y < vinfo.yres) {
		long addressOffset = point.x * (vinfo.bits_per_pixel / 8) + (point.y * finfo.line_length);

		if (color.a != 0xff) {
			Color mixedColor = overlayColor(getPixel(point), color);

			uint32_t colorValue = (mixedColor.r << vinfo.red.offset)
				| (mixedColor.g << vinfo.green.offset)
				| (mixedColor.b << vinfo.blue.offset);

			*((uint32_t*)(backBuffer + addressOffset)) = colorValue;

		} else {
			uint32_t colorValue = (color.r << vinfo.red.offset)
				| (color.g << vinfo.green.offset)
				| (color.b << vinfo.blue.offset);

			*((uint32_t*)(backBuffer + addressOffset)) = colorValue;
		}
	}
}

void FrameBuffer::clear(const Color &color) {
	for (long x = 0; x < getWidth(); x++) {
		for (long y = 0; y < getHeight(); y++) {
			setPixel(Point<long>(x, y), color);
		}
	}
}

void FrameBuffer::drawCircle(Point<double> center, double r, const Color &color) {
	for (long x = center.x - r; x <= center.x + r; x++) {
		for (long y = center.y - r; y <= center.y + r; y++) {
			double dx = center.x - x;
			double dy = center.y - y;
			if (dx*dx + dy*dy <= r*r) {
				setPixel(Point<long>(x, y), color);
			}
		}
	}
}

void FrameBuffer::drawCircleOutline(Point<double> center, double r, double thickness, const Color &color) {
	for (long x = center.x - r; x <= center.x + r; x++) {
		for (long y = center.y - r; y <= center.y + r; y++) {
			double dx = center.x - x;
			double dy = center.y - y;
			double hypotSquared = dx*dx + dy*dy;
			double innerRadius = r - thickness;
			if ((hypotSquared <= r*r) && (hypotSquared > innerRadius*innerRadius)) {
				setPixel(Point<long>(x, y), color);
			}
		}
	}
}

void FrameBuffer::drawLine(Point<double> p1, Point<double> p2, const Color &color, Point<double> topLeft, Point<double> bottomRight) {
  if (clipLine(p1, p2, topLeft, bottomRight)) {
	  long x0 = round(p1.x);
	  long y0 = round(p1.y);
	  long x1 = round(p2.x);
	  long y1 = round(p2.y);
	  long dx = (x1 - x0 > 0 ? x1 - x0 : x0 - x1), sx = x0 < x1 ? 1 : -1;
	  long dy = -(y1 - y0 > 0 ? y1 - y0 : y0 - y1), sy = y0 < y1 ? 1 : -1;
	  long err = dx + dy, e2; /* error value e_xy */

	  while (true) {
		  setPixel(Point<long>(x0, y0), color);
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
}

/* Helper function for drawing values a working area buffer buf */
void drawLineBuf(uint8_t *buf, long bufWidth, Point<long> p1, Point<long> p2, uint8_t value) {
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

void FrameBuffer::drawPathClip(Point<double> topLeftPosition, Path path, const Color &fillColor, const Color &strokeColor, Point<double> clipTopLeft, Point<double> clipBottomRight) {
	if (path.segments.size() <= 0) return;
	std::vector<PathSegment<double> > segments = path.segments;
	std::vector<double> inter;
	
	// Determine the max and min of y (height) and x
	// double xmin = segments[0].start.x;
	// double xmax = segments[0].start.x;
	double ymin = segments[0].start.y;
	double ymax = segments[0].start.y;
	for (size_t i = 0; i < segments.size(); i++) {
	  //std::cout << i << ". (" << segments[i].start.x << "," << segments[i].start.y << ") (" << segments[i].end.x << "," << segments[i].end.y << ")" << std::endl;
		//if (segments[i].start.x > xmax) xmax = segments[i].start.x;
		//if (segments[i].start.x < xmin) xmin = segments[i].start.x;
		//if (segments[i].end.x > xmax) xmax = segments[i].end.x;
		//if (segments[i].end.x < xmin) xmin = segments[i].end.x;
		if (segments[i].start.y > ymax) ymax = segments[i].start.y;
		if (segments[i].start.y < ymin) ymin = segments[i].start.y;
		if (segments[i].end.y > ymax) ymax = segments[i].end.y;
		if (segments[i].end.y < ymin) ymin = segments[i].end.y;
	}
	//std::cout << "Xmax : " << xmax << std::endl;
	//std::cout << "Xmin : " << xmin << std::endl;
	//std::cout << "Ymax : " << ymax << std::endl;
	//std::cout << "Ymin : " << ymin << std::endl;
	
	//xmin += topLeftPosition.x;
	//xmax += topLeftPosition.x;
	ymin += topLeftPosition.y < clipTopLeft.y ? clipTopLeft.y : topLeftPosition.y;
	ymax += topLeftPosition.y > clipBottomRight.y ? clipBottomRight.y : topLeftPosition.y;
	
	// Do scanline for each horizontal line from y = ymin to y = ymax
	for (double y = ymin; y <= ymax; y += 1.0) {
	  //std::cout << std::endl << "Current y : " << y << std::endl;
	  double x1, x2, y1, y2, temp;
	  inter.clear();
	  
	  // Determine all intersection point
	  for (size_t i = 0; i < segments.size(); i++) {
	    x1 = segments[i].start.x+topLeftPosition.x;
	    x2 = segments[i].end.x+topLeftPosition.x;
	    y1 = segments[i].start.y+topLeftPosition.y;
	    y2 = segments[i].end.y+topLeftPosition.y;
	    
	    // Make sure y1 <= y2
	    if (y2 < y1) {
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
	    }
	    
	    // If line Y intersect segments[i]
	    if (y <= y2 && y >= y1) {
	      double x;
	      bool pushit = true;
	      if ((y1 - y2) == 0) {
	        x = x1;
	          //std::cout << "Found horizontal line (" << segments[i].start.x << "," << segments[i].start.y << ") (" << segments[i].end.x << "," << segments[i].end.y << ")" << std::endl;
	          PathSegment<double> pred = path.getPrevSegment(i);
	          //std::cout << "Predecessor line: (" << pred.start.x << "," << pred.start.y << ") (" << pred.end.x << "," << pred.end.y << ")" << std::endl;
	          PathSegment<double> succ = path.getSuccSegment(i);
	          //std::cout << "Successor line: (" << succ.start.x << "," << succ.start.y << ") (" << succ.end.x << "," << succ.end.y << ")" << std::endl;
	          if ((pred.start.y < pred.end.y && succ.start.y > succ.end.y) ||
	              (pred.start.y > pred.end.y && succ.start.y < succ.end.y)) {
	            //std::cout << "Don't push it!" << std::endl;
	            pushit = false;
	          }
	          //else {
	            //std::cout << "Push it!" << std::endl;
	          //}
	      }
	      else {
	        x = ((x2 - x1) * (y - y1)) / (y2 - y1);
          x = x + x1;
	      }
	      if (pushit) {
	        inter.push_back(x);
	        //std::cout << inter.size() << ". " << x << " from line (" << x1 << "," << y1 << ") (" << x2 << "," << y2 << ")" << std::endl;
	      }
	    }
	  }
	  //std::cout << "Total intersections : " << inter.size() << std::endl;
	  
	  // Sort the points
	  if (inter.size() > 0) {
			for (size_t i = inter.size()-1; i >= 1; i--) {
				for (size_t j = 0; j < i; j++) {
					if (inter[j] > inter[i]) {
						temp = inter[i];
						inter[i] = inter[j];
						inter[j] = temp;
					}
				}
			}
	  }
	  //std::cout << "Sorted" << std::endl << std::endl;  
	  
	  // Draw the line
	  for (size_t i = 0; i < inter.size(); i+=2) {
	    drawLine(Point<double>(inter[i], y), Point<double>(inter[i+1], y), fillColor, clipTopLeft, clipBottomRight);
	  }
	  //std::cout << "Fill for y = " << y << " drawn" << std::endl << std::endl;
	}
	
	// Last, draw the outline
	for (size_t i = 0; i < segments.size(); i++) {
	  drawLine(
			Point<double>(segments[i].start.x+topLeftPosition.x, segments[i].start.y+topLeftPosition.y),
			Point<double>(segments[i].end.x+topLeftPosition.x, segments[i].end.y+topLeftPosition.y),
			strokeColor,
			clipTopLeft,
			clipBottomRight
		);
	}
}

void FrameBuffer::drawPath(Point<double> topLeftPosition, Path path, const Color &fillColor, const Color &strokeColor) {
	drawPathClip(topLeftPosition, path, fillColor, strokeColor, Point<double>(0, 0), Point<double>(this->getWidth(), this->getHeight()));
}

void FrameBuffer::drawText(Point<double> topLeftPosition, const std::string &text, const Font &font, double size, const Color &fillColor, const Color &strokeColor) {
	drawPath(topLeftPosition, font.getTextPath(text, size), fillColor, strokeColor);
}

void FrameBuffer::drawVectorSpriteClip(Point<double> topLeftPosition, const VectorSprite &sprite, Point<double> clipTopLeft, Point<double> clipBottomRight) {
	for (size_t i = 0; i < sprite.paths.size(); i++) {
		drawPathClip(topLeftPosition, sprite.paths[i], sprite.fillColors[i], sprite.strokeColors[i], clipTopLeft, clipBottomRight);
	}
}

void FrameBuffer::drawVectorSprite(Point<double> topLeftPosition, const VectorSprite &sprite) {
	for (size_t i = 0; i < sprite.paths.size(); i++) {
		drawPath(topLeftPosition, sprite.paths[i], sprite.fillColors[i], sprite.strokeColors[i]);
	}
}