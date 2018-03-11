#include "Crosshair.h"

Crosshair::Crosshair(long elapsedMillis, Point<double> topLeftClip, Point<double> bottomRightClip) : Renderable(elapsedMillis), cursor(topLeftClip, bottomRightClip) {
  cursor.setPosition(Point<double>((topLeftClip.x + bottomRightClip.x)/2, (topLeftClip.y + bottomRightClip.y)/2));
}

void Crosshair::update(long elapsedMillis) {
  position = cursor.getPosition();
  prevMouseClicked = currentMouseClicked;
  currentMouseClicked = cursor.getLeftClickDown();
  if (currentMouseClicked && !prevMouseClicked) {
    clicked = true;
  }
}

void Crosshair::render(FrameBuffer &fb) {
  if (source != NULL) {
    fb.drawVectorSprite(position, *source);
  }
}

bool Crosshair::isClicked() {
  if (clicked) {
    clicked = false;
    return true;
  }
  else return false;
}
