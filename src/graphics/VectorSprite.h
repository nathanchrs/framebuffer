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

    VectorSprite scale(Point<double> scalingFactor, Point<double> origin) const;
    VectorSprite scale(double scalingFactor, Point<double> origin) const { return scale(Point<double>(scalingFactor, scalingFactor), origin); }
    VectorSprite scale(Point<double> scalingFactor) const { return scale(scalingFactor, Point<double>()); }
    VectorSprite scale(double scalingFactor) const { return scale(scalingFactor, Point<double>()); }

    VectorSprite translate(Point<double> delta) const;

    VectorSprite rotate(double rotationAngle, Point<double> origin) const;
    VectorSprite rotate(double rotationAngle) const { return rotate(rotationAngle, Point<double>()); }

    VectorSprite mirrorHorizontal(double mirrorX) const;
    VectorSprite mirrorHorizontal() const {return mirrorHorizontal(0); }
    VectorSprite mirrorVertical(double mirrorY) const;
    VectorSprite mirrorVertical() const {return mirrorVertical(0); }

    std::vector<Path> paths;
    std::vector<Color> fillColors;
    std::vector<Color> strokeColors;
};

#endif
