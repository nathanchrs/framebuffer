#include <math.h>
#include <iostream>
#include "Crosshair.h"

Crosshair::Crosshair(long elapsedMillis, Point<double> topLeftClip, Point<double> bottomRightClip) : Renderable(elapsedMillis), cursor(topLeftClip, bottomRightClip) {
  cursor.setPosition(Point<double>((topLeftClip.x + bottomRightClip.x)/2, (topLeftClip.y + bottomRightClip.y)/2));
}

void Crosshair::update(long elapsedMillis) {
  prevPosition = position;
  position = cursor.getPosition();
  prevMouseClicked = currentMouseClicked;
  currentMouseClicked = cursor.getLeftClick();
  
  if (currentMouseClicked && !prevMouseClicked) {
    lastDown = elapsedMillis;
    lastDownPosition.x = position.x;
    lastDownPosition.y = position.y;
  }
  else if (!currentMouseClicked && prevMouseClicked) {
    lastUp = elapsedMillis;
  }
  
  Point<double> dPos = position - lastDownPosition;
  if ((elapsedMillis - lastDown < treshold) && (abs(dPos.x) < tresholdPosition && abs(dPos.y) < tresholdPosition) && !currentMouseClicked && prevMouseClicked) {
    clicked = true;
  }
  else {
    clicked = false;
  }
  
  if (((abs(dPos.x) >= tresholdPosition || abs(dPos.y) >= tresholdPosition)) && currentMouseClicked && prevMouseClicked) {
    hold = true;
  }
  else {
    hold = false;
  }
}

void Crosshair::render(FrameBuffer &fb) {
  if (source != NULL) {
    fb.drawVectorSprite(position, *source);
  }
}

bool Crosshair::isClicked() {
  return clicked;
}

bool Crosshair::isHold() {
  return hold;
}

Point<double> Crosshair::diffPosition() {
  return position - prevPosition;
}

/*bool Crosshair::isMouseDown() {
  
}

bool Crosshair::isMouseUp() {
  
}*/

