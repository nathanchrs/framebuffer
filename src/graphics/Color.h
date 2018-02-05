#ifndef COLOR_H
#define COLOR_H

#include <cmath>

class Color {
public:
	Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) : a(a), r(r), g(g), b(b) {}
	Color(unsigned char r, unsigned char g, unsigned char b) : a(0xff), r(r), g(g), b(b) {}	

	friend Color overlayColor(const Color &baseColor, const Color &overlayColor) {
		unsigned char r = baseColor.r + round((double) (overlayColor.a * (overlayColor.r - baseColor.r)) / 255.0);
		unsigned char g = baseColor.g + round((double) (overlayColor.a * (overlayColor.g - baseColor.g)) / 255.0);
		unsigned char b = baseColor.b + round((double) (overlayColor.a * (overlayColor.b - baseColor.b)) / 255.0);
		return Color(r, g, b);
	}

	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

#endif
