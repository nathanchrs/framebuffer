#include "Cursor.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <iostream>

void Cursor::clipCursorPosition() {
    if (position.x < topLeftClip.x) {
        position.x = topLeftClip.x;
    } else if (position.x > bottomRightClip.x) {
        position.x = bottomRightClip.x;
    }
    if (position.y < topLeftClip.y) {
        position.y = topLeftClip.y;
    } else if (position.y > bottomRightClip.y) {
        position.y = bottomRightClip.y;
    }
}

Cursor::Cursor(Point<double> topLeftClip, Point<double> bottomRightClip) {
    currentLeftClick = false;
    currentRightClick = false;
    prevLeftClick = false;
    prevRightClick = false;

    this->topLeftClip = topLeftClip;
    this->bottomRightClip = bottomRightClip;
    position = topLeftClip;

    const char *devicePath = "/dev/input/mice";
    mouseDevice = open(devicePath, O_RDONLY);
    if (mouseDevice == -1) {
        printf("Cursor: error opening %s\n", devicePath);
        return;
    }

    active = true;
    inputThread = std::thread(&Cursor::inputHandler, this);
}

Cursor::~Cursor() {
    active = false;
    inputThread.join();
}

bool Cursor::getLeftClick() {
    return currentLeftClick;
}

bool Cursor::getRightClick() {
    return currentRightClick;
}

bool Cursor::getLeftClickDown() {
    return !prevLeftClick && currentLeftClick;
}

bool Cursor::getRightClickDown() {
    return !prevRightClick && currentRightClick;
}

bool Cursor::getLeftClickUp() {
    return prevLeftClick && !currentLeftClick;
}

bool Cursor::getRightClickUp() {
    return prevRightClick && !currentRightClick;
}

Point<double> Cursor::getPosition() {
    return position;
}

void Cursor::setPosition(Point<double> position) {
    this->position = position;
    clipCursorPosition();
}

void Cursor::setClip(Point<double> topLeftClip, Point<double> bottomRightClip) {
    this->topLeftClip = topLeftClip;
    this->bottomRightClip = bottomRightClip;
    clipCursorPosition();
}

void Cursor::inputHandler() {
    int bytesRead;
    struct input_event inputEvent;

    while (active) {
        bytesRead = read(mouseDevice, &inputEvent, sizeof(struct input_event));
        if (bytesRead > 0) {
            prevLeftClick = currentLeftClick;
            prevRightClick = currentRightClick;

            unsigned char *mouseData = (unsigned char*) &inputEvent;
            currentLeftClick = mouseData[0] & 0x1;
            currentRightClick = mouseData[0] & 0x2;
            position.x += (double) ((char) mouseData[1]);
            position.y -= (double) ((char) mouseData[2]);

            clipCursorPosition();
        }
        usleep(1);
    }
}
