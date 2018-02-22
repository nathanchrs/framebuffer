#define FPS 60

#include <unistd.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "common/Input.h"
#include "graphics/FrameBuffer.h"
#include "graphics/Font.h"
#include "graphics/PathSegment.h"
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
	mapView.position = Point<double>(0, 0);
	mapView.size = Point<double>(600, 600);
	mapView.sourcePosition = Point<double>(0, 0);
	mapView.sourceSize = Point<double>(600, 600);
	objects.push_back(&mapView);

	View detailView(0);
	double detailBoxSize = 200;
	detailView.source = &itbBuildings;
	detailView.position = Point<double>(700, 100);
	detailView.size = Point<double>(500, 500);
	detailView.sourcePosition = Point<double>(0, 0);
	detailView.sourceSize = Point<double>(detailBoxSize, detailBoxSize);
	objects.push_back(&detailView);

	std::vector<PathSegment<double> > detailBoxPathSegments;
	detailBoxPathSegments.push_back(PathSegment<double>(Point<double>(0, 0), Point<double>(detailBoxSize, 0)));
	detailBoxPathSegments.push_back(PathSegment<double>(Point<double>(detailBoxSize, 0), Point<double>(detailBoxSize, detailBoxSize)));
	detailBoxPathSegments.push_back(PathSegment<double>(Point<double>(detailBoxSize, detailBoxSize), Point<double>(0, detailBoxSize)));
	detailBoxPathSegments.push_back(PathSegment<double>(Point<double>(0, detailBoxSize), Point<double>(0, 0)));
	Path detailBox(detailBoxPathSegments);

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
			detailView.sourcePosition.y -= 5;
		} else if (input.getKeyPress('a')) {
			detailView.sourcePosition.x -= 5;
		} else if (input.getKeyPress('s')) {
			detailView.sourcePosition.y += 5;
		} else if (input.getKeyPress('d')) {
			detailView.sourcePosition.x += 5;
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

		// Draw detail box
		fb.drawPath(detailView.sourcePosition, detailBox, Color(0x55, 0xff, 0, 0), Color(0xff, 0, 0));

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
