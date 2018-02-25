#ifndef VIEW_H
#define VIEW_H

#include "../graphics/Point.h"
#include "../graphics/VectorSprite.h"
#include "Renderable.h"

class View : public Renderable {
public:
  View(long elapsedMillis) : Renderable(elapsedMillis) {}
  virtual void update(long elapsedMillis) {}
  virtual void render(FrameBuffer &fb);
  void zoomIn(double scaleFactor);
  void zoomOut(double scaleFactor);
  void updateDetailBox();

  Point<double> position;
  Point<double> size;
  VectorSprite *source;
  Point<double> sourcePosition;
  Point<double> sourceSize;
  Path detailBox;
};

#endif
