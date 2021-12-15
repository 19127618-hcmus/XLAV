#pragma once
#include "Lib.h"
class EdgeDetector
{
public:
	/*
	Hàm phát hiện biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
		1: Sobel
		2: Prewitt
		3: Laplace		
	Hàm trả về
		0: nếu detect thành công
		1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int method);

	//sobel
	int Sobel(const Mat& sourceImage, Mat& destinationImage);

	//prewitt
	int xGradient(Mat image, int x, int y);
	int yGradient(Mat image, int x, int y);
	int Prewitt(const Mat& sourceImage, Mat& destinationImage);

	//laplace
	int Laplace(const Mat& sourceImage, Mat& destinationImage);

	EdgeDetector();
	~EdgeDetector();
};

