#ifndef FRAME_H
#define FRAME_H

#include "../graphics/FrameBuffer.h"
#include "../graphics/Point.h"
#include "Renderable.h"

class Frame : public Renderable {
public:
  Frame(long elapsedMillis, FrameBuffer &fb);
  virtual void update(long elapsedMillis);
  virtual void render(FrameBuffer &fb);

  VectorSprite source;

  long thickness = 2;
  long navbarHeight = 80;
  long sidebarWidth = 300;
  long bottomSidebarHeight = 300;
};

#endif
