#ifndef FONT_H
#define FONT_H

#include <vector>
#include <map>
#include "Point.h"
#include "PathSegment.h"

class Font {
public:
	Font(const char* fontFilePath, double scale);

  	std::vector<PathSegment> getCharacterPath(char character);
	long getCharacterWidth(char character);

  private:
	std::map<char, std::vector<PathSegment> > characterPaths;
	std::map<char, long> characterWidths;
};

#endif
