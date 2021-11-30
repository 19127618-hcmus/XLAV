#pragma once
#ifndef LIB_H_

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace cv;

double* RGB2HSVInPixel(uchar B, uchar R, uchar G);




# endif	