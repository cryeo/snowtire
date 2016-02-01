#include "GLFrontTireView.h"

void GLFrontTireView::initialize(int _offsetX, int _offsetY, int _width, int _height, string _title, ThreadData* _threadData) {
    GLFrontTireView::offsetX = _offsetX;
    GLFrontTireView::offsetY = _offsetY;
    GLFrontTireView::width = _width;
    GLFrontTireView::height = _height;
    GLFrontTireView::title = _title;
    GLFrontTireView::threadData = _threadData;

    glutInitWindowPosition(GLFrontTireView::offsetX, GLFrontTireView::offsetY);
    glutInitWindowSize(GLFrontTireView::width, GLFrontTireView::height);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    GLFrontTireView::windowId = glutCreateWindow(GLFrontTireView::title.c_str());

    glEnable(GL_DEPTH_TEST);

    GLFrontTireView::image = cvCreateImage(cvSize(GLFrontTireView::width, GLFrontTireView::height), IPL_DEPTH_8U, 3);
    cvSet(GLFrontTireView::image, cvScalar(0, 0, 0));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GLFrontTireView::width, GLFrontTireView::height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, GLFrontTireView::image->imageData);

    glutDisplayFunc(GLFrontTireView::display);
    glutTimerFunc(1000 / 30, GLFrontTireView::timer, 0);
}

void GLFrontTireView::loadImage(IplImage* _image) {
    cvCopy(_image, GLFrontTireView::image);
}

void GLFrontTireView::display() {
    glTexSubImage2D(GL_TEXTURE_2D,
        0,
        0, 0,
        GLFrontTireView::width, GLFrontTireView::height,
        GL_BGR_EXT, GL_UNSIGNED_BYTE,
        GLFrontTireView::image->imageData);

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

void GLFrontTireView::timer(int value) {
    glutSetWindow(GLFrontTireView::windowId);
    glutPostRedisplay();
    glutTimerFunc(1000 / REFRESH_RATE, GLFrontTireView::timer, 0);
}
