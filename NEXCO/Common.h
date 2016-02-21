#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4819) // unicode
#pragma once

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <future>
#include <initializer_list>
#include <sys/types.h> 
#include <cstring>
#include <cerrno>
#include <memory>
#include <conio.h>
#include <direct.h>

#include "OpenCV.h"
#include "Socket.h"
#include "Logger.h"

extern float frameRate;
extern std::string serverHost;
extern int serverPort;
extern bool debug;
extern bool method;
extern int bufferSize;

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

template <typename Target, typename Source>
Target lexical_cast(const Source& arg) {
    std::stringstream ss;
    Target result;
    ss << arg;
    ss >> result;
    return result;
}