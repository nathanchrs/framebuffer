#define FPS 60

#include <unistd.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "graphics/FrameBuffer.h"
#include "graphics/Font.h"
#include "objects/Renderable.h"
#include "objects/Enemy.h"

#define TOTAL_DURATION 10000

int main() {

	/* INITIALIZATION */

	FrameBuffer fb = FrameBuffer("/dev/fb0");
	std::vector<Renderable*> objects;
	std::vector<Enemy*> enemies;

	Font font = Font("a.txt", 0.2);

	long enemySpawnInterval = 200;

	/* MAIN LOOP */

	long millisPerFrame = 1000 / FPS;
	long elapsedMillis = 0;
	unsigned char isRunning = 1;
	while (isRunning) {

		// TODO: read input
		
		/* UPDATE */

		// Spawn enemies
		if ((elapsedMillis % enemySpawnInterval) >= 0 && (elapsedMillis % enemySpawnInterval) < millisPerFrame) {
			Enemy *newEnemy = new Enemy(elapsedMillis, Point(0, 100));
			objects.push_back(newEnemy);
			enemies.push_back(newEnemy);
		}

		// Destroy enemies which has moved offscreen
		for (int i = enemies.size() - 1; i >= 0; i--) {
			if (enemies[i]->position.x > fb.getWidth() + 20) {
				objects.erase(std::remove(objects.begin(), objects.end(), enemies[i]), objects.end());
				delete enemies[i];
				enemies.erase(enemies.begin() + i);
			}
		}

		/* RENDER */

		// Initially clear drawing surface
		fb.clear(Color(0, 0, 0));

		// Draw all objects
		for (size_t i = 0; i < objects.size(); i++) {
			objects[i]->render(fb, elapsedMillis);
		}

		// Draw progress bar
		fb.drawRectangle(Point(0, 0), Point(elapsedMillis * fb.getWidth() / TOTAL_DURATION, 10), Color(0xff, 0xff, 0xff));

		// Test draws
		fb.drawLine(Point(0, 0), Point(100, 200), Color(0, 0xff, 0));
		fb.drawLine(Point(100, 200), Point(200, 200), Color(0, 0, 0xff));
		fb.drawLine(Point(100, 300), Point(200, 200), Color(0xff, 0, 0));

		fb.drawPath(Point(100, 100), font.getCharacterPath('A'), Color(0xff, 0, 0), Color(0xff, 0xff, 0xff));

		// Render drawn graphics on screen
		fb.output();

		/* LOOP TIMEKEEPING */

		usleep(millisPerFrame * 1000);
		elapsedMillis += millisPerFrame;
		if (elapsedMillis > TOTAL_DURATION) isRunning = 0;
	}

	/* CLEAN UP */

	for (size_t i = 0; i < objects.size(); i++) {
		delete objects[i];
	}

	fb.clear(Color(0, 0, 0));
	fb.output();

	return 0;
}
