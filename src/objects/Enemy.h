#ifndef ENEMY_H
#define ENEMY_H

#include "Renderable.h"
#include "../graphics/VectorSprite.h"

class Enemy : public Renderable {
public:
	Enemy(long elapsedMillis, Point<double> position) : Renderable(elapsedMillis, position) {}
	virtual void update(long elapsedMillis);
	virtual void render(FrameBuffer& fb);

	static VectorSprite sprite;
};

#endif
