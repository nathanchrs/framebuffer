#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Renderable.h"

class Explosion : public Renderable {
public:
	Explosion(long elapsedMillis, Point<double> position) : Renderable(elapsedMillis, position) {}
	virtual void update(long elapsedMillis);
	virtual void render(FrameBuffer &fb);

protected:
	double radius = 50;
	double animRadius = 0;
};

#endif
