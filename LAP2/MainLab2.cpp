#include "GeometricTransformer.h"
//



bool compare(const char* a, string b)
{
	int la = strlen(a), lb = b.length();
	//cout << la << " - " << lb;
	if (la != lb) return 0;
	else
	{
		for (int i = 0; i < la; i++)
		{
			if (a[i] != b[i]) return 0;
		}
	}

	return 1;
}

float char2float(char* argv[], int n)
{
	float temp = strtod(argv[n], NULL);
	return temp;
}

int main(int argc, char* argv[])
{
	GeometricTransformer* geometric = new GeometricTransformer();
	PixelInterpolate* pixel;
	Mat des;

	//interp
	if(compare(argv[2],"-bl"))
	{
		pixel = new BilinearInterpolate();
	}
	else if(compare(argv[2], "-nn"))
	{
		pixel = new NearestNeighborInterpolate();
	}
	else
	{
		cout << "Wrong interp\n";
		return 0;
	}

	//function
	if (argc == 7)
	{
		Mat src = imread(argv[5], cv::IMREAD_COLOR);
		if(!src.data)
		{
			cout << "Wrong path!";
			return 0;
		}
		//zoom
		if(compare(argv[1], "-zoom"))
		{
			bool success = 0;
			success = geometric->Scale(src, des, char2float(argv, 3), char2float(argv,4), pixel);
			if (success)imwrite(argv[6], des);
		}
		//resize
		else if (compare(argv[1], "-resize"))
		{
			bool success = 0;
			success = geometric->Resize(src, des, char2float(argv, 3), char2float(argv, 4), pixel);
			if (success)imwrite(argv[6], des);
		}
		else
		{
			cout << "Wrong function\n";
			return 0;
		}
	}
	else if (argc == 6)
	{
		Mat src = imread(argv[4], cv::IMREAD_COLOR);
		if (!src.data)
		{
			cout << "Wrong path!";
			return 0;
		}
		//rotate unKeep
		if (compare(argv[1], "-rotK"))
		{
			bool success = 0;
			success = geometric->RotateUnkeepImage(src, des, char2float(argv, 3), pixel);
			if (success)imwrite(argv[5], des);
		}
		//rotate keep
		else if (compare(argv[1], "-rotP"))
		{
			bool success = 0;
			success = geometric->RotateKeepImage(src, des, char2float(argv, 3), pixel);
			if (success)imwrite(argv[5], des);
		}
		else
		{
			cout << "Wrong function\n";
			return 0;
		}
	}
	else if (argc == 5)
	{
		Mat src = imread(argv[3], cv::IMREAD_COLOR);
		if (!src.data)
		{
			cout << "Wrong path!";
			return 0;
		}
		//flip horizontally
		if (compare(argv[1], "-flipH"))
		{
			bool success = 0;
			success = geometric->Flip(src, des, 1, pixel);
			if (success)imwrite(argv[4], des);
		}
		//flip vertically
		else if (compare(argv[1], "-flipV"))
		{
			bool success = 0;
			success = geometric->Flip(src, des, 0, pixel);
			if (success)imwrite(argv[4], des);
		}
		else
		{
			cout << "Wrong function\n";
			return 0;
		}
	}
	else cout << "Input error!\n";


	return 0;
}



//
//void printMatrix(int matrix[][5], int w, int h)
//{
//	for (int i = 0; i < h; i++)
//	{
//		for(int j=0;j<w;j++)
//		{
//			cout << matrix[i][j] << " ";
//		}
//		cout << endl;
//	}
//}
//
//void cal(float a, float b, int matrix[][5])
//{
//	int x = a, y = b;
//	a = a - x;
//	b = b - y;
//
//	float q = (1 - a) * (1 - b) * matrix[y][x];
//
//	q += a * (1 - b) * matrix[y][x+1];
//	q += (1 - a) * b * matrix[y+1][x];
//	q += +a * b * matrix[y + 1][x + 1];
//	cout << q;
//}
//int main() {
//	Mat src = imread("m1.jpg", cv::IMREAD_COLOR);
//
//	int matrix1[5][5] = {
//		{1,2,6,3,7},
//		{3,5,3,7,8},
//		{1,2,3,4,5},
//		{0,3,0,3,1},
//		{1,4,5,6,7}
//	};
//	int matrix2[10][10];
//
//
//	//printMatrix(matrix1,5,5);
//	//cal(0,0, matrix1);
//
//	 Mat des;
//	 PixelInterpolate* pixel;
//	 pixel = new BilinearInterpolate();
//	 GeometricTransformer* x = new GeometricTransformer();
//	 //x->RotateKeepImage(src, des, 45, pixel);
//	 //x->RotateUnkeepImage(src, des, 45, pixel);
//	 // x->Scale(src, des, 3, 2, pixel);
//	 x->Resize(src, des, 200, 200, pixel);
//	 //x->Flip(src, des, 1, pixel);
//
//	//imshow("source", src);
//	//waitKey(0);
//	return 0;
//}
//
///*
// *5,6 /2 -> 2.5, 3
// *a=0.5  , b=0
// *f(l,k) = (2,3)
// *f(l+1,k) = (3,3)
// *=> cong thuc: 0.5*1*(2,3) + 0.5*1*(3,3) + 0.5*0*(2,4) + 0.5*0*(3,4)
// *
// *0+1.5 = 1.5
// *
// *
// */