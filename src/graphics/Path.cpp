#include "Path.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdlib.h>

Path::Path(std::string filePath) {
    std::ifstream fin;
    fin.open(filePath);

    std::string character;
    char ignoreCharacter;

    double dx, dy;
    double accX = 0.0;
    double accY = 0.0;

    // The character to be read and the number of paths it has
    int closedPathsCount;
    fin >> closedPathsCount;
    fin.ignore();

    for (int j = 0; j < closedPathsCount; j++) {
        bool first = true;
        std::string line;
        std::getline(fin, line);
        std::istringstream closedPath(line);

        Point<double> startingPoint;
        while (closedPath >> dx >> ignoreCharacter >> dy) {
            if (first) {
                startingPoint = Point<double>(accX + dx, accY + dy);
                first = false;
            } else {
                segments.push_back(PathSegment<double>(
                    Point<double>(accX, accY),
                    Point<double>(accX + dx, accY + dy)
                ));
            }
            accX += dx;
            accY += dy;
        }
        /*segments.push_back(PathSegment<double>(
            Point<double>(accX, accY),
            startingPoint
        ));*/
    }
    std::cout << "File " << filePath << std::endl;
    for (size_t i = 0; i < segments.size(); i++) {
      std::cout << "line (" << segments[i].start.x << "," << segments[i].start.y << ") (" << segments[i].end.x << "," << segments[i].end.y << ")" << std::endl;
    }
    std::cout << std::endl;
    
    fin.close();
}

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

PathSegment<double> Path::getPredSegment(size_t index) {
    if (index > 0 && std::abs(segments[index-1].end.x - segments[index].start.x) < 0.0001
            && std::abs(segments[index-1].end.y - segments[index].start.y) < 0.0001) {
        return segments[index-1];
    }
    else {
        for (size_t i = 0; i < segments.size(); i++) {
            if (std::abs(segments[i].end.x - segments[index].start.x) < 0.0001
                    && std::abs(segments[i].end.y - segments[index].start.y) < 0.0001) {
                return segments[i];
            }
        }
    }
    return PathSegment<double>(Point<double>(0.0, 0.0), Point<double>(0.0, 0.0));
}

PathSegment<double> Path::getSuccSegment(size_t index) {
    if (index < segments.size() && std::abs(segments[index].end.x - segments[index+1].start.x) < 0.0001
            && std::abs(segments[index].end.y - segments[index+1].start.y) < 0.0001) {
        return segments[index+1];
    }
    else {
        for (size_t i = 0; i < segments.size(); i++) {
            if (std::abs(segments[index].end.x - segments[i].start.x) < 0.0001
                    && std::abs(segments[index].end.y - segments[i].start.y) < 0.0001) {
                return segments[i];
            }
        }
    }
    return PathSegment<double>(Point<double>(0.0, 0.0), Point<double>(0.0, 0.0));
    
}
