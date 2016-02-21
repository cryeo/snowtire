#pragma once

#include "CameraBuffer.h"
#include "GLWindow.h"
#include "CRSignal.h"
#include "CRTracking.h"
#include "CRCamera.h"

namespace Global {
    extern CameraBuffer* volatile cameraBuffer;
    extern GLWindow* volatile glWindow;
    extern CRSignal* volatile crSignal;
    extern CRTracking* volatile crTracking;
    extern CRCamera* volatile crCamera;
}