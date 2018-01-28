#ifndef POINT_H
#define POINT_H

class Point {
public:
	Point(long x, long y) : x(x), y(y) {}
	Point() : x(0), y(0) {}

	long x;
	long y;
};

#endif
