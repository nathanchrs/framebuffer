#include "Enemy.h"
#include "../graphics/Point.h"
#include "../common/easing.h"

VectorSprite Enemy::planeSprite;
VectorSprite Enemy::propSprite;
VectorSprite Enemy::wheelSprite;

void Enemy::update(long elapsedMillis) {
	scale = easeInQuintic(0.02, 10, getAge(elapsedMillis), 10000);
	if (getAge(elapsedMillis) > 5000) {
		wheelDrop = easeInQuadratic(0, 100, getAge(elapsedMillis) - 5000, 5000) * 6;
	}
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

	fb.drawVectorSprite(position, Enemy::wheelSprite.scale(scale)
		.translate(Point<double>(0, scale * wheelDrop))
	);
	fb.drawVectorSprite(position, Enemy::wheelSprite.scale(scale)
		.mirrorHorizontal()
		.translate(Point<double>(0, scale * wheelDrop))
	);
}
