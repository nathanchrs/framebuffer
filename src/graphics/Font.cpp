#include "Font.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

Font::Font(const char* fontFilePath, double scale) {
    std::ifstream fin;
    fin.open(fontFilePath);

    std::string character;
    char ignoreCharacter;

    int characterCount;
    fin >> characterCount;
    for (int i = 0; i < characterCount; i++) {
        std::vector<PathSegment> segments;
        double dx, dy;
        double accX = 0.0;
        double accY = 0.0;
        long characterWidth = 0;

        // The character to be read and the number of paths it has
        int closedPathsCount;
        fin >> character >> closedPathsCount;
        fin.ignore();

        for (int j = 0; j < closedPathsCount; j++) {
            bool first = true;
            std::string line;
            std::getline(fin, line);
            std::istringstream closedPath(line);

            std::cout << "line: " << line << std::endl;

            Point startingPoint;
            while (closedPath >> dx >> ignoreCharacter >> dy) {
                dx *= scale;
                dy *= scale;
                if (first) {
                    startingPoint = Point(round(accX + dx), round(accY + dy));
                    first = false;
                } else {
                    segments.push_back(PathSegment(
                        Point(round(accX), round(accY)),
                        Point(round(accX + dx), round(accY + dy))
                    ));
                }
                accX += dx;
                accY += dy;
                if (round(accX) > characterWidth) characterWidth = round(accX);
                std::cout << "move: " << accX + dx << "," << accY + dy << std::endl;
            }
            segments.push_back(PathSegment(
                Point(round(accX), round(accY)),
                startingPoint
            ));
            std::cout << "close to: " << startingPoint.x << "," << startingPoint.y << std::endl;
            std::cout << "end poly" << std::endl;
        }

        // TODO: format validation
        std::cout << "read char: " << character[0] << std::endl;
        characterPaths[character[0]] = segments;
        characterWidths[character[0]] = characterWidth + 1;
    }

    fin.close();
}

std::vector<PathSegment> Font::getCharacterPath(char character) {
    if (characterPaths.find(character) != characterPaths.end()) {
        return characterPaths[character];
    } else {
        std::vector<PathSegment> emptySegment;
        return emptySegment;
    }
}

long Font::getCharacterWidth(char character) {
    double SPACE_WIDTH = 16; // TODO: scale this too, make scale a method parameter
    if (characterWidths.find(character) != characterWidths.end()) {
        return characterWidths[character];
    } else {
        return (long) (SPACE_WIDTH);
    }
}
