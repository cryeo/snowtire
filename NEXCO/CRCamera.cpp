#include "CRCamera.h"

void CRCamera::initialize() {
    DWORD dwNumberOfDevices = 0;
    float maxFps;

    xiGetNumberDevices(&dwNumberOfDevices);
    if (!dwNumberOfDevices) {
        LOG("Camera was NOT found.");
    }

    xiOpenDevice(0, &this->xiH);

    int exposure = (int)(1000000.0 / XI_FRAMERATE);

    xiSetParamInt(this->xiH, XI_PRM_EXPOSURE, exposure);
    xiGetParamFloat(this->xiH, XI_PRM_FRAMERATE XI_PRM_INFO_MAX, &maxFps);

    LOG("Max framerate : %f [fps]", maxFps);
    LOG("Exposure time : %d [us]", exposure);

    xiSetParamInt(this->xiH, XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FRAME_RATE);
    xiSetParamFloat(this->xiH, XI_PRM_FRAMERATE, XI_FRAMERATE);
    xiSetParamInt(this->xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RGB24);
}