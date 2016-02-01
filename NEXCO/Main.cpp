#include "Common.h"
#include "XIQCamera.h"
#include "GLWindow.h"

using namespace std;

ThreadData* threadData;
GLWindow* glWindow;

void threadCamera(ThreadData* threadData) {
    Logger::log(__FUNCTION__, "Start");

	XI_IMG image;
	HANDLE xiH = NULL;
	DWORD dwNumberOfDevices = 0;
	float maxFps;

	xiGetNumberDevices(&dwNumberOfDevices);
	if (!dwNumberOfDevices) {
        Logger::log(__FUNCTION__, "Camera was NOT found.");
	}

	xiOpenDevice(0, &xiH);

	int exposure = (int)(1000000.0 / XI_FRAMERATE);
	//xiSetParamInt(xiH, XI_PRM_TRG_SOURCE, XI_TRG_OFF);						// for trigger mode
	xiSetParamInt(xiH, XI_PRM_EXPOSURE, exposure);
	xiGetParamFloat(xiH, XI_PRM_FRAMERATE XI_PRM_INFO_MAX, &maxFps);
    Logger::log(__FUNCTION__, "Max framerate : %f [fps]", maxFps);
    Logger::log(__FUNCTION__, "Exposure time : %d [us]", exposure);

	xiSetParamInt(xiH, XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FRAME_RATE);	// if you use the trigger mode, you have to change the acquisition timing mode
	xiSetParamFloat(xiH, XI_PRM_FRAMERATE, XI_FRAMERATE);
	//xiSetParamInt(xiH, XI_PRM_GPO_SELECTOR, 1);								// for trigger mode
	//xiSetParamInt(xiH, XI_PRM_GPO_MODE, XI_GPO_EXPOSURE_PULSE);				// for trigger mode
	xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RGB24);

	xiStartAcquisition(xiH);

    xiGetImage(xiH, 5000, &image);

    IplImage* capture = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	while (true) {
        xiGetImage(xiH, 5000, &image);
		memcpy(capture->imageData, image.bp, IMAGE_WIDTH * IMAGE_HEIGHT * 3);
        threadData->setBuffer(capture);
        threadData->startCapture = true;
        //Logger::log(__FUNCTION__, "%d %d", threadData->lastSetFrame, threadData->lastGetFrame);
	}

    threadData->startCapture = false;
    cvReleaseImage(&capture);
	xiCloseDevice(xiH);
}

void threadOpenGL(int argc, char** argv, ThreadData *threadData) {
    Logger::log(__FUNCTION__, "Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    Logger::log(__FUNCTION__, "Start");

    glWindow = new GLWindow(argc, argv, threadData);
    glWindow->showWindow();

    delete glWindow;
}

void threadTracking(ThreadData *threadData) {
    Logger::log(__FUNCTION__, "Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    Logger::log(__FUNCTION__, "Start");

    int state = 0, beforeState = 0;
    IplImage* base = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    IplImage* baseGray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    IplImage* curr = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    IplImage* currGray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    IplImage* diff = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    
    int max = 0;
    IplImage* optimal = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);

    while (true) {
        state = threadData->state->getState();
        if (state == States::TIRE_IN) {
            if (beforeState != state) {
                cvCopy(threadData->getBuffer(), base);
                cvCvtColor(base, baseGray, CV_RGB2GRAY);
            } 
            else {
                cvCopy(threadData->getBuffer(), curr);
                cvCvtColor(curr, currGray, CV_RGB2GRAY);
                cvAbsDiff(baseGray, currGray, diff);
                CvScalar sum = cvSum(diff);
                if (sum.val[0] > max) {
                    max = sum.val[0];
                    cvCopy(curr, optimal);
                }
            }
        }
        else if (state == States::TIRE_OUT) {
            if (beforeState != state) {
                cvSaveImage("c:\\data\\1.bmp", optimal);
                glWindow->setFrontTireImage(optimal);
                max = 0;
            }
        }
        beforeState = state;
    }
    cvReleaseImage(&base);
}

void threadSignal(ThreadData *threadData) {
    Logger::log(__FUNCTION__, "Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    Logger::log(__FUNCTION__, "Start");

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("133.11.191.233");
    address.sin_port = htons(6307);
    connect(s, (sockaddr*)&address, sizeof(address));

    Logger::log(__FUNCTION__, "Connection estabilished");

    int res;
    char buf[255] = "\0";
    while (true) {
        res = recv(s, buf, 255, 0);
        if (res > 0) {
            if (strlen(buf) > 0) {
                threadData->state->transit(buf[0]);
            }
        }
        else if (res == 0) {
            Logger::log(__FUNCTION__, "Connection closed");
            break;
        }
        else {
            Logger::log(__FUNCTION__, "recv failed: %d", WSAGetLastError());
            break;
        }
    }
}

int main(int argc, char** argv) {
    threadData = new ThreadData();

    vector<thread> threads;
    threads.push_back(thread(threadCamera, threadData));
    threads.push_back(thread(threadOpenGL, argc, argv, threadData));
    threads.push_back(thread(threadSignal, threadData));
    threads.push_back(thread(threadTracking, threadData));

    for (auto &thread : threads) {
		thread.join();
		cout << "thread ok";
	}

    delete threadData;
	return 0;
}