#ifndef INPUT_H
#define INPUT_H

#include <thread>

class Input {
public:
	Input();
    ~Input();

    bool getKeyPress(int c);

private:
    void inputHandler();

    std::thread inputThread;
    bool keyStates[255];
    bool active;
};

#endif
