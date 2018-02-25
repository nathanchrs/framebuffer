#include "FrameBuffer.h"
#include <iostream>
#define EPS 1e-9

#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
#define abs(i) ((i>=0)?(i):(-i))

// Some struct used in scanline
typedef struct {
  double ymin;
  double ymax;
  double m;
  double y1;
  double x1;
} Bucket;

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
	std::vector<Bucket*> list;
	std::vector<Bucket*> active;
	std::vector<double> inter;
	Bucket all[segments.size()];
	
	//std::cout << "Initiate bucket list" << std::endl;
	for (size_t i = 0; i < segments.size(); i++) {
	  //std::cout << i << ". (" << segments[i].start.x << "," << segments[i].start.y << ") (" << segments[i].end.x << "," << segments[i].end.y << ")" << std::endl;
	  if (segments[i].start.y < segments[i].end.y) {
	    all[i].ymin = segments[i].start.y+topLeftPosition.y;
	    all[i].ymax = segments[i].end.y+topLeftPosition.y;
	  }
	  else {
	    all[i].ymin = segments[i].end.y+topLeftPosition.y;
	    all[i].ymax = segments[i].start.y+topLeftPosition.y;
	  }
	  all[i].m = (segments[i].end.x - segments[i].start.x) / (segments[i].end.y - segments[i].start.y);
	  all[i].x1 = segments[i].start.x+topLeftPosition.x;
	  all[i].y1 = segments[i].start.y+topLeftPosition.y;
	  //std::cout << "Bucket " << i << " (" << &(all[i]) << ") :" << std::endl;
	  //std::cout << "    ymin: " << all[i].ymin << std::endl;
	  //std::cout << "    ymax: " << all[i].ymax << std::endl;
	  //std::cout << "    m: " << all[i].m << std::endl;
	  //std::cout << "    x1: " << all[i].x1 << std::endl;
	  //std::cout << "    y1: " << all[i].y1 << std::endl;
	  list.push_back(&(all[i]));
	}
	
	
	//std::cout << std::endl << "Sorting" << std::endl;
	for (size_t i = list.size()-1; i >= 1; i--) {
		for (size_t j = 0; j < i; j++) {
			if (list[j]->ymin > list[i]->ymin) {
				Bucket* temp = list[i];
				list[i] = list[j];
				list[j] = temp;
			}
		}
	}
	for (size_t i = 0; i <list.size(); i++) {
		//std::cout << "List " << i << " (" << list[i] << "): " << list[i]->ymin << std::endl << std::endl;
	}
	
	int curY;
	if (!list.empty()) curY = list[0]->ymin;
	
	for (size_t i = 0; i < list.size(); i++) {
	  if (curY >= list[i]->ymin || (list[i]->ymin - curY) < 0.0001) {
	    Bucket* temp = list[i];
	    list.erase(list.begin() + i);
	    active.push_back(temp);
	    i--;
	  }
	  else break;
	}
	
	for (size_t i = 0; i < active.size(); i++) {
	  if (curY >= active[i]->ymax || (active[i]->ymax - curY) < 0.0001) {
	    active.erase(active.begin() + i);
	    i--;
	  }
	}
	
	while (!(list.empty() && active.empty())) {
	  /*std::cout << std::endl << "Current Y = " << curY << std::endl;
	  for (size_t i = 0; i <list.size(); i++) {
		  std::cout << "List " << i << " (" << list[i] << "): " << list[i]->ymin << std::endl;
	  }
	  for (size_t i = 0; i <active.size(); i++) {
		  std::cout << "Active " << i << " (" << active[i] << "): " << active[i]->ymin << std::endl;
	  }*/
	  
	  inter.clear();
	  
	  for (size_t i = 0; i < active.size(); i++) {
	    double x = active[i]->m * ((double) curY - active[i]->y1);
	    x += active[i]->x1;
	    inter.push_back(x);
	    //std::cout << "Active number " << i << " intersects at " << x << std::endl;
	  }
	  
	  if (inter.size() > 0) {
	    for (size_t i = inter.size()-1; i >= 1; i--) {
		    for (size_t j = 0; j < i; j++) {
			    if (inter[j] > inter[i]) {
				    double temp = inter[i];
				    inter[i] = inter[j];
				    inter[j] = temp;
			    }
		    }
	    }
	  }
	  
	  for (size_t i = 0; i < inter.size(); i+=2) {
	    //std::cout << "Draw line at (" << inter[i] << "," << curY << ") (" << inter[i+1] << "," << curY << ")" << std::endl;
	    drawLine(Point<double>(inter[i], curY), Point<double>(inter[i+1], curY), fillColor, clipTopLeft, clipBottomRight);
	  }
	  
	  curY++;
	  
	  for (size_t i = 0; i < list.size(); i++) {
	    if (curY >= list[i]->ymin || (list[i]->ymin - curY) < 0.0001) {
	      Bucket* temp = list[i];
	      list.erase(list.begin() + i);
	      active.push_back(temp);
	      i--;
	      //std::cout << "Move bucket (" << temp << ") with ymin " << temp->ymin << " into active" << std::endl;
	    }
	    else break;
	  }
	  
	  for (size_t i = 0; i < active.size(); i++) {
	    if (curY >= active[i]->ymax || (active[i]->ymax - curY) < 0.0001) {
	      //Bucket* temp = active[i];
	      active.erase(active.begin() + i);
	      i--;
	      //std::cout << "Delete bucket (" << temp << ") with ymin " << temp->ymin << std::endl;
	    }
	  }
	}
	
	//std::cout << std::endl << "Finished" << std::endl;
	
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
