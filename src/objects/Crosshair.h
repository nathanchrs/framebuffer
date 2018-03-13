#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "../common/Cursor.h"
#include "../graphics/Point.h"
#include "Renderable.h"

class Crosshair : public Renderable {
public:
  Crosshair(long elapsedMillis, Point<double> topLeftClip, Point<double> bottomRightClip);
  virtual void update(long elapsedMillis);
  virtual void render(FrameBuffer &fb);
  bool isLeftClicked();
  bool isLeftHold();
  bool isRightClicked();
  bool isRightHold();
  Point<double> diffPosition();
  /*bool isMouseDown();
  bool isMouseUp();*/

  Cursor cursor;
  Point<double> position;
  Point<double> lastDownPosition;
  Point<double> prevPosition;
  VectorSprite *source;
private:
  bool prevLeftClicked = false;
  bool currentLeftClicked = false;
  bool leftClicked = false;
  bool leftHold = false;
  bool prevRightClicked = false;
  bool currentRightClicked = false;
  bool rightClicked = false;
  bool rightHold = false;
  long lastDown = 0;
  long treshold = 40;
  double tresholdPosition = 7.0;
};

#endif
