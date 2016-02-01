#include "GLWindow.h"
#include "GLCameraView.h"
#include "GLFrontTireView.h"

GLWindow::GLWindow(int argc, char** argv, ThreadData* _threadData) : threadData(_threadData) {
	glutInit(&argc, argv);
}

GLWindow::~GLWindow() {
}

void GLWindow::showWindow() {
    Logger::log(__FUNCTION__, "Start");
    GLCameraView::initialize(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, "Camera", this->threadData);
    GLFrontTireView::initialize(0, IMAGE_HEIGHT + 20, IMAGE_WIDTH, IMAGE_HEIGHT, "Front Tire", this->threadData);
    //GLFrontTireView::initialize(IMAGE_WIDTH + 15, IMAGE_HEIGHT + 20, IMAGE_WIDTH, IMAGE_HEIGHT, "Rear Tire", this->threadData);
    glutMainLoop();
}

void GLWindow::setCameraImage(IplImage* image) {

}

void GLWindow::setFrontTireImage(IplImage* image) {
    GLFrontTireView::loadImage(image);
}