#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) : a(a), r(r), g(g), b(b) {}
	Color(unsigned char r, unsigned char g, unsigned char b) : a(0xff), r(r), g(g), b(b) {}

	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

#endif
