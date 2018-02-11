#ifndef VECTOR_SPRITE_H
#define VECTOR_SPRITE_H

#include <vector>
#include <string>
#include "Path.h"
#include "Color.h"

class VectorSprite {
public:
    VectorSprite() {}
    VectorSprite(std::string filePath);

    std::vector<Path> paths;
    std::vector<Color> fillColors;
    std::vector<Color> strokeColors;
};

#endif
