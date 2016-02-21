#include "Common.h"
#include "Global.h"

float frameRate;
int imageWidth;
int imageHeight;
std::string serverHost;
int serverPort;
bool debug;
bool method;
int bufferSize;

namespace Global {
    CameraBuffer* volatile cameraBuffer;
    GLWindow* volatile glWindow;
    CRSignal* volatile crSignal;
    CRTracking* volatile crTracking;
    CRCamera* volatile crCamera;
    CRConfig* volatile crConfig;
}

void loadConfig() {
    LOG("Start");
    Global::crConfig = new CRConfig("config.yaml");
    LOG("End");
}

void initialize(int argc, char** argv) {
    LOG("Start");

    auto getTrackingMethod = [](std::string method) -> std::function<void(const CvArr *, const CvArr *, CvArr *)> {
        if (method == "xor") return static_cast<void(&)(const CvArr *, const CvArr *, CvArr *)>(cvXor);
        return cvAbsDiff;
    };

    Global::cameraBuffer = new CameraBuffer(Global::crConfig->bufferSize);
    Global::glWindow = new GLWindow(argc, argv);
    Global::crSignal = new CRSignal();

    Global::crTracking = new CRTracking(getTrackingMethod(Global::crConfig->method));

    Global::crCamera = new CRCamera();


    Global::crCamera->initialize();
    Global::crCamera->startAcquisition();

    LOG("End");
}

void threadCamera() {
    LOG("Start");
    XI_IMG capturedImage;
    IplImage* image = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
    while (true) {
        Global::crCamera->capture(capturedImage);
        memcpy(image->imageData, capturedImage.bp, IMAGE_WIDTH * IMAGE_HEIGHT * 3);
        Global::cameraBuffer->setBuffer(image);
    }
    cvReleaseImage(&image);
    Global::crCamera->stopAcquisition();
    LOG("End");
}

void threadOpenGL() {
    LOG("Start");
    Global::glWindow->showWindow();
    LOG("End");
}

void threadTracking() {
    LOG("Start");
    Global::crTracking->trace();
    LOG("End");
}

void threadSignal() {
    LOG("Start");
    Global::crSignal->listen();
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

    delete Global::glWindow;
    delete Global::crConfig;
    delete Global::cameraBuffer;
    delete Global::crSignal;
    delete Global::crTracking;
    delete Global::crCamera;

	return 0;
}