#pragma once
#include "Common.h"
#include "CRSignal.h"
#include "GLWindow.h"

class CRTracking {
public:
    CRTracking(std::function<void(const CvArr*, const CvArr*, CvArr*)> _diffMethod, ThreadData* _threadData, CRSignal* _crSignal, GLWindow* _glWindow) : threadData(_threadData), crSignal(_crSignal), glWindow(_glWindow) {
        this->base = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
        this->baseGray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
        this->curr = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
        this->currGray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
        this->diff = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
        this->optimal = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);

        using namespace std::placeholders;
        this->diffMethod = std::bind(_diffMethod, _1, _2, _3);
    }

    virtual ~CRTracking() {
        cvReleaseImage(&this->base);
        cvReleaseImage(&this->baseGray);
        cvReleaseImage(&this->curr);
        cvReleaseImage(&this->currGray);
        cvReleaseImage(&this->diff);
        cvReleaseImage(&this->optimal);
    }

    void trace();

private:
    IplImage* base;
    IplImage* baseGray;
    IplImage* curr;
    IplImage* currGray;
    IplImage* diff;
    IplImage* optimal;

    std::function<void(const CvArr*, const CvArr*, CvArr*)> diffMethod;

    ThreadData* threadData;
    CRSignal* crSignal;
    GLWindow* glWindow;
};