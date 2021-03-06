#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../graphics/Point.h"
#include "../graphics/FrameBuffer.h"

class Renderable {
public:
	Renderable(long elapsedMillis) : createdAt(elapsedMillis), prevElapsedMillis(elapsedMillis), isAlive(true) {}
	virtual ~Renderable() = 0;

	virtual void update(long elapsedMillis) = 0;
	virtual void render(FrameBuffer &fb) = 0;
	virtual void die() {
		isAlive = false;	
	};

	long getAge(long elapsedMillis) { return elapsedMillis - createdAt; }

	long getDeltaTime(long elapsedMillis) {
		long deltaTime = elapsedMillis - prevElapsedMillis;
		prevElapsedMillis = elapsedMillis;
		return deltaTime;
	}

	long createdAt;
	long prevElapsedMillis;
	bool isAlive;
};

#endif
