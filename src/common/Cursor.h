#ifndef CURSOR_H
#define CURSOR_H

#include "../graphics/Point.h"
#include <thread>

class Cursor {
public:
	Cursor(Point<double> topLeftClip, Point<double> bottomRightClip);
    ~Cursor();

    bool getLeftClick();
    bool getRightClick();
    bool getLeftClickDown();
    bool getRightClickDown();
    bool getLeftClickUp();
    bool getRightClickUp();

    Point<double> getPosition();
    void setPosition(Point<double> position);

    void setClip(Point<double> topLeftClip, Point<double> bottomRightClip);

private:
    void inputHandler();
    void clipCursorPosition();

    std::thread inputThread;
    bool currentLeftClick;
    bool currentRightClick;
    bool prevLeftClick;
    bool prevRightClick;
    Point<double> position;
    Point<double> topLeftClip;
    Point<double> bottomRightClip;
    int mouseDevice;
    bool active;
    double sensitivity = 1.2;
};

#endif
