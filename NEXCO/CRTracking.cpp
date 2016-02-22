#include "CRTracking.h"
#include "OpenCV.h"
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

IplImage* brightness(IplImage* src, int brightness) {
    brightness = min(brightness, 100);
    brightness = max(brightness, -100);

    IplImage* ret = cvCloneImage(src);
    IplImage* gray = cvCreateImage(cvGetSize(src), src->depth, 1);

    if (src->nChannels == 3) cvCvtColor(src, gray, CV_RGB2GRAY);
    else cvCopyImage(src, gray);

    CvMat* lutMat = cvCreateMatHeader(1, 256, CV_8UC1);
    uchar lut[256];
    cvSetData(lutMat, lut, 0);

    double b = (double)brightness;
    for (int i = 0; i < 256; i++) {
        int v = cvRound(i + b);
        v = max(v, 0);
        v = min(v, 255);
        lut[i] = v;
    }

    if (src->nChannels == 3) {
        IplImage* R = cvCreateImage(cvGetSize(src), src->depth, 1);
        IplImage* G = cvCreateImage(cvGetSize(src), src->depth, 1);
        IplImage* B = cvCreateImage(cvGetSize(src), src->depth, 1);
        cvCvtPixToPlane(src, R, G, B, NULL);
        cvLUT(R, R, lutMat);
        cvLUT(G, G, lutMat);
        cvLUT(B, B, lutMat);
        cvCvtPlaneToPix(R, G, B, NULL, ret);
        cvReleaseImage(&R);
        cvReleaseImage(&G);
        cvReleaseImage(&B);
    }
    else {
        cvLUT(gray, ret, lutMat);
    }

    cvReleaseImage(&gray);
    cvReleaseMat(&lutMat);

    return ret;
}

IplImage* contrast(IplImage* src, int contrast) {
    contrast = min(contrast, 100);
    contrast = max(contrast, -100);

    IplImage* ret = cvCloneImage(src);
    IplImage* gray = cvCreateImage(cvGetSize(src), src->depth, 1);

    if (src->nChannels == 3) cvCvtColor(src, gray, CV_RGB2GRAY);
    else cvCopyImage(src, gray);

    CvMat* lutMat = cvCreateMatHeader(1, 256, CV_8UC1);
    uchar lut[256];
    cvSetData(lutMat, lut, 0);

    if (contrast > 0) {
        double delta = 127. * contrast / 100;
        double a = 255. / (255. - delta * 2);
        double b = a * (-delta);
        for (int i = 0; i < 256; i++) {
            int v = cvRound(a * i + b);
            v = max(v, 0);
            v = min(v, 255);
            lut[i] = v;
        }
    }
    else {
        double delta = -128. * contrast / 100;
        double a = (256. - delta * 2) / 255.;
        double b = delta;
        for (int i = 0; i < 256; i++) {
            int v = cvRound(a * i + b);
            v = max(v, 0);
            v = min(v, 255);
            lut[i] = v;
        }
    }

    if (src->nChannels == 3) {
        IplImage* R = cvCreateImage(cvGetSize(src), src->depth, 1);
        IplImage* G = cvCreateImage(cvGetSize(src), src->depth, 1);
        IplImage* B = cvCreateImage(cvGetSize(src), src->depth, 1);
        cvCvtPixToPlane(src, R, G, B, NULL);
        cvLUT(R, R, lutMat);
        cvLUT(G, G, lutMat);
        cvLUT(B, B, lutMat);
        cvCvtPlaneToPix(R, G, B, NULL, ret);
        cvReleaseImage(&R);
        cvReleaseImage(&G);
        cvReleaseImage(&B);
    }
    else {
        cvLUT(gray, ret, lutMat);
    }

    cvReleaseImage(&gray);
    cvReleaseMat(&lutMat);

    return ret;
}

