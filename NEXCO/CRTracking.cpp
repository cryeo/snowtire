#include "CRTracking.h"
#include "Global.h"
#include "Timer.h"
#include <direct.h>

auto makeLogPath = [](std::string time, std::string type) {
    std::stringstream ss;
    ss << "./log/" << time << type;
    return ss.str();
};


template<typename T>
std::string makeFileName(std::string base, T name, std::string ext) {
    std::stringstream ss;
    ss << base << "/" << name << "." << ext;
    return ss.str();
}

void CRTracking::trace() {
    int state = 0, beforeState = 0;

    double max = 0;

    int idxTire = 0;

    int currentFrame = 0;
    int startFrame = 0;
    int optimalFrame = 0;
    int endFrame = 0;

    int numCase = 0;

    std::string logPath;

    while (true) {
        state = Global::crSignal->getState();
        currentFrame = Global::cameraBuffer->lastSetFrame;

        switch (state) {
        case States::TIRE_IN:
            if (beforeState != state) {
                startFrame = currentFrame;

                cvCopy(Global::cameraBuffer->getBuffer(), base);
                cvCvtColor(base, baseGray, CV_RGB2GRAY);

                if (Global::crConfig->debug) {
                    std::string type = (idxTire % 2 == 0 ? "front" : "rear");
                    LOG("Start tire recognization : %s", type.c_str());
                    LOG("Base frame %d", currentFrame);

                    logPath = makeLogPath(Timer::now(), type);
                    _mkdir(logPath.c_str());
                    cvSaveImage(makeFileName(logPath, currentFrame, ".bmp").c_str(), base);
                }
            }
            else {
                endFrame = currentFrame;
                cvCopy(Global::cameraBuffer->getBuffer(), curr);
                cvCvtColor(curr, currGray, CV_RGB2GRAY);

                diffMethod(baseGray, currGray, diff);

                CvScalar sum = cvSum(diff);
                if (sum.val[0] > max) {
                    max = sum.val[0];
                    cvCopy(curr, optimal);
                    optimalFrame = currentFrame;
                }

                if (Global::crConfig->debug) {
                    cvSaveImage(makeFileName(logPath, currentFrame, ".bmp").c_str(), curr);
                }
            }
            break;

        case States::TIRE_OUT:
            if (beforeState != state) {
                if (idxTire == 0) {
                    Global::glWindow->setFrontTireImage(optimal);
                }
                else {
                    Global::glWindow->setRearTireImage(optimal);
                }

                max = 0;

                if (Global::crConfig->debug) {
                    std::string type = (idxTire % 2 == 0 ? "front" : "rear");
                    LOG("End tire recognization : %s", type.c_str());
                    LOG("End frame %d", endFrame);
                    LOG("Optimal frame %d", optimalFrame);
                    std::ofstream fout(makeFileName(logPath, "info", "txt").c_str());
                    fout << "method : " << Global::crConfig->method << std::endl;
                    fout << "start frame : " << startFrame << std::endl;
                    fout << "end frame : " << endFrame << std::endl;
                    fout << "optimal frame : " << optimalFrame << std::endl;
                    fout.close();
                    cvSaveImage(makeFileName(logPath, "optimal", ".bmp").c_str(), optimal);
                }

                idxTire++;
            }
            break;
        }
        beforeState = state;
    }
}
