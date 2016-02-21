#include "Common.h"
#include "XIQCamera.h"
#include "GLWindow.h"
#include "CRSignal.h"

ThreadData* volatile threadData;
GLWindow* volatile glWindow;
CRSignal* volatile crSignal;

float frameRate;
int imageWidth;
int imageHeight;
string serverHost;
int serverPort;
bool debug;
bool method;
int bufferSize;

template <typename Target, typename Source>
Target lexical_cast(const Source& arg) {
    std::stringstream ss;
    Target result;
    ss << arg;
    ss >> result;
    return result;
}

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

    int state = 0, beforeState = 0;
    IplImage* base = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    IplImage* baseGray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    IplImage* curr = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    IplImage* currGray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    IplImage* diff = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
    
    double max = 0;
    IplImage* optimal = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);

    int idxTire = 0;

    int currentFrame = 0;
    int baseFrame = 0;
    int optimalFrame = 0;
    int endFrame = 0;
    
    int numCase = 0;
    string debugPath;
    chrono::system_clock::time_point now;
    time_t nowtime;

    using namespace std::placeholders;

    auto cvXorWithoutMask = [](const CvArr *a, const CvArr *b, CvArr *c) {
        cvXor(a, b, c);
    };
    auto diffMethod = std::bind((method ? cvAbsDiff : cvXorWithoutMask), _1, _2, _3);
    
    while (true) {
        state = crSignal->getState();
        currentFrame = threadData->lastSetFrame;
        switch (state) {
        case States::TIRE_IN:
            if (beforeState != state) {
                baseFrame = currentFrame;
                
                cvCopy(threadData->getBuffer(), base);
                cvCvtColor(base, baseGray, CV_RGB2GRAY);

                if (DEBUG) {
                    string type = (idxTire % 2 == 0 ? "front" : "rear");
                    LOG("Start tire recognization : %s", type.c_str());
                    LOG("Base frame %d", currentFrame);
                    now = chrono::system_clock::now();
                    nowtime = chrono::system_clock::to_time_t(now);
                    debugPath = "./log/" + lexical_cast<string, time_t>(nowtime)+type;
                    _mkdir(debugPath.c_str());
                    string filename = debugPath + "/" + lexical_cast<string, int>(currentFrame)+".bmp";
                    cvSaveImage(filename.c_str(), base);
                }
            }
            else {
                endFrame = currentFrame;
                cvCopy(threadData->getBuffer(), curr);
                cvCvtColor(curr, currGray, CV_RGB2GRAY);

                diffMethod(baseGray, currGray, diff);

                CvScalar sum = cvSum(diff);
                if (sum.val[0] > max) {
                    max = sum.val[0];
                    cvCopy(curr, optimal);
                    optimalFrame = currentFrame;
                }
                if (DEBUG) {
                    string filename = debugPath + "/" + lexical_cast<string, int>(currentFrame)+".bmp";
                    cvSaveImage(filename.c_str(), curr);
                }
            }
            break;
        case States::TIRE_OUT:
            if (beforeState != state) {
                if (idxTire == 0) {
                    glWindow->setFrontTireImage(optimal);
                }
                else {
                    glWindow->setRearTireImage(optimal);
                }

                max = 0;

                if (DEBUG) {
                    string type = (idxTire % 2 == 0 ? "front" : "rear");
                    LOG("End tire recognization : %s", type.c_str());
                    LOG("End frame %d", endFrame);
                    LOG("Optimal frame %d", optimalFrame);
                    string filename = debugPath + "/info.txt";
                    ofstream fout(filename.c_str());
                    fout << "method : " << (method ? "diff" : "xor") << endl;
                    fout << "start frame : " << baseFrame << endl;
                    fout << "end frame : " << endFrame << endl;
                    fout << "optimal frame : " << optimalFrame << endl;
                    fout.close();
                    filename = debugPath + "/optimal.bmp";
                    cvSaveImage(filename.c_str(), optimal);
                }

                idxTire++;
            }
            break;
        }
        beforeState = state;
    }
    cvReleaseImage(&base);
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