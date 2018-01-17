#define FPS 10

#include <unistd.h>
#include <stdio.h>
#include "framebuffer.h"

FrameBuffer fb;

void render(long frameIndex) {
	fb_clear(&fb, COLOR_BLACK);
	fb_drawRectangle(&fb, p_make(20, 20), p_make(20 + frameIndex, 30), c_make(0xff, 0xff, 0xff));
	fb_switchBuffer(&fb);
}

int main() {

	// Initialize
	fb_init(&fb, "/dev/fb0");

	fb_printInfo(&fb);

	/*
	fb_clear(&fb, COLOR_BLACK);
	fb_switchBuffer(&fb);

	// Main loop
	long nanoSecondsPerFrame = 1000000 / FPS;
	long frameIndex = 0;
	unsigned char isRunning = 1;
	while (isRunning) {
		render(frameIndex);

		usleep(nanoSecondsPerFrame);
		frameIndex++;

		if (frameIndex > 600) isRunning = 0;
	}

	// Clean up
	fb_clear(&fb, COLOR_BLACK);
	fb_switchBuffer(&fb);
	return 0;*/
}
