#include "Convolution.h"

Convolution::Convolution(){}
Convolution::~Convolution(){}

 vector<float> Convolution::GetKernel()
 {
	 return this->_kernel;
 }

int Convolution::DoConvolution(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
	{
		  return 0;
	}
	int width = sourceImage.cols, height = sourceImage.rows;
	int srcChannels = sourceImage.channels();
	destinationImage = Mat(height, width, CV_8UC1);
	int desChannels = destinationImage.channels();

	int xStart = 1, yStart = 1;
	int xEnd = width - 2, yEnd = height - 2;
	int widthStep = sourceImage.step[0];

	uchar* pSrcData = (uchar*)sourceImage.data + widthStep + 1;
	uchar* pDesData = (uchar*)destinationImage.data + widthStep + 1;

	vector<int> offsets = kernelIndex(widthStep, _kernelWidth, _kernelHeight);
	int kernelSize = _kernelWidth * _kernelHeight;

	for (int y = yStart; y <= yEnd; y++, pSrcData += widthStep, pDesData += widthStep)
	{
		const uchar* pSrcRow = pSrcData;
		uchar* pDesRow = pDesData;
		for (int x = xStart; x <= xEnd; x++, pSrcRow++, pDesRow++)
		{
			float center = 0;
			for (int i = 0; i < kernelSize; i++)
			{
				center += pSrcRow[offsets[i]] * _kernel[i];
			}
			*pDesRow = (uchar)center;
		}
	}
	return 1;
}

/*-------------------------------------------------------------------------------------*/
//average
AverageKernel::AverageKernel(){}
AverageKernel::~AverageKernel(){}

void AverageKernel::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	int width = 0, height = 0;
	width = isOdd(kWidth) ? kWidth : makeOdd(kWidth);
	height = isOdd(kHeight) ? kHeight : makeOdd(kHeight);
	width = width < 3 ? 3 : width;
	height = height < 3 ? 3 : height;

	this->_kernelWidth = width;
	this->_kernelHeight = height;

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			kernel.push_back((float)1.0/(width*height));
		}
	}
	this->_kernel = kernel;
}

/*-------------------------------------------------------------------------------------*/
//gaussian
GaussianKernel::GaussianKernel(){}
GaussianKernel::~GaussianKernel(){}

void GaussianKernel::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	int width = 0, height = 0;
	width = isOdd(kWidth) ? kWidth : makeOdd(kWidth);
	height = isOdd(kHeight) ? kHeight : makeOdd(kHeight);
	width = width < 3 ? 3 : width;
	height = height < 3 ? 3 : height;
	int size = width * height;

	this->_kernelWidth = width;
	this->_kernelHeight = height;

	double sigma = 2;
	double meanX = width / 2, meanY = height / 2;

	double sum = 0.0; // For accumulating the kernel values
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y) {
			double temp = exp(-0.5 * (pow((x - meanX) / sigma, 2.0) + pow((y - meanY) / sigma, 2.0)))
				/ (2 * PI * sigma * sigma);
			kernel.push_back(temp);
			// Accumulate the kernel values
			sum += temp;
		}

	// Normalize the kernel
	for (int k = 0; k < size; k++)
	{
		kernel.at(k) = kernel.at(k)*1.0 / sum;
	}

	this->_kernel = kernel;
}

/*-------------------------------------------------------------------------------------*/
//median
MedianKernel::MedianKernel() {}
MedianKernel::~MedianKernel(){}
void MedianKernel::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	int width = 0, height = 0;
	width = isOdd(kWidth) ? kWidth : makeOdd(kWidth);
	height = isOdd(kHeight) ? kHeight : makeOdd(kHeight);
	width = width < 3 ? 3 : width;
	height = height < 3 ? 3 : height;
	int size = width * height;

	this->_kernelWidth = width;
	this->_kernelHeight = height;

	kernel.resize(size);
	this->_kernel = kernel;

}
int MedianKernel::DoConvolution(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
	{
		  return 0;
	}
	int width = sourceImage.cols, height = sourceImage.rows;
	int srcChannels = sourceImage.channels();
	destinationImage = Mat(height, width, CV_8UC1);
	int desChannels = destinationImage.channels();

	int xStart = 1, yStart = 1;
	int xEnd = width - 2, yEnd = height - 2;
	int widthStep = sourceImage.step[0];

	uchar* pSrcData = (uchar*)sourceImage.data + widthStep + 1;
	uchar* pDesData = (uchar*)destinationImage.data + widthStep + 1;

	vector<int> offsets = kernelIndex(widthStep, _kernelWidth, _kernelHeight);
	int kernelSize = _kernelWidth * _kernelHeight;

	for (int y = yStart; y <= yEnd; y++, pSrcData += widthStep, pDesData += widthStep)
	{
		const uchar* pSrcRow = pSrcData;
		uchar* pDesRow = pDesData;
		for (int x = xStart; x <= xEnd; x++, pSrcRow++, pDesRow++)
		{
			float median = 0;
			vector<int> data;
			data.resize(0);
			for (int i = 0; i < kernelSize; i++)
			{
				data.push_back(pSrcRow[offsets[i]]);
			}
			sort(data.begin(), data.end());
			median = data.at(kernelSize / 2);
			*pDesRow = (uchar)median;
		}
	}
	return 1;
}


