#ifndef ANDROID_H
#define ANDROID_H

/*
 * Compilation pour Android
 * 
 */

#ifndef DEBUG_MODE

#define LOG_TAG "TRAITEMENT_IMAGE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#include "cv.h"
#include <android/log.h>
#include <sstream>

#endif


/*
 * Compilation pour PC, debug
 * 
 */

#ifdef DEBUG_MODE

#define LOGI(x) (std::cout << x << std::endl)

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

#endif

template<typename T>
std::string convertString(T x)
{
	std::ostringstream oss;
	oss<<x;
	return oss.str();
}

#endif
