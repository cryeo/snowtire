#pragma once

#include "Common.h"
#include "xiApi.h"
#pragma comment(lib, "C:\\XIMEA\\API\\x64\\m3apiX64.lib")

#define XI_FRAMERATE (static_cast<const float>(frameRate))

class CRCamera {
public:
    CRCamera() {
        this->iplImage = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    }

    void initialize();

    void capture(XI_IMG &image) {
        xiGetImage(this->xiH, 5000, &image);
    }
    
    inline int startAcquisition() {
        return xiStartAcquisition(this->xiH);
    }

    inline int stopAcquisition() {
        return xiStopAcquisition(this->xiH);
    }

    virtual ~CRCamera() {
        cvReleaseImage(&iplImage);
        xiCloseDevice(xiH);
    }

    HANDLE xiH;
    IplImage* iplImage;
};

