#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4819) // unicode
#pragma once

#include <cv.h>
#include <opencv2/opencv.hpp>
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
#include <WinSock2.h>

using namespace std;

#ifdef WIN32
	#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)

	#ifdef _DEBUG
		#define CV_PATH "C:\\opencv2411\\build\\x64\\vc12\\staticlib\\"
		#define CV_EXT_STR "d.lib"
	#else
        #define CV_PATH "C:\\opencv2411\\build\\x64\\vc12\\staticlib\\"
		#define CV_EXT_STR ".lib"
	#endif

#pragma comment(lib, CV_PATH "IlmImf"	CV_EXT_STR)
#pragma comment(lib, CV_PATH "libjasper"	CV_EXT_STR)
#pragma comment(lib, CV_PATH "libjpeg"	CV_EXT_STR)
#pragma comment(lib, CV_PATH "libpng"	CV_EXT_STR)
#pragma comment(lib, CV_PATH "libtiff"	CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_calib3d" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_contrib" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_core" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_features2d" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_flann" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_gpu" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_highgui" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_imgproc" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_legacy" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_ml" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_nonfree" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_objdetect" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_ocl" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_photo" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_stitching" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_superres" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_ts" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_video" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "opencv_videostab" CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, CV_PATH "zlib"	CV_EXT_STR)

#pragma comment(lib, "ws2_32.lib")
#endif

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

enum States {
    INITIAL = 0,
    TIRE_IN = 1,
    TIRE_OUT = 2
};

class State {
public:

    State() : _state(States::INITIAL) {
        this->transition[States::INITIAL][0] = States::TIRE_IN;
        this->transition[States::INITIAL][1] = States::INITIAL;
        this->transition[States::TIRE_IN][0] = States::INITIAL;
        this->transition[States::TIRE_IN][1] = States::TIRE_OUT;
        this->transition[States::TIRE_OUT][0] = States::TIRE_IN;
        this->transition[States::TIRE_OUT][1] = States::INITIAL;
    }

    inline void transit(char _action) {
        // f : 0, r : 1
        int action = (_action == 'f') ? 0 : 1;
        this->_state = this->transition[this->_state][action];
    }

    inline States getState() {
        return this->_state;
    }

private:
    States transition[3][2];
    States _state;
};

class ThreadData {
public:
    ThreadData() : ThreadData(500) {}
    ThreadData(int _numBuffer) : numBuffer(_numBuffer) {
        this->state = new State();
        this->lastSetFrame = -1;
        this->lastGetFrame = -1;
        this->startCapture = false;

        this->buffer = new IplImage*[this->numBuffer];
        for (int i = 0; i < this->numBuffer; i++) {
            this->buffer[i] = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
        }
    }

    virtual ~ThreadData() {
        for (int i = 0; i < this->numBuffer; i++) {
            cvReleaseImage(&this->buffer[i]);
        }
        delete this->buffer;
        delete this->state;
    }

    inline void setBuffer(IplImage* image) {
        cvCopy(image, this->buffer[++this->lastSetFrame % this->numBuffer]);
    }

    inline IplImage* getBuffer() {
        return buffer[this->lastSetFrame % this->numBuffer];
    }

    IplImage** buffer;
    State* state;
    int numBuffer;
    int lastGetFrame;
    int lastSetFrame;
    bool startCapture;

    class Signal {
    public:
        Signal() : signal(false) {}
        ~Signal() {}

        inline void flipSignal() {
            this->signal = !this->signal;
        }

        inline void setSignal(bool _signal) {
            this->signal = _signal;
        }

        inline bool getSignal() {
            return this->signal;
        }
        bool signal;
    };
};


namespace Logger2 {
    template<typename...Args>
    struct Log;

    template<typename T, typename...Args>
    struct Log<T, Args...>  {
        void operator()(T head, Args... tail) {
            cout << " " << head;
            Log<Args...>{}(tail...);
        }
    };

    template<>
    struct Log<> {
        void operator()() {
            cout << endl;
        }
    };

    template<typename T, typename... Args>
    void log(T caller, Args... args) {
        cout << "[" << caller << "]";
        Log<Args...>{}(args...);
    }
};

namespace Logger {
    template<typename T, typename... Args>
    inline void log(T caller, Args... args) {
        printf("[%s] ", caller);
        printf(args...);
        printf("\n");
    }
};