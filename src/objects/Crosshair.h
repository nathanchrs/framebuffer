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
  bool isClicked();
  bool isHold();
  Point<double> diffPosition();
  /*bool isMouseDown();
  bool isMouseUp();*/

  Cursor cursor;
  Point<double> position;
  VectorSprite *source;
private:
  bool prevMouseClicked = false;
  bool currentMouseClicked = false;
  bool clicked = false;
  bool hold = false;
  long lastDown = 0;
  long lastUp = 0;
  long treshold = 40;
  double tresholdPosition = 7.0;
  Point<double> lastDownPosition;
  Point<double> prevPosition;
};

#endif
