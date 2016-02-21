#pragma once

#if defined(_MSC_VER)
    #define CV_DO_PRAGMA(x) __pragma(x)
    #define __CVSTR2__(x) #x
    #define __CVSTR1__(x) __CVSTR2__(x)
    #define __CVMSVCLOC__ __FILE__ "("__CVSTR1__(__LINE__)") : "
    #define CV_MSG_PRAGMA(_msg) CV_DO_PRAGMA(message (__CVMSVCLOC__ _msg))
#elif defined(__GNUC__)
    #define CV_DO_PRAGMA(x) _Pragma (#x)
    #define CV_MSG_PRAGMA(_msg) CV_DO_PRAGMA(message (_msg))
#else
    #define CV_DO_PRAGMA(x)
    #define CV_MSG_PRAGMA(_msg)
#endif
#define CV_WARNING(x) CV_MSG_PRAGMA("Warning: " #x)

#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"

#if !defined(CV_IMPL)
#define CV_IMPL extern "C"
#endif //CV_IMPL

#if defined(__cplusplus)
#include "opencv2/core/internal.hpp"
#endif //__cplusplus

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
