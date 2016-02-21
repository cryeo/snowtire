#pragma once

#include "Common.h"
#include "GLCommon.h"
#include "GLView.h"

class GLRearTireView : public GLView<GLRearTireView> {
public:
public:
    GLRearTireView() : GLView<GLRearTireView>() {}
    GLRearTireView(int _offsetX, int _offsetY, int _width, int _height, std::string _title) : GLView<GLRearTireView>(_offsetX, _offsetY, _width, _height, _title) {}
    virtual ~GLRearTireView() {}
    void initialize();
    void display();
    void setCallback();

};

GLRearTireView* GLView<GLRearTireView>::currentInstance;