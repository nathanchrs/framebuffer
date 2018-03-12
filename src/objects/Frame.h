#ifndef FRAME_H
#define FRAME_H

#include "../graphics/FrameBuffer.h"
#include "../graphics/Point.h"
#include "Renderable.h"

class Frame : public Renderable {
public:
  Frame(long elapsedMillis, FrameBuffer &fb);
  virtual void update(long elapsedMillis) {}
  virtual void render(FrameBuffer &fb);

  VectorSprite source;
};

#endif
