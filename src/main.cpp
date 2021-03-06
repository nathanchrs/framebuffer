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
#include "graphics/Bitmap.h"
#include "objects/Renderable.h"
#include "objects/View.h"
#include "objects/Crosshair.h"
#include "objects/DetailBox.h"
#include "objects/Frame.h"

#define TOTAL_DURATION 10000

int main() {

	/* INITIALIZATION */

	FrameBuffer fb("/dev/fb0");
	Input input;
	std::vector<Renderable*> objects;
	
	Font font("./src/assets/font.txt");
	VectorSprite itbBuildingsReal("./src/assets/itb-buildings.txt");
	VectorSprite itbBuildings = itbBuildingsReal.scale(0.5, Point<double>());
	VectorSprite crossHairSprite("./src/assets/crosshair.txt");
	Bitmap headerImage("./src/assets/banner.bmp");

	Path dot;
	dot.segments.push_back(PathSegment<double>(Point<double>(-0.95,-0.95), Point<double>(-0.95,1.05)));
	dot.segments.push_back(PathSegment<double>(Point<double>(-0.95,1.05), Point<double>(1.05,1.05)));
	dot.segments.push_back(PathSegment<double>(Point<double>(1.05,1.05), Point<double>(1.05,-0.95)));
	dot.segments.push_back(PathSegment<double>(Point<double>(1.05,-0.95), Point<double>(-0.95,-0.95)));

	Frame frame(0, fb);
	objects.push_back(&frame);

	View mapView(0);
	mapView.source = &itbBuildings;
	mapView.position = frame.getBottomNavStart();
	mapView.size = frame.getBottomNavSize();
	mapView.sourcePosition = Point<double>(0, 0);
	mapView.sourceSize = frame.getBottomNavSize();
	objects.push_back(&mapView);

	View detailView(0);
	double detailBoxSize = 50;
	detailView.source = &itbBuildings;
	detailView.position = frame.getContentStart();
	detailView.size = frame.getContentSize();
	detailView.sourcePosition = Point<double>(100, 100);
	detailView.sourceSize = Point<double>((detailView.size.x/detailView.size.y) * detailBoxSize, detailBoxSize);
	
	objects.push_back(&detailView);
	
	Crosshair crosshair(0, detailView.position, detailView.position + detailView.size);
	crosshair.source = &crossHairSprite;
	objects.push_back(&crosshair);
	
	DetailBox detailbox(0, &detailView, &mapView, Color(0xff, 0, 0));
	objects.push_back(&detailbox);

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
		} else if (input.getKeyPress('z')) {
			detailView.zoomIn(1.2);
		} else if (input.getKeyPress('x')) {
			detailView.zoomOut(1.2);
		}

		/* UPDATE */

		// Garbage collect dead objects
		for (size_t i = 0; i < objects.size(); i++) {
			if (!objects[i]->isAlive) {
				objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());
			}
		}

		// Update internal state of all objects
		if (crosshair.isLeftHold()) {
		  Point<double> diff = crosshair.diffPosition();
		  diff.x *= detailView.sourceSize.x/detailView.size.x;
		  diff.y *= detailView.sourceSize.y/detailView.size.y;
		  detailView.sourcePosition = detailView.sourcePosition - diff;
		}
		if (crosshair.isRightHold()) {
		  Point<double> prev = crosshair.prevPosition;
		  Point<double> now = crosshair.position;
		  Point<double> center = detailView.position + Point<double>(detailView.size.x * 0.5, detailView.size.y * 0.5);
		  double a1 = atan((now.y - center.y)/(now.x - center.x));
		  if ((now.x - center.x) < 0 && (now.y - center.y) < 0) {
		    a1 -= M_PI;
		  }
		  else if ((now.x - center.x) < 0 && (now.y - center.y) > 0) {
		    a1 += M_PI;
		  }
		  double a2 = atan((prev.y - center.y)/(prev.x - center.x));
		  if ((prev.x - center.x) < 0 && (prev.y - center.y) < 0) {
		    a2 -= M_PI;
		  }
		  else if ((prev.x - center.x) < 0 && (prev.y - center.y) > 0) {
		    a2 += M_PI;
		  }
		  double a = (a1-a2 < 0) ? a1-a2 + 2*M_PI : a1-a2;
		  Point<double> origin = mapView.sourcePosition + Point<double>(mapView.sourceSize.x * 0.5, mapView.sourceSize.x * 0.5);
		  VectorSprite newSprite = mapView.source->rotate(a * 180/M_PI, origin);
		  mapView.source->paths = newSprite.paths;
		  
	    double sinTetha = sin(a);
	    double cosTetha = cos(a);
      double xPos = detailView.sourcePosition.x + detailView.sourceSize.x/2;
      double yPos = detailView.sourcePosition.y + detailView.sourceSize.y/2;
		  detailView.sourcePosition.x = ((xPos-origin.x)*cosTetha - (yPos-origin.y)*sinTetha) + origin.x - detailView.sourceSize.x/2;
		  detailView.sourcePosition.y = ((xPos-origin.x)*sinTetha + (yPos-origin.y)*cosTetha) + origin.y - detailView.sourceSize.y/2;
		}
		
		for (size_t i = 0; i < objects.size(); i++) {
			objects[i]->update(elapsedMillis);
		}
		
		/*if (crosshair.isClicked()) {
		  itbBuildings.paths.push_back(dot.translate(crosshair.position - mapView.position));
		  itbBuildings.fillColors.push_back(Color(0xff, 0xff, 0xff));
		  itbBuildings.strokeColors.push_back(Color(0xff, 0xff, 0xff));
		}*/

		/* RENDER */

		// Initially clear drawing surface
		fb.clear(Color(0x07, 0x00, 0x1a));

		// Draw all objects
		headerImage.draw(fb, Point<double>(frame.thickness-1, frame.thickness+1));
		for (size_t i = 0; i < objects.size(); i++) {
			objects[i]->render(fb);
		}


		// Draw credits text
		/*int dataWidth = 31;
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
		fb.drawText(Point<double>(x_text, y_text+180), "LAZUARDI FIRDAUS   13515136", font, 1.0, Color(0xff, 0, 0xff), Color(0xff, 0xff, 0xff));*/

		// Render drawn graphics on screen
		fb.output();

		/* LOOP TIMEKEEPING */

		usleep(millisPerFrame * 1000);
		elapsedMillis += millisPerFrame;
	}

	/* CLEAN UP */

	fb.clear(Color(0, 0, 0));
	fb.output();

	return 0;
}
