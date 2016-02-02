#include "GLFrontTireView.h"

void GLFrontTireView::initialize() {
    GLView<GLFrontTireView>::initialize();

    glEnable(GL_DEPTH_TEST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, this->image->imageData);
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

void GLFrontTireView::setCallback() {
    glutDisplayFunc(GLFrontTireView::displayCallback);
    glutTimerFunc(1000 / REFRESH_RATE, GLFrontTireView::timerCallback, 0);
}
