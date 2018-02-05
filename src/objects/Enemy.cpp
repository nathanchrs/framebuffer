#include "Enemy.h"
#include "../common/easing.h"

void Enemy::update(long elapsedMillis) {
	position.x = getAge(elapsedMillis) / 4;
}

void Enemy::render(FrameBuffer& fb) {
	long radius = 20;
	fb.drawCircleOutline(position, radius, 2.0, Color(0xee, 0x0a, 0x0a));
}
