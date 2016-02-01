#pragma once

#include "Common.h"
#include "GLCommon.h"

class GLWindow {
public:
	GLWindow(int argc, char** argv, ThreadData* _threadData);
	virtual ~GLWindow();

	void showWindow();
    void setFrontTireImage(IplImage* image);
    void setCameraImage(IplImage* image);
private:
    ThreadData* threadData;
};

