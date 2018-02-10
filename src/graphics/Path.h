#ifndef PATH_H
#define PATH_H

#include <vector>
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

    Path rotate(double rotationAngle, Point<double> origin) const;
    Path rotate(double rotationAngle) const { return rotate(rotationAngle, Point<double>());}

    std::vector<PathSegment<double>> segments;
};

#endif
