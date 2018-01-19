#define FPS 60

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "easing.h"
#include "framebuffer.h"

#define TOTAL_DURATION 10000

FrameBuffer fb;


void drawPulse(long elapsedMillis, Point center, double radius, Color color) {
	long duration = 700;
	long time = elapsedMillis % duration;
	double animRadius = easeOutQuadratic(time, 0, radius, duration);

	long colorTime = time > duration * 0.2 ? (time - duration*0.2) * 1.2 : 0;
	Color animColor = c_make(
		easeOutQuadratic(colorTime, color.r, 0, duration),
		easeOutQuadratic(colorTime, color.g, 0, duration),
		easeOutQuadratic(colorTime, color.b, 0, duration)
	);
	fb_drawCircleOutline(&fb, center, animRadius, 1.0, animColor);
}

void render(long elapsedMillis) {
	// Clear screen from previous frame
	fb_clear(&fb, COLOR_BLACK);

	// Draw
	
	long width = fb_getWidth(&fb);
	long height = fb_getHeight(&fb);

	// Pulse effect
	int i, j;
	for (i = 0; i < width; i += 60) {
		for (j = 0; j < height; j += 60) {
			drawPulse(elapsedMillis - sqrt((i-width/2)*(i-width/2) + (j-height/2)*(j-height/2)) - 40, p_make(i, j), 20, c_make(0x01, 0x22, 0x55));
			drawPulse(elapsedMillis - sqrt((i-width/2)*(i-width/2) + (j-height/2)*(j-height/2)) + 40, p_make(i, j), 20, c_make(0x01, 0x22, 0x55));
			drawPulse(elapsedMillis - sqrt((i-width/2)*(i-width/2) + (j-height/2)*(j-height/2)), p_make(i, j), 20, c_make(0x05, 0xcb, 0xff));
		}
	}

	// Progress rectangle
	fb_drawRectangle(&fb, p_make(0, 0), p_make(elapsedMillis * width / TOTAL_DURATION, 10), c_make(0xff, 0xff, 0xff));
 
	// Output drawn pixels to screen
	fb_output(&fb);
}

int main() {

	// Initialize
	fb_init(&fb, "/dev/fb0");

	// Main loop
	long nanoSecondsPerFrame = 1000000 / FPS;
	long elapsedMillis = 0;
	unsigned char isRunning = 1;
	while (isRunning) {
		render(elapsedMillis);
		usleep(nanoSecondsPerFrame);
		elapsedMillis += nanoSecondsPerFrame / 1000;
		if (elapsedMillis > TOTAL_DURATION) isRunning = 0;
	}

	// Clean up
	fb_clear(&fb, COLOR_BLACK);
	fb_output(&fb);
	return 0;
}
