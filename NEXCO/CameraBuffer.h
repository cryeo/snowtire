#pragma once

#include "Common.h"

class CameraBuffer {
public:
    CameraBuffer() : CameraBuffer(500) {}
    CameraBuffer(int _numBuffer) : numBuffer(_numBuffer) {
        this->lastSetFrame = -1;
        this->lastGetFrame = -1;

        this->buffer = new IplImage*[this->numBuffer];
        for (int i = 0; i < this->numBuffer; i++) {
            this->buffer[i] = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
        }
    }

    virtual ~CameraBuffer() {
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
};

