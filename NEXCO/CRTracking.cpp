#include "CRTracking.h"

void CRTracking::trace() {
    int state = 0, beforeState = 0;

    double max = 0;

    int idxTire = 0;

    int currentFrame = 0;
    int baseFrame = 0;
    int optimalFrame = 0;
    int endFrame = 0;

    int numCase = 0;
    std::string debugPath;
    std::chrono::system_clock::time_point now;
    time_t nowtime;

    while (true) {
        state = this->crSignal->getState();
        currentFrame = this->cameraBuffer->lastSetFrame;

        switch (state) {
        case States::TIRE_IN:
            if (beforeState != state) {
                baseFrame = currentFrame;

                cvCopy(this->cameraBuffer->getBuffer(), base);
                cvCvtColor(base, baseGray, CV_RGB2GRAY);

                if (DEBUG) {
                    std::string type = (idxTire % 2 == 0 ? "front" : "rear");
                    LOG("Start tire recognization : %s", type.c_str());
                    LOG("Base frame %d", currentFrame);
                    now = std::chrono::system_clock::now();
                    nowtime = std::chrono::system_clock::to_time_t(now);
                    debugPath = "./log/" + lexical_cast<std::string, time_t>(nowtime)+type;
                    _mkdir(debugPath.c_str());
                    std::string filename = debugPath + "/" + lexical_cast<std::string, int>(currentFrame)+".bmp";
                    cvSaveImage(filename.c_str(), base);
                }
            }
            else {
                endFrame = currentFrame;
                cvCopy(this->cameraBuffer->getBuffer(), curr);
                cvCvtColor(curr, currGray, CV_RGB2GRAY);

                diffMethod(baseGray, currGray, diff);

                CvScalar sum = cvSum(diff);
                if (sum.val[0] > max) {
                    max = sum.val[0];
                    cvCopy(curr, optimal);
                    optimalFrame = currentFrame;
                }
                if (DEBUG) {
                    std::string filename = debugPath + "/" + lexical_cast<std::string, int>(currentFrame)+".bmp";
                    cvSaveImage(filename.c_str(), curr);
                }
            }
            break;

        case States::TIRE_OUT:
            if (beforeState != state) {
                if (idxTire == 0) {
                    this->glWindow->setFrontTireImage(optimal);
                }
                else {
                    this->glWindow->setRearTireImage(optimal);
                }

                max = 0;

                if (DEBUG) {
                    std::string type = (idxTire % 2 == 0 ? "front" : "rear");
                    LOG("End tire recognization : %s", type.c_str());
                    LOG("End frame %d", endFrame);
                    LOG("Optimal frame %d", optimalFrame);
                    std::string filename = debugPath + "/info.txt";
                    std::ofstream fout(filename.c_str());
                    fout << "method : " << (method ? "diff" : "xor") << std::endl;
                    fout << "start frame : " << baseFrame << std::endl;
                    fout << "end frame : " << endFrame << std::endl;
                    fout << "optimal frame : " << optimalFrame << std::endl;
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
}
