#pragma once

#include "Common.h"
#include "GLCommon.h"
#include "GLView.h"

class GLFrontTireView : public GLView<GLFrontTireView> {
public:
public:
    GLFrontTireView() : GLView<GLFrontTireView>() {}
    GLFrontTireView(int _offsetX, int _offsetY, int _width, int _height, string _title) : GLView<GLFrontTireView>(_offsetX, _offsetY, _width, _height, _title) {}
    virtual ~GLFrontTireView() {}
    void initialize();
    void display();
    void setCallback();

};

GLFrontTireView* GLView<GLFrontTireView>::currentInstance;