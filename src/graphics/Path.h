#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>
#include "Point.h"
#include "PathSegment.h"

class Path {
public:
    Path() {}
	Path(std::vector<PathSegment<double> > segments) : segments(segments) {}

    std::vector<PathSegment<long> > getSegmentsWithIntegerCoordinates() const;

    Path scale(double scalingFactor, Point<double> origin) const;
    Path scale(double scalingFactor) const { return scale(scalingFactor, Point<double>()); }

    Path translate(Point<double> delta) const;

    std::vector<PathSegment<double>> segments;
};

#endif
