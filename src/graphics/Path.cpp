#include "Path.h"
#include <cmath>

std::vector<PathSegment<long> > Path::getSegmentsWithIntegerCoordinates() const {
    std::vector<PathSegment<long> > integerSegments;
    for (std::size_t i = 0; i < segments.size(); i++) {
        integerSegments.push_back(PathSegment<long>(
            segments[i].start.toLong(),
            segments[i].end.toLong()
        ));
    }
    return integerSegments;
}

Path Path::scale(double scalingFactor, Point<double> origin) const {
    Path newPath(*this);
    for (std::size_t i = 0; i < segments.size(); i++) {
        newPath.segments[i].start = ((segments[i].start - origin) * scalingFactor) + origin;
        newPath.segments[i].end = ((segments[i].end - origin) * scalingFactor) + origin;
    }
    return newPath;
}

Path Path::translate(Point<double> delta) const {
    Path newPath(*this);
    for (std::size_t i = 0; i < segments.size(); i++) {
        newPath.segments[i].start = segments[i].start + delta;
        newPath.segments[i].end = segments[i].end + delta;
    }
    return newPath;
}
