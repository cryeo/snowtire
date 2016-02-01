#pragma once

#include "Common.h"
#include "GLCommon.h"
#include "GLCameraView.h"
#include "GLTireView.h"

class GLWindow {
public:
	GLWindow(int argc, char** argv, ThreadData* _threadData);
    virtual ~GLWindow();

	void showWindow();
    void setFrontTireImage(IplImage* _image);

private:
    ThreadData* threadData;
    GLCameraView* glCameraView;
    GLTireView* glFrontTireView;
    GLTireView* glRearTireView;
};