/*
IplImage* contrastStretch(IplImage* src) {
    IplImage* ret = cvCloneImage(src);
    IplImage* gray = cvCreateImage(cvGetSize(src), src->depth, 1);

    if (src->nChannels == 3) cvCvtColor(src, gray, CV_RGB2GRAY);
    else cvCopyImage(src, gray);

    CvMat* lutMat = cvCreateMatHeader(1, 256, CV_8UC1);
    uchar lut[256];
    cvSetData(lutMat, lut, 0);

    int width = src->width;
    int height = src->height;

    int ct = 0;

    uchar minValue = 0;
    uchar maxValue = 255;
    for (int i = 0; i < src->height; i++) {
        for (int j = 0; j < src->width; j++) {
            minValue = min(minValue, (uchar)gray->imageData[j + i * src->widthStep]);
            maxValue = max(maxValue, (uchar)gray->imageData[j + i * src->widthStep]);
        }
    }

    for (int i = 0; i < 256; i++) {
        if (i <= minValue) lut[i] = 0;
        else if (i >= maxValue) lut[i] = 255;
        else {
            int k = 255 * (i - minValue) / (maxValue - minValue);
            lut[i] = k;
        }
    }


    if (src->nChannels == 3) {
        IplImage* R = cvCreateImage(cvGetSize(src), src->depth, 1);
        IplImage* G = cvCreateImage(cvGetSize(src), src->depth, 1);
        IplImage* B = cvCreateImage(cvGetSize(src), src->depth, 1);
        cvCvtPixToPlane(src, R, G, B, NULL);
        cvLUT(R, R, lutMat);
        cvLUT(G, G, lutMat);
        cvLUT(B, B, lutMat);
        cvCvtPlaneToPix(R, G, B, NULL, ret);
        cvReleaseImage(&R);
        cvReleaseImage(&G);
        cvReleaseImage(&B);
    }
    else {
        cvLUT(gray, ret, lutMat);
    }

    cvReleaseImage(&gray);
    cvReleaseMat(&lutMat);

    return ret;
}
*/

IplImage* gammaCorrection(IplImage *src, double gamma) {
    IplImage* ret = cvCloneImage(src);
    IplImage* gray = cvCreateImage(cvGetSize(src), src->depth, 1);

    if (src->nChannels == 3) cvCvtColor(src, gray, CV_RGB2GRAY);
    else cvCopyImage(src, gray);

    CvMat* lutMat = cvCreateMatHeader(1, 256, CV_8UC1);
    uchar lut[256];
    cvSetData(lutMat, lut, 0);

    for (int i = 0; i < 256; i++)
        lut[i] = uchar(255.0 * pow((i / 255.), 1. / gamma));

    if (src->nChannels == 3) {
        IplImage* R = cvCreateImage(cvGetSize(src), src->depth, 1);
        IplImage* G = cvCreateImage(cvGetSize(src), src->depth, 1);
        IplImage* B = cvCreateImage(cvGetSize(src), src->depth, 1);
        cvCvtPixToPlane(src, R, G, B, NULL);
        cvLUT(R, R, lutMat);
        cvLUT(G, G, lutMat);
        cvLUT(B, B, lutMat);
        cvCvtPlaneToPix(R, G, B, NULL, ret);
        cvReleaseImage(&R);
        cvReleaseImage(&G);
        cvReleaseImage(&B);
    }
    else {
        cvLUT(gray, ret, lutMat);
    }

    cvReleaseImage(&gray);
    cvReleaseMat(&lutMat);

    return ret;
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
                    cvSaveImage(makeFileName(logPath, currentFrame, "bmp").c_str(), base);
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
                    cvSaveImage(makeFileName(logPath, currentFrame, "bmp").c_str(), curr);
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
                    cvSaveImage(makeFileName(logPath, "optimal", "bmp").c_str(), optimal);
                }

                idxTire++;
            }
            break;
        }
        beforeState = state;
    }
}

/* bright 20 contrast 40 */
