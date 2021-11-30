#pragma once
#ifndef LIB_H_
#define LIB_H_

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
#include <cmath>

using namespace std;
using namespace cv;

const double PI = 3.14159265358979323846;

bool compare(const char* a, string b);
float char2float(char* argv[], int n);

# endif	