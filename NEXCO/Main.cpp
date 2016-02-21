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
}

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

    Global::cameraBuffer = new CameraBuffer(bufferSize);
    Global::glWindow = new GLWindow(argc, argv);
    Global::crSignal = new CRSignal();
    Global::crTracking = new CRTracking(cvAbsDiff);
    Global::crCamera = new CRCamera();

    auto cvXorWithoutMask = [](const CvArr *src1, const CvArr *src2, CvArr *dst) {
        cvXor(src1, src2, dst);
    };

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
    delete Global::cameraBuffer;
    delete Global::crSignal;
    delete Global::crTracking;
    delete Global::crCamera;

	return 0;
}