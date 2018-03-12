#include "Frame.h"

Frame::Frame(long elapsedMillis, FrameBuffer &fb) : Renderable(elapsedMillis) {
  long width = fb.getWidth();
  long height = fb.getHeight();
  
  Path box;
	box.segments.push_back(PathSegment<double>(Point<double>(0, 0), Point<double>(width, 0)));
	box.segments.push_back(PathSegment<double>(Point<double>(width, 0), Point<double>(width, height)));
	box.segments.push_back(PathSegment<double>(Point<double>(width, height), Point<double>(0, height)));
	box.segments.push_back(PathSegment<double>(Point<double>(0, height), Point<double>(0, 0)));
	
	box.segments.push_back(PathSegment<double>(Point<double>(10, 10), Point<double>(width-10, 10)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-10, 10), Point<double>(width-10, 80)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-10, 80), Point<double>(10, 80)));
	box.segments.push_back(PathSegment<double>(Point<double>(10, 80), Point<double>(10, 10)));
	
	box.segments.push_back(PathSegment<double>(Point<double>(10, 90), Point<double>(310, 90)));
	box.segments.push_back(PathSegment<double>(Point<double>(310, 90), Point<double>(310, height-320)));
	box.segments.push_back(PathSegment<double>(Point<double>(310, height-320), Point<double>(10, height-320)));
	box.segments.push_back(PathSegment<double>(Point<double>(10, height-320), Point<double>(10, 90)));
	
	box.segments.push_back(PathSegment<double>(Point<double>(10, height-310), Point<double>(310, height-310)));
	box.segments.push_back(PathSegment<double>(Point<double>(310, height-310), Point<double>(310, height-10)));
	box.segments.push_back(PathSegment<double>(Point<double>(310, height-10), Point<double>(10, height-10)));
	box.segments.push_back(PathSegment<double>(Point<double>(10, height-10), Point<double>(10, height-310)));
	
	box.segments.push_back(PathSegment<double>(Point<double>(320, 90), Point<double>(width-10, 90)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-10, 90), Point<double>(width-10, height-10)));
	box.segments.push_back(PathSegment<double>(Point<double>(width-10, height-10), Point<double>(320, height-10)));
	box.segments.push_back(PathSegment<double>(Point<double>(320, height-10), Point<double>(320, 90)));
	
	source.paths.push_back(box);
	source.fillColors.push_back(Color(0xff, 0xff, 0xff));
	source.strokeColors.push_back(Color(0xff, 0xff, 0xff));
}

void Frame::render(FrameBuffer &fb) {
  fb.drawVectorSprite(Point<double>(0,0), source);
}

