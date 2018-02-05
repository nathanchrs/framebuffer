#ifndef PATH_SEGMENT_H
#define PATH_SEGMENT_H

#include "Point.h"

class PathSegment {
public:
	PathSegment(Point<double> start, Point<double> end) : start(start), end(end) {}

	Point<double> start;
	Point<double> end;
};

#endif
