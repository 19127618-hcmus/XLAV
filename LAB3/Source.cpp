#include "Blur.h"
#include "Convolution.h"
#include "EdgeDetector.h"

int test(Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
	{
		cout << "\nWrong path\n"; return 0;
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

	int offsets[9] = {
		-widthStep - 1, -widthStep, -widthStep + 1,
			-1,				0,			1,
		widthStep - 1, widthStep, widthStep + 1,
	};


	for (int y = yStart; y <= yEnd; y++, pSrcData += widthStep, pDesData+= widthStep)
	{
		const uchar* pSrcRow = pSrcData;
		uchar* pDesRow = pDesData;
		for (int x = xStart; x <= xEnd; x++, pSrcRow++, pDesRow++)
		{
			float avg = 0;
			for (int i = 0; i < 9; i++)
			{
				avg += pSrcRow[offsets[i]];
			}
			avg = avg / 9;
			*pDesRow = (uchar)avg;
		}
	}
	return 1;
}

void printK(vector<float>kernel, int w)
{
	for (int i = 0, f = 0; i < kernel.size(); i++)
	{
		
		cout << kernel.at(i) << " ";
		f++;
		if (f == w)
		{
			cout << endl;
			f = 0;
		}
	}
}

int main(int argc, char*argv[])
{
	if (argc == 6)
	{
		int kx = 0, ky = 0;
		kx = (int)char2float(argv,2);
		ky = (int)char2float(argv, 3);
		Mat src = imread(argv[4], cv::IMREAD_COLOR);
		if (!src.data)
		{
			cout << "Wrong path!";
			return 0;
		}
		Mat graySrc;
		Mat des;
		cvtColor(src, graySrc, COLOR_BGR2GRAY);
		vector<float> kernel;


		if(compare(argv[1],"-avg"))
		{
			AverageKernel Average;
			Average.SetKernel(kernel, kx, ky);
			Average.DoConvolution(graySrc, des);

			imshow("Gray source image", graySrc);
			imshow("Average blur", des);
			imwrite(argv[5], des);
		}
		else if(compare(argv[1],"-med"))
		{
			MedianKernel Median;
			Median.SetKernel(kernel, kx, ky);
			Median.DoConvolution(graySrc, des);

			imshow("Gray source image", graySrc);
			imshow("Median blur", des);
			imwrite(argv[5], des);
		}
		else if (compare(argv[1], "-gau"))
		{
			GaussianKernel Gaussian;
			Gaussian.SetKernel(kernel, kx, ky);
			Gaussian.DoConvolution(graySrc, des);

			imshow("Gray source image", graySrc);
			imshow("Gaussian blur", des);
			imwrite(argv[5], des);
		}
		else
		{
			cout << "Wrong function\n";
			return 0;
		}
	}
	else if (argc == 4)
	{
		EdgeDetector edgeDetec;
		Mat src = imread(argv[2], cv::IMREAD_COLOR);
		if (!src.data)
		{
			cout << "Wrong path!";
			return 0;
		}
		Mat graySrc;
		Mat des;
		cvtColor(src, graySrc, COLOR_BGR2GRAY);

		if(compare(argv[1],"-sobel"))
		{
			edgeDetec.DetectEdge(graySrc, des, 1);
			imshow("Gray source image", graySrc);
			imshow("Sobel", des);
			imwrite(argv[3], des);
		}
		else if (compare(argv[1], "-prew"))
		{
			edgeDetec.DetectEdge(graySrc, des,2);
			imshow("Gray source image", graySrc);
			imshow("Prewitt", des);
			imwrite(argv[3], des);
		}
		else if (compare(argv[1], "-lap"))
		{
			edgeDetec.DetectEdge(graySrc, des, 3);
			imshow("Gray source image", graySrc);
			imshow("Laplace", des);
			imwrite(argv[3], des);
		}
		else
		{
			cout << "Wrong function\n";
			return 0;
		}

	}
	else
	{
		cout << "Input error!\n";
	}



	 /*string path = "m2.png";
	 Mat src = imread(path, cv::IMREAD_COLOR);
	 if (!src.data)
	 {
	 	cout << "Wrong path!";
	 	return 0;
	 }
	 Mat graySrc;
	 cvtColor(src, graySrc, COLOR_BGR2GRAY);

	Mat des;*/

	// test(graySrc, des);
	//
	//
	// imshow("src", src);
	// imshow("graySrc", graySrc);
	// imshow("des", des);
	// waitKey(0);

	 //AverageKernel test;
	 //vector<float> kernel;
	 //test.SetKernel(kernel, 10, 10);
	 //// kernel = test.GetKernel();
	 //test.DoConvolution(graySrc, des);
	 //imshow("graySrc", graySrc);
	 //imshow("des", des);
	 //waitKey(0);

	/*GaussianKernel test2;
	vector<float> kernel;
	test2.SetKernel(kernel, 9, 9);
	test2.DoConvolution(graySrc, des);
	imshow("graySrc", graySrc);
	imshow("des", des);
	waitKey(0);*/

	/*GaussianKernel test2;
	vector<float> kernel;
	test2.SetKernel(kernel, 3, 3);
	kernel = test2.GetKernel();
	printK(kernel, 3);
	test2.DoConvolution(graySrc, des);*/

	/*vector<int> index;
	index = kernelIndex(313, 3, 3);
	printK(index, 3);*/


	/*MedianKernel test3;
	vector<float> kernel;
	test3.SetKernel(kernel, 5, 5);
	kernel = test3.GetKernel();
	// printK(kernel, 3);
	test3.DoConvolution(graySrc, des);
	imshow("graySrc", graySrc);
	imshow("des", des);
	waitKey(0);*/

	/*Blur blur;
	blur.BlurImage(graySrc, des, 5, 5, 2);*/

	// EdgeDetector test4;
	// test4.Laplace(graySrc, des);
	// test4.Prewitt(graySrc, des);
	// test4.Sobel(graySrc, des);

	// imshow("src", src);
	// imshow("graySrc", graySrc);
	// imshow("des", des);
	// waitKey(0);

	/*EdgeDetector x;
	x.DetectEdge(graySrc, des, 3);
	imshow("des", des);*/
	
	waitKey(0);
	return 0;
}