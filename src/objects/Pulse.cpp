#include "Pulse.h"
#include "../common/easing.h"

void Pulse::render(FrameBuffer& fb, long elapsedMillis) {

	long radius = 20;
	Color color = Color(0x05, 0xcb, 0xff);

	long duration = 700;
	long time = getAge(elapsedMillis) % duration;
	double animRadius = easeOutQuadratic(0, radius, time, duration);

	long colorTime = time > duration * 0.2 ? (time - duration*0.2) * 1.2 : 0;
	Color animColor = Color(
		easeOutQuadratic(color.r, 0, colorTime, duration),
		easeOutQuadratic(color.g, 0, colorTime, duration),
		easeOutQuadratic(color.b, 0, colorTime, duration)
	);
	fb.drawCircleOutline(position, animRadius, 1.0, animColor);
}
