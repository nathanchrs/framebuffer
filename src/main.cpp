#define FPS 60

#include <unistd.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "common/Input.h"
#include "graphics/FrameBuffer.h"
#include "graphics/Font.h"
#include "graphics/VectorSprite.h"
#include "objects/Renderable.h"
#include "objects/View.h"

#define TOTAL_DURATION 10000

int main() {

	/* INITIALIZATION */

	FrameBuffer fb("/dev/fb0");
	Input input;
	std::vector<Renderable*> objects;
	
	Font font("./src/assets/font.txt");
	VectorSprite itbBuildings("./src/assets/itb-buildings.txt");

	View mapView(0);
	mapView.source = &itbBuildings;
	mapView.position = Point<double>(100, 100);
	objects.push_back(&mapView);

	/* MAIN LOOP */

	long millisPerFrame = 1000 / FPS;
	long elapsedMillis = 0;
	bool isRunning = true;
	while (isRunning) {

		/* PROCESS INPUT */

		if (input.getKeyPress('q')) {
			isRunning = false;
			break;
		} else if (input.getKeyPress('w')) {
			mapView.position.y -= 10;
		} else if (input.getKeyPress('a')) {
			mapView.position.x -= 10;
		} else if (input.getKeyPress('s')) {
			mapView.position.y += 10;
		} else if (input.getKeyPress('d')) {
			mapView.position.x += 10;
		}

		/* UPDATE */

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

		// Draw credits text
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
	}

	/* CLEAN UP */

	for (size_t i = 0; i < objects.size(); i++) {
		delete objects[i];
	}

	fb.clear(Color(0, 0, 0));
	fb.output();

	return 0;
}
