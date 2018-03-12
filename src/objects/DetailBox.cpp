#include <vector>
#include "DetailBox.h"

DetailBox::DetailBox(long elapsedMillis, View *detailView, View *sourceView, Color color) : Renderable(elapsedMillis), color(0,0,0) {
  this->detailView = detailView;
  this->sourceView = sourceView;
  this->color = color;

  std::vector<PathSegment<double> > detailBoxPathSegments;
	detailBoxPathSegments.push_back(PathSegment<double>(Point<double>(0, 0), Point<double>(detailView->sourceSize.x, 0)));    // 1st: top
	detailBoxPathSegments.push_back(PathSegment<double>(Point<double>(detailView->sourceSize.x, 0), Point<double>(detailView->sourceSize.x, detailView->sourceSize.y)));    // 2nd: right
	detailBoxPathSegments.push_back(PathSegment<double>(Point<double>(detailView->sourceSize.x, detailView->sourceSize.y), Point<double>(0, detailView->sourceSize.y)));    // 3rd: bottom
	detailBoxPathSegments.push_back(PathSegment<double>(Point<double>(0, detailView->sourceSize.y), Point<double>(0, 0)));    // 4th: left
	Path detailBox(detailBoxPathSegments);
	detailView->detailBox = detailBox;
}

void DetailBox::update(long elapsedMillis) {
  if (detailView->sourcePosition.x < sourceView->sourcePosition.x) {
    detailView->sourcePosition.x = 0;
  }
  if (detailView->sourcePosition.x > sourceView->sourcePosition.x + sourceView->sourceSize.x - detailView->sourceSize.x) {
    detailView->sourcePosition.x = sourceView->sourcePosition.x + sourceView->sourceSize.x - detailView->sourceSize.x;
  }
  if (detailView->sourcePosition.y < sourceView->sourcePosition.y) {
    detailView->sourcePosition.y = 0;
  }
  if (detailView->sourcePosition.y > sourceView->sourcePosition.y + sourceView->sourceSize.y - detailView->sourceSize.y) {
    detailView->sourcePosition.y = sourceView->sourcePosition.y + sourceView->sourceSize.y - detailView->sourceSize.y;
  }
}

void DetailBox::render(FrameBuffer &fb) {
  fb.drawPath(detailView->sourcePosition + sourceView->position, detailView->detailBox, Color(0x55, color.r, color.g, color.b), color);
}

