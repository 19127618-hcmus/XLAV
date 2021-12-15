#include "Blur.h"
Blur::Blur(){}
Blur::~Blur(){}

//Mọi thứ đã được làm hết trong Convolution

int Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	if (sourceImage.data == NULL)
	{
		return 0;
	}

	if(method == 0) //trung binh
	{
		AverageKernel avg;
		vector<float> kernel;
		avg.SetKernel(kernel, kWidth, kHeight);
		avg.DoConvolution(sourceImage, destinationImage);
		imshow("Average blur", destinationImage);
		waitKey(0);
	}
	else if(method == 1) //trung vi
	{
		MedianKernel med;
		vector<float> kernel;
		med.SetKernel(kernel, kWidth, kHeight);
		med.DoConvolution(sourceImage, destinationImage);
		imshow("Median blur", destinationImage);
		waitKey(0);
	}
	else if(method == 2) //gaussian
	{
		GaussianKernel gaus;
		vector<float> kernel;
		gaus.SetKernel(kernel, kWidth, kHeight);
		gaus.DoConvolution(sourceImage, destinationImage);
		imshow("Average blur", destinationImage);
		waitKey(0);
	}
	else
	{
		return 0;
	}
	return 1;
}


