#pragma once

#include "Common.h"
#include "GLCommon.h"
#include "GLView.h"

class GLCameraView : public GLView<GLCameraView> {
public:
	static void initialize(int _offsetX, int _offsetY, int _width, int _height, string _title, ThreadData* _threadData);
    static void loadImage(IplImage* _image);
	static void timer(int value);
	static void display();
    static void keyboard(uchar key, int x, int y);
};
