#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4819) // unicode
#pragma once

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <future>
#include <initializer_list>
#include <sys/types.h> 
#include <cstring>
#include <cerrno>
#include <memory>
#include <direct.h>

#include "OpenCV.h"
#include "Socket.h"
#include "Logger.h"

using namespace std;

extern float frameRate;
extern string serverHost;
extern int serverPort;
extern bool debug;
extern bool method;
extern int bufferSize;

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

enum States {
    INITIAL = 0,
    TIRE_IN = 1,
    TIRE_OUT = 2
};

class State {
public:

    State() : _state(States::INITIAL) {
        this->transition[States::INITIAL][0] = States::TIRE_IN;
        this->transition[States::INITIAL][1] = States::INITIAL;
        this->transition[States::TIRE_IN][0] = States::INITIAL;
        this->transition[States::TIRE_IN][1] = States::TIRE_OUT;
        this->transition[States::TIRE_OUT][0] = States::TIRE_IN;
        this->transition[States::TIRE_OUT][1] = States::INITIAL;
    }

    inline void transit(char _action) {
        // f : 0, r : 1
        int action = (_action == 'f') ? 0 : 1;
        this->_state = this->transition[this->_state][action];
    }

    inline States getState() {
        return this->_state;
    }

private:
    States transition[3][2];
    States _state;
};

class ThreadData {
public:
    ThreadData() : ThreadData(500) {}
    ThreadData(int _numBuffer) : numBuffer(_numBuffer) {
        this->state = new State();
        this->lastSetFrame = -1;
        this->lastGetFrame = -1;
        this->startCapture = false;

        this->buffer = new IplImage*[this->numBuffer];
        for (int i = 0; i < this->numBuffer; i++) {
            this->buffer[i] = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
        }
    }

    virtual ~ThreadData() {
        for (int i = 0; i < this->numBuffer; i++) {
            cvReleaseImage(&this->buffer[i]);
        }
        delete this->buffer;
        delete this->state;
    }

    inline void setBuffer(IplImage* image) {
        cvCopy(image, this->buffer[++this->lastSetFrame % this->numBuffer]);
    }

    inline IplImage* getBuffer() {
        return buffer[this->lastSetFrame % this->numBuffer];
    }

    IplImage** buffer;
    State* state;
    int numBuffer;
    int lastGetFrame;
    int lastSetFrame;
    bool startCapture;

    class Signal {
    public:
        Signal() : signal(false) {}
        ~Signal() {}

        inline void flipSignal() {
            this->signal = !this->signal;
        }

        inline void setSignal(bool _signal) {
            this->signal = _signal;
        }

        inline bool getSignal() {
            return this->signal;
        }
        bool signal;
    };
};