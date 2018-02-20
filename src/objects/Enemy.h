#ifndef ENEMY_H
#define ENEMY_H

#include "Renderable.h"
#include "../graphics/VectorSprite.h"

class Enemy : public Renderable {
public:
	Enemy(long elapsedMillis, Point<double> position);
	virtual void update(long elapsedMillis);
	virtual void render(FrameBuffer& fb);
	void zoomIn();
	void zoomOut();

  char direction;
  long lastChange;
	double propRotation;
	double wheelDrop;
	double parachuteScale;
	double parachuteDrop;
	static VectorSprite planeSprite;
	static VectorSprite propSprite;
	static VectorSprite wheelSprite;
	static VectorSprite parachuteSprite;
};

#endif
