#include "Enemy.h"
#include "../common/easing.h"

VectorSprite Enemy::sprite;

void Enemy::update(long elapsedMillis) {
	position.x = getAge(elapsedMillis) / 4;
}

void Enemy::render(FrameBuffer& fb) {
	fb.drawVectorSprite(position, Enemy::sprite);
}
