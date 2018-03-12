#include <math.h>
#include "Frame.h"

Frame::Frame(long elapsedMillis, FrameBuffer &fb) : Renderable(elapsedMillis) {
  long width = fb.getWidth();
  long height = fb.getHeight();
  Path box;

	// Outer box
	box.segments.push_back(PathSegment<double>(Point<double>(0, 0), Point<double>(width, 0)));
	box.segments.push_back(PathSegment<double>(Point<double>(width, 0), Point<double>(width, height)));
	box.segments.push_back(PathSegment<double>(Point<double>(width, height), Point<double>(0, height)));
	box.segments.push_back(PathSegment<double>(Point<double>(0, height), Point<double>(0, 0)));
	
	// Navbar
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, thickness), Point<double>(width-thickness, thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-thickness, thickness), Point<double>(width-thickness, navbarHeight)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-thickness, navbarHeight), Point<double>(thickness, navbarHeight)));
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, navbarHeight), Point<double>(thickness, thickness)));
	
	// Top sidebar
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, navbarHeight+thickness), Point<double>(sidebarWidth, navbarHeight+thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(sidebarWidth, navbarHeight+thickness), Point<double>(sidebarWidth, height-thickness-bottomSidebarHeight)));
	box.segments.push_back(PathSegment<double>(Point<double>(sidebarWidth, height-thickness-bottomSidebarHeight), Point<double>(thickness, height-thickness-bottomSidebarHeight)));
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, height-thickness-bottomSidebarHeight), Point<double>(thickness, navbarHeight+thickness)));
	
	// Bottom sidebar
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, height-sidebarWidth), Point<double>(sidebarWidth, height-sidebarWidth)));
	box.segments.push_back(PathSegment<double>(Point<double>(sidebarWidth, height-sidebarWidth), Point<double>(sidebarWidth, height-thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(sidebarWidth, height-thickness), Point<double>(thickness, height-thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, height-thickness), Point<double>(thickness, height-sidebarWidth)));
	
	// Content box
	box.segments.push_back(PathSegment<double>(Point<double>(thickness+sidebarWidth, navbarHeight+thickness), Point<double>(width-thickness, navbarHeight+thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-thickness, navbarHeight+thickness), Point<double>(width-thickness, height-thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-thickness, height-thickness), Point<double>(thickness+sidebarWidth, height-thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(thickness+sidebarWidth, height-thickness), Point<double>(thickness+sidebarWidth, navbarHeight+thickness)));
	
	source.paths.push_back(box);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
}

void Frame::update(long elapsedMillis) {
  double frequency = 0.01;
  source.fillColors[0].r = sin(elapsedMillis*frequency + 0) * 127 + 128;
  source.strokeColors[0].r = sin(elapsedMillis*frequency + 0) * 127 + 128;
  source.fillColors[0].g = sin(elapsedMillis*frequency + 2) * 127 + 128;
  source.strokeColors[0].g = sin(elapsedMillis*frequency + 2) * 127 + 128;
  source.fillColors[0].b = sin(elapsedMillis*frequency + 4) * 127 + 128;
  source.strokeColors[0].b = sin(elapsedMillis*frequency + 4) * 127 + 128;
}

void Frame::render(FrameBuffer &fb) {
  fb.drawVectorSprite(Point<double>(0,0), source);
}

