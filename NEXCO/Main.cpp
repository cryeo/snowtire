#include "Common.h"
#include "XIQCamera.h"
#include "GLWindow.h"

using namespace std;

ThreadData* threadData;
GLWindow* glWindow;

void threadCamera(ThreadData* threadData, GLWindow* glWindow) {
    LOG("Start");

	XI_IMG image;
	HANDLE xiH = NULL;
	DWORD dwNumberOfDevices = 0;
	float maxFps;

	xiGetNumberDevices(&dwNumberOfDevices);
	if (!dwNumberOfDevices) {
        LOG("Camera was NOT found.");
	}

	xiOpenDevice(0, &xiH);

	int exposure = (int)(1000000.0 / XI_FRAMERATE);
	//xiSetParamInt(xiH, XI_PRM_TRG_SOURCE, XI_TRG_OFF);						// for trigger mode
	xiSetParamInt(xiH, XI_PRM_EXPOSURE, exposure);
	xiGetParamFloat(xiH, XI_PRM_FRAMERATE XI_PRM_INFO_MAX, &maxFps);
    LOG("Max framerate : %f [fps]", maxFps);
    LOG("Exposure time : %d [us]", exposure);

	xiSetParamInt(xiH, XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FRAME_RATE);	// if you use the trigger mode, you have to change the acquisition timing mode
	xiSetParamFloat(xiH, XI_PRM_FRAMERATE, XI_FRAMERATE);
	//xiSetParamInt(xiH, XI_PRM_GPO_SELECTOR, 1);								// for trigger mode
	//xiSetParamInt(xiH, XI_PRM_GPO_MODE, XI_GPO_EXPOSURE_PULSE);				// for trigger mode
	xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RGB24);

	xiStartAcquisition(xiH);

    xiGetImage(xiH, 5000, &image);
    threadData->startCapture = true;

    IplImage* capture = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	while (true) {
        xiGetImage(xiH, 5000, &image);
		memcpy(capture->imageData, image.bp, IMAGE_WIDTH * IMAGE_HEIGHT * 3);
        threadData->setBuffer(capture);
        //LOG("%d %d", threadData->lastSetFrame, threadData->lastGetFrame);
	}

    threadData->startCapture = false;
    cvReleaseImage(&capture);
	xiCloseDevice(xiH);
}

void threadOpenGL(ThreadData* threadData, GLWindow* glWindow) {
    LOG("Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    LOG("Start");

    glWindow->showWindow();
}

void threadTracking(ThreadData* threadData, GLWindow* glWindow) {
    LOG("Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    LOG("Start");

    int state = 0, beforeState = 0;
    IplImage* base = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    IplImage* baseGray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    IplImage* curr = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    IplImage* currGray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    IplImage* diff = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    
    double max = 0;
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
    LOG("Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    LOG("Start");

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        LOG("WSAStartup failed");
        exit(-1);
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        LOG("Invalid Socket. Error code : %d", WSAGetLastError());
        exit(-1);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("133.11.191.233");
    address.sin_port = htons(6307);

    auto connection = [&sock](sockaddr_in &address) {
        LOG("Connecting");
        while (connect(sock, (sockaddr*)&address, sizeof(address)) != 0);
        LOG("Connection estabilished");
    };

    connection(address);

    int res;
    char buf[255] = "\0";
    while (true) {
        res = recv(sock, buf, 255, 0);
        if (res > 0) {
            if (strlen(buf) > 0) {
                threadData->state->transit(buf[0]);
            }
        }
        else if (res == 0) {
            LOG("Connection closed. reconnect");
            connection(address);
            break;
        }
        else {
            int err = WSAGetLastError();
            LOG("Receive failed: Error code : %d", err);
            switch (err) {
            case WSAENOTCONN:
                connection(address);
                break;
            default:
                exit(-1);
            }
            break;
        }
    }
}

int main(int argc, char** argv) {
    threadData = new ThreadData();
    glWindow = new GLWindow(argc, argv, threadData);

    vector<thread> threads;
    threads.push_back(thread(threadCamera, threadData, glWindow));
    threads.push_back(thread(threadOpenGL, threadData, glWindow));
    threads.push_back(thread(threadTracking, threadData, glWindow));
    threads.push_back(thread(threadSignal, threadData));

    for (auto &thread : threads) {
		thread.join();
		cout << "thread ok";
	}

    delete glWindow;
    delete threadData;
	return 0;
}