#include "GLCameraView.h"

void GLCameraView::initialize(int _offsetX, int _offsetY, int _width, int _height, string _title, ThreadData* _threadData) {
	GLCameraView::offsetX = _offsetX;
	GLCameraView::offsetY = _offsetY;
	GLCameraView::width = _width;
	GLCameraView::height = _height;
	GLCameraView::title = _title;
    GLCameraView::threadData = _threadData;

    glutInitWindowPosition(GLCameraView::offsetX, GLCameraView::offsetY);
    glutInitWindowSize(GLCameraView::width, GLCameraView::height);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    GLCameraView::windowId = glutCreateWindow(GLCameraView::title.c_str());

    glEnable(GL_DEPTH_TEST);
    
    GLCameraView::image = cvCreateImage(cvSize(GLCameraView::width, GLCameraView::height), IPL_DEPTH_8U, 3);
    cvSet(GLCameraView::image, cvScalar(0, 0, 0));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GLCameraView::width, GLCameraView::height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, GLCameraView::image->imageData);

    glutDisplayFunc(GLCameraView::display);
    glutTimerFunc(1000 / 30, GLCameraView::timer, 0);
    glutKeyboardFunc(GLCameraView::keyboard);
}

void GLCameraView::loadImage(IplImage* _image) {
    cvCopy(_image, GLCameraView::image);
    glTexSubImage2D(GL_TEXTURE_2D,
        0,
        0, 0,
        GLCameraView::width, GLCameraView::height,
        GL_BGR_EXT, GL_UNSIGNED_BYTE,
        GLCameraView::image->imageData);
}

void GLCameraView::display() {
    if (GLCameraView::threadData->startCapture) {
        GLCameraView::loadImage(threadData->getBuffer());
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(-1, -1, 0);
    glTexCoord2f(1, 1); glVertex3f(1, -1, 0);
    glTexCoord2f(1, 0); glVertex3f(1, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, 1, 0);
    glEnd();

    glPopAttrib();
    glPopMatrix();

    glutSwapBuffers();
}

void GLCameraView::timer(int value) {
    glutSetWindow(GLCameraView::windowId);
    glutPostRedisplay();
    glutTimerFunc(1000 / REFRESH_RATE, GLCameraView::timer, 0);
}

void GLCameraView::keyboard(uchar key, int x, int y) {
    switch (key) {
    case ',':
        cout << "1" << endl;
        break;
    case '.':
        cout << "2" << endl;
        break;
    }
}