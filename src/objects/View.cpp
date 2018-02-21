#include "View.h"

void View::render(FrameBuffer &fb) {
    if (source != NULL) {
        VectorSprite sourceImage = (*source);
        fb.drawVectorSprite(position, sourceImage);
    }
}
