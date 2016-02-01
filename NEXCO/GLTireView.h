#pragma once

#include "Common.h"
#include "GLCommon.h"
#include "GLView.h"

class GLTireView : public GLView<GLTireView> {
public:
public:
    GLTireView() : GLView<GLTireView>() {}
    GLTireView(int _offsetX, int _offsetY, int _width, int _height, string _title) : GLView<GLTireView>(_offsetX, _offsetY, _width, _height, _title) {}
    ~GLTireView() {}
    void initialize();
    void display();
    void setCallback();

};

GLTireView* GLView<GLTireView>::currentInstance;