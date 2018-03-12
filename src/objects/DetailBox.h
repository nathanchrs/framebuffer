#ifndef DETAILBOX_H
#define DETAILBOX_H

#include "../common/Cursor.h"
#include "../graphics/Point.h"
#include "Renderable.h"
#include "View.h"

class DetailBox : public Renderable {
public:
  DetailBox(long elapsedMillis, View *detailView, View *sourceView, Color color);
  virtual void update(long elapsedMillis);
  virtual void render(FrameBuffer &fb);

  Color color;
  View *detailView;
  View *sourceView;
};

#endif
