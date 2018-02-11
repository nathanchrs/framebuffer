#ifndef PATH_H
#define PATH_H

#include <vector>
#include <sstream>
#include "Point.h"
#include "PathSegment.h"

class Path {
public:
    Path() {}
	  Path(std::vector<PathSegment<double> > segments) : segments(segments) {}
	  Path(std::string);

    std::vector<PathSegment<long> > getSegmentsWithIntegerCoordinates() const;

    Path scale(double scalingFactor, Point<double> origin) const;
    Path scale(double scalingFactor) const { return scale(scalingFactor, Point<double>()); }

    Path translate(Point<double> delta) const;
    
    PathSegment<double> getPredSegment(size_t index);
    PathSegment<double> getSuccSegment(size_t index);

    std::vector<PathSegment<double>> segments;
};

#endif
