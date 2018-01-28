#ifndef ENEMY_H
#define ENEMY_H

#include "Renderable.h"

class Enemy : public Renderable {
public:
	Enemy(long elapsedMillis, Point position) : Renderable(elapsedMillis, position) {}
	virtual void render(FrameBuffer& fb, long elapsedMillis);
};

#endif
