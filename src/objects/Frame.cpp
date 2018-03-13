#include <math.h>
#include "Frame.h"
#include "../graphics/Font.h"

Frame::Frame(long elapsedMillis, FrameBuffer &fb) : Renderable(elapsedMillis) {
  Font font("./src/assets/font.txt");
  width = fb.getWidth();
  height = fb.getHeight();
  Path box;
  Path text;
  Path text1;
  Path text2;
  Path text3;
  Path text4;
  Path text5;
  Path text6;
  Path text7;
  int textStep=50;
  float textSize=0.7f;

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
	
	//Text
	text1 = font.getTextPath("Z TO ZOOM IN",textSize);
	text1 = text1.translate(Point<double>(thickness + 25,navbarHeight + (thickness*2) + 20 + (0*textStep)));
	text2 = font.getTextPath("X TO ZOOM OUT",textSize);
	text2 = text2.translate(Point<double>(thickness + 25,navbarHeight + (thickness*2) + 20 + (1*textStep)));
	text3 = font.getTextPath("W TO MOVE UP",textSize);
	text3 = text3.translate(Point<double>(thickness + 25,navbarHeight + (thickness*2) + 20 + (2*textStep)));
	text4 = font.getTextPath("S TO MOVE DOWN",textSize);
	text4 = text4.translate(Point<double>(thickness + 25,navbarHeight + (thickness*2) + 20 + (3*textStep)));
	text5 = font.getTextPath("A TO MOVE LEFT",textSize);
	text5 = text5.translate(Point<double>(thickness + 25,navbarHeight + (thickness*2) + 20 + (4*textStep)));
	text6 = font.getTextPath("D TO MOVE RIGHT",textSize);
	text6 = text6.translate(Point<double>(thickness + 25,navbarHeight + (thickness*2) + 20 + (5*textStep)));
	text7 = font.getTextPath("MOUSE DRAG TO MOVE",textSize);
	text7 = text7.translate(Point<double>(thickness + 25,navbarHeight + (thickness*2) + 20 + (6*textStep)));

	source.paths.push_back(box);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
	source.paths.push_back(text1);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
	source.paths.push_back(text2);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
	source.paths.push_back(text3);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
	source.paths.push_back(text4);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
	source.paths.push_back(text5);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
	source.paths.push_back(text6);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
	source.paths.push_back(text7);
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

