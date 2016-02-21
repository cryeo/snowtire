#include "Common.h"
#include "GLWindow.h"
#include "CRSignal.h"
#include "CRTracking.h"
#include "CRCamera.h"

ThreadData* volatile threadData;
GLWindow* volatile glWindow;
CRSignal* volatile crSignal;
CRTracking* volatile crTracking;
CRCamera* volatile crCamera;

float frameRate;
int imageWidth;
int imageHeight;
std::string serverHost;
int serverPort;
bool debug;
bool method;
int bufferSize;

void loadConfig() {
    LOG("Start");

    std::ifstream fin("config.txt");
    std::string name, value;
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

    LOG("End");
}

void initialize(int argc, char** argv) {
    LOG("Start");

    threadData = new ThreadData(bufferSize);
    glWindow = new GLWindow(argc, argv, threadData);
    crSignal = new CRSignal();
    crTracking = new CRTracking(cvAbsDiff, threadData, crSignal, glWindow);
    crCamera = new CRCamera();

    auto cvXorWithoutMask = [](const CvArr *src1, const CvArr *src2, CvArr *dst) {
        cvXor(src1, src2, dst);
    };

    crCamera->initialize();
    crCamera->startAcquisition();

    LOG("End");
}

void threadCamera() {
    LOG("Start");
    XI_IMG capturedImage;
    IplImage* image = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    while (true) {
        crCamera->capture(capturedImage);
        memcpy(image->imageData, capturedImage.bp, IMAGE_WIDTH * IMAGE_HEIGHT * 3);
        threadData->setBuffer(image);
    }
    cvReleaseImage(&image);
    crCamera->stopAcquisition();
    LOG("End");
}

void threadOpenGL() {
    LOG("Start");
    glWindow->showWindow();
    LOG("End");
}

void threadTracking() {
    LOG("Start");
    crTracking->trace();
    LOG("End");
}

void threadSignal() {
    LOG("Start");
    crSignal->listen();
    LOG("End");
}

int main(int argc, char** argv) {
    loadConfig();
    initialize(argc, argv);

    std::vector<std::thread> threads;
    threads.push_back(std::thread(threadCamera));
    threads.push_back(std::thread(threadOpenGL));
    threads.push_back(std::thread(threadTracking));
    threads.push_back(std::thread(threadSignal));

    for (auto &thread : threads) {
		thread.join();
	}

    delete glWindow;
    delete threadData;
    delete crSignal;
    delete crTracking;
    delete crCamera;

	return 0;
}