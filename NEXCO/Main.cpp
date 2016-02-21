#include "Common.h"
#include "XIQCamera.h"
#include "GLWindow.h"
#include "CRSignal.h"
#include "CRTracking.h"

ThreadData* volatile threadData;
GLWindow* volatile glWindow;
CRSignal* volatile crSignal;
CRTracking* volatile crTracking;

float frameRate;
int imageWidth;
int imageHeight;
string serverHost;
int serverPort;
bool debug;
bool method;
int bufferSize;

void initialize() {
    ifstream fin("config.txt");
    string name, value;
    while (!fin.eof()) {
        fin >> name >> value;
        if (name == "framerate") {
            frameRate = lexical_cast<float>(value);
        }
        else if (name == "host") {
            serverHost = value;
        }
        else if (name == "port") {
            serverPort = lexical_cast<int>(value);
        }
        else if (name == "buffer") {
            bufferSize = lexical_cast<int>(value);
        }
        else if (name == "debug") {
            if (value == "true") {
                debug = true;
            }
            else if (value == "false") {
                debug = false;
            }
        }
        else if (name == "method") {
            if (value == "diff") {
                method = true;
            }
            else if (value == "xor") {
                method = false;
            }
        }
    }
    fin.close();
}

void threadCamera() {
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
	xiSetParamInt(xiH, XI_PRM_EXPOSURE, exposure);
	xiGetParamFloat(xiH, XI_PRM_FRAMERATE XI_PRM_INFO_MAX, &maxFps);
    LOG("Max framerate : %f [fps]", maxFps);
    LOG("Exposure time : %d [us]", exposure);

	xiSetParamInt(xiH, XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FRAME_RATE);
	xiSetParamFloat(xiH, XI_PRM_FRAMERATE, XI_FRAMERATE);
	xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RGB24);

	xiStartAcquisition(xiH);

    xiGetImage(xiH, 5000, &image);
    threadData->startCapture = true;

    IplImage* capture = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	while (true) {
        xiGetImage(xiH, 5000, &image);
		memcpy(capture->imageData, image.bp, IMAGE_WIDTH * IMAGE_HEIGHT * 3);
        threadData->setBuffer(capture);
        _CrtDumpMemoryLeaks();
	}

    threadData->startCapture = false;
    cvReleaseImage(&capture);
	xiCloseDevice(xiH);
}

void threadOpenGL() {
    LOG("Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    LOG("Start");

    glWindow->showWindow();
}

void threadTracking() {
    LOG("Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    LOG("Start");

    crTracking->trace();
}

void threadSignal() {
    LOG("Waiting until camera is ready to capture");
    while (!threadData->startCapture);
    LOG("Start");

    crSignal->listen();
}

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    initialize();

    threadData = new ThreadData(bufferSize);
    glWindow = new GLWindow(argc, argv, threadData);
    crSignal = new CRSignal();
    crTracking = new CRTracking(cvAbsDiff, threadData, crSignal, glWindow);

    auto cvXorWithoutMask = [](const CvArr *src1, const CvArr *src2, CvArr *dst) {
        cvXor(src1, src2, dst);
    };


    vector<thread> threads;
    threads.push_back(thread(threadCamera));
    threads.push_back(thread(threadOpenGL));
    threads.push_back(thread(threadTracking));
    threads.push_back(thread(threadSignal));

    for (auto &thread : threads) {
		thread.join();
	}

    delete glWindow;
    delete threadData;
	return 0;
}