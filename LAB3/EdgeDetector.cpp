#include "EdgeDetector.h"
EdgeDetector::EdgeDetector()
{
}
EdgeDetector::~EdgeDetector()
{
}
int EdgeDetector::DetectEdge(const Mat& sourceImage, Mat& destinationImage, int method)
{
	bool success = 0;
	if(method==1) //sobel
	{
		Sobel(sourceImage, destinationImage);
	}
	else if(method == 2) //prewitt
	{
		Prewitt(sourceImage, destinationImage);
	}
	else if (method == 3) //laplace
	{
		Laplace(sourceImage, destinationImage);
	}
	else
	{
		return 0;
	}
	return 1;
}

int EdgeDetector::Laplace(const Mat& sourceImage, Mat& destinationImage)
{
	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;

	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			int sum = sourceImage.at<uchar>(y - 1, x)
				+ sourceImage.at<uchar>(y + 1, x)
				+ sourceImage.at<uchar>(y, x - 1)
				+ sourceImage.at<uchar>(y, x + 1)
				- 4 * sourceImage.at<uchar>(y, x);

			output.at<uchar>(y, x) = saturate_cast<uchar>(sum);
		}
	}
	destinationImage = output.clone();
	return 1;
}

/*---------------------------------------------------------------------------*/

int EdgeDetector::xGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1)
		+ 2 * image.at<uchar>(y, x - 1)
		+ image.at<uchar>(y + 1, x - 1)
		- image.at<uchar>(y - 1, x + 1)
		- 2 * image.at<uchar>(y, x + 1)
		- image.at<uchar>(y + 1, x + 1);
}
int EdgeDetector::yGradient(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1)
		+ 2 * image.at<uchar>(y - 1, x)
		+ image.at<uchar>(y - 1, x + 1)
		- image.at<uchar>(y + 1, x - 1)
		- 2 * image.at<uchar>(y + 1, x)
		- image.at<uchar>(y + 1, x + 1);
}
int EdgeDetector::Prewitt(const Mat& sourceImage, Mat& destinationImage)
{
	int gx = 0, gy = 0, sum = 0;
	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;

	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			gx = xGradient(sourceImage, x, y);
			gy = yGradient(sourceImage, x, y);
			sum = abs(gx) + abs(gy);
			sum = sum > 255 ? 255 : sum;
			sum = sum < 0 ? 0 : sum;
			output.at<uchar>(y, x) = saturate_cast<uchar>(sum);
		}
	}
	destinationImage = output.clone();
	return 1;
}

/*---------------------------------------------------------------------------*/

int EdgeDetector::Sobel(const Mat& sourceImage, Mat& destinationImage)
{
	std::vector<int> kernel_x({ 1,2,1,0,0,0,-1,-2,-1 });
	std::vector<int> kernel_y({ 1,0,-1,2,0,-2,1,0,-1 });
	int kernel_size = 3;

	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;
	int srcStep = sourceImage.step;

	uchar* data_in = (uchar*)sourceImage.data;
	uchar* data_out = (uchar*)output.data;

	for(int y=0;y<height;y++)
	{
		for(int x=0;x<width;x++)
		{
			if (y <= kernel_size / 2 || y >= height - kernel_size / 2
			 || x <= kernel_size / 2 || x >= width - kernel_size / 2)
			{
				data_out[srcStep * y + x] = 0;
				continue;
			}
			int gx = 0, gy = 0, k = 0;
			for(int k_y=-kernel_size/2;k_y<=kernel_size/2;++k_y)
			{
				for(int k_x = -kernel_size/2;k_x<=kernel_size/2;++k_x)
				{
					gx += kernel_x[k] * data_in[srcStep * (y + k_y) + x + k_x];
					gy += kernel_y[k] * data_in[srcStep * (y + k_y) + x + k_x];
					k++;
				}
			}
			int G = unsigned(sqrt(gx * gx + gy * gy));
			data_out[output.step * y + x] = min(G, 255);
		}
	}
	destinationImage = output.clone();
	return 1;
}
