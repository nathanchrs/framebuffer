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

Path Path::rotate(double rotationAngle, Point<double> origin) const{
	Path newPath(*this);
	double sinTetha = sin(rotationAngle * M_PI / 180);
	double cosTetha = cos(rotationAngle * M_PI / 180);
	double startX;
	double startY;
	double endX;
	double endY;

	for(std::size_t i=0; i < segments.size(); i++) {
		startX = newPath.segments[i].start.x;
		startY = newPath.segments[i].start.y;
		endX = newPath.segments[i].end.x;
		endY = newPath.segments[i].end.y;

		newPath.segments[i].start.x = ((startX-origin.x)*cosTetha - (startY-origin.y)*sinTetha) + origin.x;

		newPath.segments[i].start.y = ((startX-origin.x)*sinTetha + (startY-origin.y)*cosTetha) + origin.y;
		newPath.segments[i].end.x = ((endX-origin.x)*cosTetha - (endY-origin.y)*sinTetha) + origin.x;
		newPath.segments[i].end.y = ((endX-origin.x)*sinTetha + (endY-origin.y)*cosTetha) + origin.y;
	}
	return newPath;
}
