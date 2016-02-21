#pragma once

#include "Common.h"
#include "xiApi.h"
#pragma comment(lib, "C:\\XIMEA\\API\\x64\\m3apiX64.lib")

class CRCamera {
public:
    CRCamera() {
        this->acquiring = false;
    }

    virtual ~CRCamera() {
        this->stopAcquisition();
        xiCloseDevice(this->xiH);
    }

    void initialize();

    void capture(XI_IMG &image) {
        xiGetImage(this->xiH, 5000, &image);
    }
    
    inline int startAcquisition() {
        int ret = xiStartAcquisition(this->xiH);
        this->acquiring = (ret == XI_OK);
        return ret;
    }

    inline int stopAcquisition() {
        int ret = xiStopAcquisition(this->xiH);
        this->acquiring = !(ret == XI_OK);
        return ret;
    }

    inline bool isAcquiring() {
        return this->acquiring;
    }
private:
    HANDLE xiH;
    bool acquiring;
};

