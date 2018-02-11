#include "Path.h"
#include <cmath>
#include <fstream>
#include <iostream>

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
