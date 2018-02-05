#ifndef FONT_H
#define FONT_H

#include <map>
#include "Path.h"
#include "PathSegment.h"

class Font {
public:
	Font(std::string fontFilePath);

  	Path getCharacterPath(char character, double size) const;
	double getCharacterWidth(char character, double size) const;
	Path getTextPath(const std::string &text, double size) const;

	double letterSpacing;
	double wordSpacing;

private:
	std::map<char, Path> characterPaths;
	std::map<char, double> characterWidths;
};

#endif
