#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../graphics/Point.h"
#include "../graphics/FrameBuffer.h"

class Renderable {
public:
	Renderable(long elapsedMillis, Point position) : createdAt(elapsedMillis), prevElapsedMillis(elapsedMillis), position(position) {}
	virtual ~Renderable() = 0;

	virtual void render(FrameBuffer &fb, long elapsedMillis) = 0;

	long getAge(long elapsedMillis) { return elapsedMillis - createdAt; }

	long getDeltaTime(long elapsedMillis) {
		long deltaTime = elapsedMillis - prevElapsedMillis;
		prevElapsedMillis = elapsedMillis;
		return deltaTime;
	}

	long createdAt;
	long prevElapsedMillis;
	Point position;
};

#endif
