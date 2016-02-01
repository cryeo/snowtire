#pragma once

#include "Common.h"

template<class T>
class GLView {
public:
	static int offsetX;
	static int offsetY;
	static int width;
	static int height;
	static string title;
	static GLuint windowId;
	static IplImage* image;
    static ThreadData* threadData;
};

template<class T>
int GLView<T>::offsetX;
template<class T>
int GLView<T>::offsetY;
template<class T>
int GLView<T>::width;
template<class T>
int GLView<T>::height;
template<class T>
string GLView<T>::title;
template<class T>
GLuint GLView<T>::windowId;
template<class T>
IplImage* GLView<T>::image;
template<class T>
ThreadData* GLView<T>::threadData;
