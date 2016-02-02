#pragma once
#include <cv.h>
#include <opencv2/opencv.hpp>

#ifdef WIN32
    #define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)

    #ifdef _DEBUG
        #define CV_PATH "C:\\opencv2411\\build\\x64\\vc12\\staticlib\\"
        #define CV_EXT_STR "d.lib"
    #else
        #define CV_PATH "C:\\opencv2411\\build\\x64\\vc12\\staticlib\\"
        #define CV_EXT_STR ".lib"
    #endif

    #pragma comment(lib, CV_PATH "IlmImf"	                            CV_EXT_STR)
    #pragma comment(lib, CV_PATH "libjasper"	                        CV_EXT_STR)
    #pragma comment(lib, CV_PATH "libjpeg"	                            CV_EXT_STR)
    #pragma comment(lib, CV_PATH "libpng"	                            CV_EXT_STR)
    #pragma comment(lib, CV_PATH "libtiff"	                            CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_calib3d"       CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_contrib"       CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_core"          CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_features2d"    CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_flann"         CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_gpu"           CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_highgui"       CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_imgproc"       CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_legacy"        CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_ml"            CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_nonfree"       CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_objdetect"     CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_ocl"           CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_photo"         CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_stitching"     CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_superres"      CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_ts"            CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_video"         CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "opencv_videostab"     CV_VERSION_STR  CV_EXT_STR)
    #pragma comment(lib, CV_PATH "zlib"	                                CV_EXT_STR)
#endif
