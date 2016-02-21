#pragma once

#include "Common.h"
#include "GLCommon.h"
#include "GLView.h"

class GLCameraView : public GLView<GLCameraView> {
public:
    GLCameraView() : GLView<GLCameraView>() {}
    GLCameraView(int _offsetX, int _offsetY, int _width, int _height, std::string _title) : GLView<GLCameraView>(_offsetX, _offsetY, _width, _height, _title) {}
    virtual ~GLCameraView() {}

    void initialize();
    void display();
    void setCallback();
};

GLCameraView* GLView<GLCameraView>::currentInstance;