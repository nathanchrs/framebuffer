#include "View.h"

void View::render(FrameBuffer &fb) {
    if (source != NULL) {
        Point<double> scalingFactor(size.x/sourceSize.x, size.y/sourceSize.y);
        VectorSprite sourceImage = (*source).translate(Point<double>() - sourcePosition).scale(scalingFactor);
            fb.drawVectorSpriteClip(
                position,
                sourceImage,
                position,
                position + size
            );
    }
}
