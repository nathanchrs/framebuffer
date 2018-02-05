#include "Font.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

// TODO: format validation
Font::Font(std::string fontFilePath) {
    std::ifstream fin;
    fin.open(fontFilePath);

    std::string character;
    char ignoreCharacter;

    int characterCount;
    fin >> characterCount;
    for (int i = 0; i < characterCount; i++) {
        std::vector<PathSegment<double> > segments;
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
                if (accX > characterWidth) characterWidth = accX;
                std::cout << "move: " << accX + dx << "," << accY + dy << std::endl;
            }
            segments.push_back(PathSegment<double>(
                Point<double>(accX, accY),
                startingPoint
            ));
            std::cout << "close to: " << startingPoint.x << "," << startingPoint.y << std::endl;
            std::cout << "end poly" << std::endl;
        }

        std::cout << "read char: " << character[0] << std::endl;
        characterPaths[character[0]] = Path(segments);
        characterWidths[character[0]] = characterWidth;
    }

    fin.close();

    // Set default font parameters
    letterSpacing = 6.0;
    wordSpacing = 16.0;
}

Path Font::getCharacterPath(char character, double size) const {
    if (characterPaths.find(character) != characterPaths.end()) {
        return characterPaths.at(character).scale(size);
    } else {
        Path emptyPath;
        return emptyPath;
    }
}

double Font::getCharacterWidth(char character, double size) const {
    if (characterWidths.find(character) != characterWidths.end()) {
        return characterWidths.at(character) * size;
    } else {
        return wordSpacing * size;
    }
}

Path Font::getTextPath(const std::string &text, double size) const {
    Path textPath;
	double characterX = 0.0;
	for (size_t i = 0; i < text.length(); i++) {
        Path characterPath = getCharacterPath(text[i], size).translate(Point<double>(characterX, 0.0));
        textPath.segments.reserve(textPath.segments.size() + characterPath.segments.size());
        textPath.segments.insert(textPath.segments.end(), characterPath.segments.begin(), characterPath.segments.end());
        characterX += getCharacterWidth(text[i], size) + (letterSpacing * size);
	}
    return textPath;
}
