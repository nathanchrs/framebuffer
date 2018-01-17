#define FPS 60

#include <unistd.h>
#include <stdio.h>
#include "framebuffer.h"

FrameBuffer fb;

void render(long elapsedMillis) {
	// Clear screen from previous frame
	fb_clear(&fb, COLOR_BLACK);

	// Draw

	fb_drawRectangle(&fb, p_make(0, 0), p_make(elapsedMillis * fb_getWidth(&fb) / 10000, 10), c_make(0xff, 0xff, 0xff));

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
		if (elapsedMillis > 10000) isRunning = 0;
	}

	// Clean up
	fb_clear(&fb, COLOR_BLACK);
	fb_output(&fb);
	return 0;
}
