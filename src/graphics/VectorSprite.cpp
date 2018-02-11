#include "VectorSprite.h"

#include <fstream>
#include <sstream>
#include <iostream>

VectorSprite::VectorSprite(std::string filePath) {
    std::ifstream fin;
    fin.open(filePath);

    std::string pathName;
    char ignoreCharacter;

    int pathCount;
    fin >> pathCount;
    for (int i = 0; i < pathCount; i++) {
        std::vector<PathSegment<double> > segments;
        double dx, dy;
        double accX = 0.0;
        double accY = 0.0;
        unsigned int fillR, fillG, fillB, strokeR, strokeG, strokeB;

        int closedPathsCount;
        fin >> pathName >> closedPathsCount;
        fin >> std::hex >> fillR >> fillG >> fillB;
        fin >> std::hex >> strokeR >> strokeG >> strokeB;
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
            segments.push_back(PathSegment<double>(
                Point<double>(accX, accY),
                startingPoint
            ));
        }

        paths.push_back(Path(segments));
        fillColors.push_back(Color(fillR, fillG, fillB));
        strokeColors.push_back(Color(strokeR, strokeG, strokeB));
    }

    fin.close();
}
