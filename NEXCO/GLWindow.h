#pragma once

#include "Common.h"
#include "GLCommon.h"
#include "GLCameraView.h"
#include "GLFrontTireView.h"
#include "GLRearTireView.h"

class GLWindow {
public:
	GLWindow(int argc, char** argv, CameraBuffer* _cameraBuffer);
    virtual ~GLWindow();

	void showWindow();
    void setFrontTireImage(IplImage* _image);
    void setRearTireImage(IplImage* _image);

private:
    CameraBuffer* cameraBuffer;
    GLCameraView* glCameraView;
    GLFrontTireView* glFrontTireView;
    GLRearTireView* glRearTireView;
};

