#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>
#include "Point.h"
#include "PathSegment.h"

class Path {
public:
    Path() {}
    Path(std::string filePath);
    Path(std::vector<PathSegment<double>> segments) : segments(segments) {}

    std::vector<PathSegment<long> > getSegmentsWithIntegerCoordinates() const;

    Path scale(Point<double> scalingFactor, Point<double> origin) const;
    Path scale(double scalingFactor, Point<double> origin) const { return scale(Point<double>(scalingFactor, scalingFactor), origin); }
    Path scale(Point<double> scalingFactor) const { return scale(scalingFactor, Point<double>()); }
    Path scale(double scalingFactor) const { return scale(scalingFactor, Point<double>()); }

    Path translate(Point<double> delta) const;

    Path rotate(double rotationAngle, Point<double> origin) const;
    Path rotate(double rotationAngle) const { return rotate(rotationAngle, Point<double>()); }

    Path mirrorHorizontal(double mirrorX) const;
    Path mirrorHorizontal() const { return mirrorHorizontal(0); }
    Path mirrorVertical(double mirrorY) const;
    Path mirrorVertical() const { return mirrorVertical(0); }

    PathSegment<double> getPrevSegment(size_t index);
    PathSegment<double> getSuccSegment(size_t index);

    std::vector<PathSegment<double>> segments;
};

#endif
