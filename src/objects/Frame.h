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
  Point<double> getBottomNavStart() { return Point<double>(thickness+1, height-thickness-bottomSidebarHeight); }
  Point<double> getBottomNavSize() { return Point<double>(sidebarWidth-2, bottomSidebarHeight-2); }
  Point<double> getContentStart() { return Point<double>(2*thickness+sidebarWidth+1, 2*thickness+navbarHeight+1); }
  Point<double> getContentSize() { return Point<double>(width-3*thickness-sidebarWidth-3, height-3*thickness-navbarHeight-3); }

  VectorSprite source;

  long thickness = 2;
  long navbarHeight = 80;
  long sidebarWidth = 300;
  long bottomSidebarHeight = 300;
  long width;
  long height;
};

#endif
