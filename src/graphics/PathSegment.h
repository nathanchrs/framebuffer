#ifndef PATH_SEGMENT_H
#define PATH_SEGMENT_H

#include "Point.h"

template<class T>
class PathSegment {
public:
	PathSegment(Point<T> start, Point<T> end) : start(start), end(end) {}

	Point<T> start;
	Point<T> end;
};

#endif
