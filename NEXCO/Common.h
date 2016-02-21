#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4819) // unicode
#pragma once

#define _CRTDBG_MAP_ALLOC
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <crtdbg.h>
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
#include <conio.h>
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

class ThreadData {
public:
    ThreadData() : ThreadData(500) {}
    ThreadData(int _numBuffer) : numBuffer(_numBuffer) {
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
    }

    inline void setBuffer(IplImage* image) {
        cvCopy(image, this->buffer[++this->lastSetFrame % this->numBuffer]);
    }

    inline IplImage* getBuffer() {
        return buffer[this->lastSetFrame % this->numBuffer];
    }

    IplImage** buffer;
    int numBuffer;
    int lastGetFrame;
    int lastSetFrame;
    bool startCapture;
};

template <typename Target, typename Source>
Target lexical_cast(const Source& arg) {
    std::stringstream ss;
    Target result;
    ss << arg;
    ss >> result;
    return result;
}