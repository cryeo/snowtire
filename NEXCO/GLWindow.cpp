#include "GLWindow.h"

GLWindow::GLWindow(int argc, char** argv, ThreadData* _threadData) : threadData(_threadData) {
	glutInit(&argc, argv);

    this->glCameraView = new GLCameraView(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, "Camera");
    this->glFrontTireView = new GLFrontTireView(0, IMAGE_HEIGHT + 20, IMAGE_WIDTH, IMAGE_HEIGHT, "Front Tire");
    this->glRearTireView = new GLRearTireView(IMAGE_WIDTH + 10, IMAGE_HEIGHT + 20, IMAGE_WIDTH, IMAGE_HEIGHT, "Rear Tire");

    this->glCameraView->setThreadData(_threadData);
}

GLWindow::~GLWindow() {
    delete this->glCameraView;
    delete this->glFrontTireView;
    delete this->glRearTireView;
}

void GLWindow::showWindow() {
    LOG("Start");
    this->glCameraView->initialize();
    this->glCameraView->setCallback();
    this->glFrontTireView->initialize();
    this->glFrontTireView->setCallback();
    this->glRearTireView->initialize();
    this->glRearTireView->setCallback();
    glutMainLoop();
}

void GLWindow::setFrontTireImage(IplImage* _image) {
    this->glFrontTireView->setImage(_image);
}

void GLWindow::setRearTireImage(IplImage* _image) {
    this->glRearTireView->setImage(_image);
}