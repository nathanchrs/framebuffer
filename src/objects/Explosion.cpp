#include "Explosion.h"
#include "../common/easing.h"

void Explosion::update(long elapsedMillis) {
	long duration = 700;
	long age = getAge(elapsedMillis);
	long animAge = age > duration ? duration : age;
	animRadius = easeOutQuadratic(0, radius, animAge, duration);
	if (age >= 1200) die();
}

void Explosion::render(FrameBuffer &fb) {
	Color color = Color(0xed, 0x53, 0x00); // orange
	fb.drawCircleOutline(position, animRadius, 1.0, color);
}
