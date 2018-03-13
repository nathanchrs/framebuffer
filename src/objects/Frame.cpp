#include <math.h>
#include "Frame.h"

Frame::Frame(long elapsedMillis, FrameBuffer &fb) : Renderable(elapsedMillis) {
  width = fb.getWidth();
  height = fb.getHeight();
  Path box;

	// Outer box
	box.segments.push_back(PathSegment<double>(Point<double>(0, 0), Point<double>(width, 0)));
	box.segments.push_back(PathSegment<double>(Point<double>(width, 0), Point<double>(width, height)));
	box.segments.push_back(PathSegment<double>(Point<double>(width, height), Point<double>(0, height)));
	box.segments.push_back(PathSegment<double>(Point<double>(0, height), Point<double>(0, 0)));
	
	// Navbar
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, thickness), Point<double>(width-1-thickness, thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-1-thickness, thickness), Point<double>(width-1-thickness, navbarHeight+thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-1-thickness, navbarHeight+thickness), Point<double>(thickness, navbarHeight+thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, navbarHeight+thickness), Point<double>(thickness, thickness)));
	
	// Top sidebar
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, navbarHeight+2*thickness), Point<double>(sidebarWidth+thickness, navbarHeight+2*thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(sidebarWidth+thickness, navbarHeight+2*thickness), Point<double>(sidebarWidth+thickness, height-1-2*thickness-bottomSidebarHeight)));
	box.segments.push_back(PathSegment<double>(Point<double>(sidebarWidth+thickness, height-1-2*thickness-bottomSidebarHeight), Point<double>(thickness, height-1-2*thickness-bottomSidebarHeight)));
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, height-1-2*thickness-bottomSidebarHeight), Point<double>(thickness, navbarHeight+2*thickness)));
	
	// Bottom sidebar
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, height-1-thickness-bottomSidebarHeight), Point<double>(sidebarWidth+thickness, height-1-thickness-bottomSidebarHeight)));
	box.segments.push_back(PathSegment<double>(Point<double>(sidebarWidth+thickness, height-1-thickness-bottomSidebarHeight), Point<double>(sidebarWidth+thickness, height-1-thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(sidebarWidth+thickness, height-1-thickness), Point<double>(thickness, height-1-thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(thickness, height-1-thickness), Point<double>(thickness, height-1-thickness-bottomSidebarHeight)));
	
	// Content box
	box.segments.push_back(PathSegment<double>(Point<double>(2*thickness+sidebarWidth, navbarHeight+2*thickness), Point<double>(width-1-thickness, navbarHeight+2*thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-1-thickness, navbarHeight+2*thickness), Point<double>(width-1-thickness, height-1-thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-1-thickness, height-1-thickness), Point<double>(2*thickness+sidebarWidth, height-1-thickness)));
	box.segments.push_back(PathSegment<double>(Point<double>(2*thickness+sidebarWidth, height-1-thickness), Point<double>(2*thickness+sidebarWidth, navbarHeight+2*thickness)));
	
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

