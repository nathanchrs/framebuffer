#ifndef PATH_SEGMENT_H
#define PATH_SEGMENT_H

#include "Point.h"

class PathSegment {
public:
	PathSegment(Point start, Point end) : start(start), end(end) {}

	Point start;
	Point end;
};

#endif
