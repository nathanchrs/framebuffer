#include "Input.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

void enableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

void disableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

int kbhit(void) {
    int bytesWaiting;
    ioctl(0, FIONREAD, &bytesWaiting);
    return bytesWaiting > 0;
}

Input::Input() {
    for (unsigned char i = 0; i < 255; i++) {
        keyStates[i] = false;
    }
    active = true;
    enableRawMode();
    inputThread = std::thread(&Input::inputHandler, this);
}

Input::~Input() {
    active = false;
    inputThread.join();
    disableRawMode();
}

bool Input::getKeyPress(int c) {
    if (('a' <= c && c <= 'z') || ('0' <= c && c <= '9')) {
        if (keyStates[c]) {
            keyStates[c] = false;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void Input::inputHandler() {
    while (active) {
        if (kbhit()) {
            int c = getchar();
            if (('a' <= c && c <= 'z') || ('0' <= c && c <= '9')) {
                keyStates[c] = true;
            }
        }
        usleep(1);
    }
}
