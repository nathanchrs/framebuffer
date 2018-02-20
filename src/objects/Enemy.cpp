#include "Enemy.h"
#include "../graphics/Point.h"
#include "../common/easing.h"

VectorSprite Enemy::planeSprite;
VectorSprite Enemy::propSprite;
VectorSprite Enemy::wheelSprite;
VectorSprite Enemy::parachuteSprite;

Enemy::Enemy(long elapsedMillis, Point<double> position) : Renderable(elapsedMillis, position) {
  scale = 1;
  lastChange = -500;
  direction = 1;
}

void Enemy::zoomIn() {
  scale *= 1.2;
}

void Enemy::zoomOut() {
  if (scale > 0) {
    scale /= 1.2;
  }
}

void Enemy::update(long elapsedMillis) {
	//scale = easeInQuintic(0.02, 10, getAge(elapsedMillis), 10000);
	parachuteDrop += 1;
	if (getAge(elapsedMillis) > 5000) {
		wheelDrop = easeInQuadratic(0, 100, getAge(elapsedMillis) - 5000, 5000) * 6;
	}
	propRotation += 15;
	
	if (elapsedMillis - lastChange > 1000) {
	  lastChange = elapsedMillis;
	  direction *= -1;
	}
	position.x += direction * 2 * scale;
}

void Enemy::render(FrameBuffer& fb) {
	fb.drawVectorSprite(Point<double>(300, -400), Enemy::parachuteSprite
		.translate(Point<double>(0, parachuteDrop))
	);

	fb.drawVectorSprite(position, Enemy::planeSprite.scale(scale));

	fb.drawVectorSprite(position, Enemy::propSprite.scale(scale)
		.rotate(propRotation)
		.translate(Point<double>(scale * 130, scale * 6))
	);
	fb.drawVectorSprite(position, Enemy::propSprite.scale(scale)
		.rotate(propRotation)
		.translate(Point<double>(-scale * 130, scale * 6))
	);

	fb.drawVectorSprite(position, Enemy::wheelSprite.scale(scale)
		.translate(Point<double>(0, scale * wheelDrop))
	);
	fb.drawVectorSprite(position, Enemy::wheelSprite.scale(scale)
		.mirrorHorizontal()
		.translate(Point<double>(0, scale * wheelDrop))
	);
}
