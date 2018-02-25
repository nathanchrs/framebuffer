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

void View::zoomIn(double scaleFactor) {
    double dx = sourceSize.x;
    sourceSize.x /= scaleFactor;
    dx -= sourceSize.x;
    sourcePosition.x += dx/2;
    double dy = sourceSize.y;
    sourceSize.y /= scaleFactor;
    dy -= sourceSize.y;
    sourcePosition.y += dy/2;
    updateDetailBox();
}

void View::zoomOut(double scaleFactor) {
    double dx = sourceSize.x;
    sourceSize.x *= scaleFactor;
    dx -= sourceSize.x;
    sourcePosition.x += dx/2;
    double dy = sourceSize.y;
    sourceSize.y *= scaleFactor;
    dy -= sourceSize.y;
    sourcePosition.y += dy/2;
    updateDetailBox();
}

void View::updateDetailBox() {
	detailBox.segments[0].end.x = sourceSize.x;
	detailBox.segments[1].start.x = sourceSize.x;
	detailBox.segments[1].end.x = sourceSize.x;
	detailBox.segments[1].end.y = sourceSize.y;
	detailBox.segments[2].start.x = sourceSize.x;
	detailBox.segments[2].start.y = sourceSize.y;
	detailBox.segments[2].end.y = sourceSize.y;
	detailBox.segments[3].start.y = sourceSize.y;
}
