#include <math.h>
#include <iostream>
#include "Crosshair.h"

Crosshair::Crosshair(long elapsedMillis, Point<double> topLeftClip, Point<double> bottomRightClip) : Renderable(elapsedMillis), cursor(topLeftClip, bottomRightClip) {
  cursor.setPosition(Point<double>((topLeftClip.x + bottomRightClip.x)/2, (topLeftClip.y + bottomRightClip.y)/2));
}

void Crosshair::update(long elapsedMillis) {
  prevPosition = position;
  position = cursor.getPosition();
  prevLeftClicked = currentLeftClicked;
  currentLeftClicked = cursor.getLeftClick();
  prevRightClicked = currentRightClicked;
  currentRightClicked = cursor.getRightClick();
  
  if (currentLeftClicked && !prevLeftClicked) {
    lastDown = elapsedMillis;
    lastDownPosition.x = position.x;
    lastDownPosition.y = position.y;
  }
  else if (currentRightClicked && !prevRightClicked) {
    lastDown = elapsedMillis;
    lastDownPosition.x = position.x;
    lastDownPosition.y = position.y;
  }
  
  Point<double> dPos = position - lastDownPosition;
  if ((elapsedMillis - lastDown < treshold) && (abs(dPos.x) < tresholdPosition && abs(dPos.y) < tresholdPosition) && !currentLeftClicked && prevLeftClicked) {
    leftClicked = true;
  }
  else {
    leftClicked = false;
  }
  
  if (((abs(dPos.x) >= tresholdPosition || abs(dPos.y) >= tresholdPosition)) && currentLeftClicked && prevLeftClicked) {
    leftHold = true;
  }
  else {
    leftHold = false;
  }
  
  if ((elapsedMillis - lastDown < treshold) && (abs(dPos.x) < tresholdPosition && abs(dPos.y) < tresholdPosition) && !currentRightClicked && prevRightClicked) {
    rightClicked = true;
  }
  else {
    rightClicked = false;
  }
  
  if (((abs(dPos.x) >= tresholdPosition || abs(dPos.y) >= tresholdPosition)) && currentRightClicked && prevRightClicked) {
    rightHold = true;
  }
  else {
    rightHold = false;
  }
}

void Crosshair::render(FrameBuffer &fb) {
  if (source != NULL) {
    fb.drawVectorSprite(position, *source);
  }
}

bool Crosshair::isLeftClicked() {
  return leftClicked;
}

bool Crosshair::isLeftHold() {
  return leftHold;
}

bool Crosshair::isRightClicked() {
  return rightClicked;
}

bool Crosshair::isRightHold() {
  return rightHold;
}

Point<double> Crosshair::diffPosition() {
  return position - prevPosition;
}

/*bool Crosshair::isMouseDown() {
  
}

bool Crosshair::isMouseUp() {
  
}*/

