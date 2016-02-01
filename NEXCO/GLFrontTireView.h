#pragma once

#include "Common.h"
#include "GLCommon.h"
#include "GLView.h"

class GLFrontTireView : public GLView<GLFrontTireView> {
public:
    static void initialize(int _offsetX, int _offsetY, int _width, int _height, string _title, ThreadData* _threadData);
    static void loadImage(IplImage* _image);
    static void timer(int value);
    static void display();
};
