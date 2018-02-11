#define FPS 60

#include <unistd.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "graphics/FrameBuffer.h"
#include "graphics/Font.h"
#include "graphics/VectorSprite.h"
#include "objects/Renderable.h"
#include "objects/Enemy.h"
#include "objects/Explosion.h"

#define TOTAL_DURATION 10000

int main() {

	/* INITIALIZATION */

	FrameBuffer fb = FrameBuffer("/dev/fb0");
	std::vector<Renderable*> objects;
	std::vector<Enemy*> enemies;

	Enemy::planeSprite = VectorSprite("./src/assets/plane.txt");
	Enemy::propSprite = VectorSprite("./src/assets/prop.txt");
	Font font = Font("font.txt");

	long enemySpawnInterval = 10000;

	/* MAIN LOOP */

	long millisPerFrame = 1000 / FPS;
	long elapsedMillis = 0;
	unsigned char isRunning = 1;
	while (isRunning) {

		// TODO: read input
		
		/* UPDATE */

		// Spawn enemies
		if ((elapsedMillis % enemySpawnInterval) >= 0 && (elapsedMillis % enemySpawnInterval) < millisPerFrame) {
			Enemy *newEnemy = new Enemy(elapsedMillis, Point<double>(fb.getWidth() / 2, fb.getHeight() / 2));
			objects.push_back(newEnemy);
			enemies.push_back(newEnemy);
		}

		// Destroy enemies which has moved offscreen
		for (int i = enemies.size() - 1; i >= 0; i--) {
			if (enemies[i]->position.x > fb.getWidth() + 20 - 500) {
				objects.push_back(new Explosion(elapsedMillis, enemies[i]->position));
				enemies[i]->die();
				delete enemies[i];
				enemies.erase(enemies.begin() + i);
			}
		}

		// Garbage collect dead objects
		for (size_t i = 0; i < objects.size(); i++) {
			if (!objects[i]->isAlive) {
				objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());
			}
		}

		// Update internal state of all objects
		for (size_t i = 0; i < objects.size(); i++) {
			objects[i]->update(elapsedMillis);
		}

		/* RENDER */

		// Initially clear drawing surface
		fb.clear(Color(0, 0, 0));

		// Draw all objects
		for (size_t i = 0; i < objects.size(); i++) {
			objects[i]->render(fb);
		}

		// Draw progress bar
		fb.drawLine(Point<double>(0, 0), Point<double>(elapsedMillis * fb.getWidth() / TOTAL_DURATION, 0), Color(0xff, 0xff, 0xff));

		// Test draws
		fb.drawLine(Point<double>(0, 0), Point<double>(100, 200), Color(0, 0xff, 0));
		fb.drawLine(Point<double>(100, 200), Point<double>(200, 200), Color(0, 0, 0xff));
		fb.drawLine(Point<double>(100, 300), Point<double>(200, 200), Color(0xff, 0, 0));

		int dataWidth = 31;
		int dataHeight = 7;
		long width = fb.getWidth();
		long height = fb.getHeight();
		int x_text = (width - (dataWidth*20))/2;
		int y_text = height - (elapsedMillis * (height + (dataHeight*40 - 15)) / TOTAL_DURATION);
		fb.drawText(Point<double>(x_text, y_text), "         KELOMPOK EVOS         ", font, 1.0, Color(0xff, 0, 0), Color(0xff, 0xff, 0xff));
		fb.drawText(Point<double>(x_text, y_text+60), "JONATHAN CHRISTOPHER   13515001", font, 1.0, Color(0xff, 0x66, 0), Color(0xff, 0xff, 0xff));
		fb.drawText(Point<double>(x_text, y_text+90), "ROBBY SYAIFULLAH   13515013", font, 1.0, Color(0xff, 0xff, 0), Color(0xff, 0xff, 0xff));
		fb.drawText(Point<double>(x_text, y_text+120), "KEVIN JONATHAN   13515052", font, 1.0, Color(0, 0xff, 0), Color(0xff, 0xff, 0xff));
		fb.drawText(Point<double>(x_text, y_text+150), "AFIF BAMBANG P   13515058", font, 1.0, Color(0, 0, 0xff), Color(0xff, 0xff, 0xff));
		fb.drawText(Point<double>(x_text, y_text+180), "LAZUARDI FIRDAUS   13515136", font, 1.0, Color(0xff, 0, 0xff), Color(0xff, 0xff, 0xff));

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
