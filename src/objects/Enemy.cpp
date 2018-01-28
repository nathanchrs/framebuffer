#include "Enemy.h"
#include "../common/easing.h"
#include <iostream>

void Enemy::render(FrameBuffer& fb, long elapsedMillis) {
	long radius = 20;
    position.x = getAge(elapsedMillis) / 4;

	fb.drawCircleOutline(position, radius, 2.0, Color(0xee, 0x0a, 0x0a));
}
