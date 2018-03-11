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

  Cursor cursor;
  Point<double> position;
  VectorSprite *source;
private:
  bool prevMouseClicked = false;
  bool currentMouseClicked = false;
  bool clicked = false;
};

#endif
