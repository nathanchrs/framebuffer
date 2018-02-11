#include "Enemy.h"
#include "../graphics/Point.h"
#include "../common/easing.h"

VectorSprite Enemy::planeSprite;
VectorSprite Enemy::propSprite;

void Enemy::update(long elapsedMillis) {
	scale = easeInQuartic(0.1, 10, getAge(elapsedMillis), 10000);
	propRotation += 15;
}

void Enemy::render(FrameBuffer& fb) {
	fb.drawVectorSprite(position, Enemy::planeSprite.scale(scale));
	fb.drawVectorSprite(position, Enemy::propSprite.scale(scale)
		.rotate(propRotation)
		.translate(Point<double>(scale * 130, scale * 6))
	);
	fb.drawVectorSprite(position, Enemy::propSprite.scale(scale)
		.rotate(propRotation)
		.translate(Point<double>(-scale * 130, scale * 6))
	);
}
