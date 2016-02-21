#pragma once

#include "Common.h"
#include "GLCommon.h"

template<class T>
class GLView {
public:
    static T* currentInstance;

    GLView() {}
    GLView(int _offsetX, int _offsetY, int _width, int _height, std::string _title) : offsetX(_offsetX), offsetY(_offsetY), width(_width), height(_height), title(_title) {
        this->image = cvCreateImage(cvSize(this->width, this->height), IPL_DEPTH_8U, 3);
        cvSet(this->image, cvScalar(0, 0, 0));
    }

    virtual ~GLView() {
        cvReleaseImage(&this->image);
    }

    void initialize() {
        glutInitWindowPosition(this->offsetX, this->offsetY);
        glutInitWindowSize(this->width, this->height);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
        this->windowId = glutCreateWindow(this->title.c_str());
        this->currentInstance = static_cast<T*>(this);
    }

    void setCameraBuffer(CameraBuffer* _cameraBuffer) {
        this->cameraBuffer = _cameraBuffer;
    }

    void timer(int value) {
        glutSetWindow(this->windowId);
        glutPostRedisplay();
        glutTimerFunc(1000 / REFRESH_RATE, timerCallback, 0);
    }

    void display() {}
    void keyboard(uchar key, int x, int y) {}

    void setImage(IplImage* _image) {
        cvCopy(_image, this->image);
    }

    static void displayCallback() {
        currentInstance->display();
    }

    static void timerCallback(int value) {
        currentInstance->timer(value);
    }

    static void keyboardCallback(uchar key, int x, int y) {
        currentInstance->keyboard(key, x, y);
    }

protected:
	int offsetX;
	int offsetY;
	int width;
	int height;
	std::string title;
	GLuint windowId;
	IplImage* image;
    CameraBuffer* cameraBuffer;
};
