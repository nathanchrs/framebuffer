#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Renderable.h"

class Explosion : public Renderable {
public:
	Explosion(long elapsedMillis, Point position) : Renderable(elapsedMillis, position) {}
	virtual void update(long elapsedMillis);
	virtual void render(FrameBuffer &fb);

protected:
	long radius = 50;
	long animRadius = 0;
};

#endif
