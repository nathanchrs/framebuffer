#ifndef PULSE_H
#define PULSE_H

#include "Renderable.h"

class Pulse : public Renderable {
public:
	Pulse(long elapsedMillis, Point position) : Renderable(elapsedMillis, position) {}
	virtual void render(FrameBuffer& fb, long elapsedMillis);
};

#endif
