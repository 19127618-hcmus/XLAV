#include "Converter.h"
#include "Lib.h"
#include "ColorTransformer.h"

//Converter
Converter::Converter(){}
Converter::~Converter(){}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	if(type == 0)
	{
		return RGB2GrayScale(sourceImage, destinationImage);
	}
	else if(type == 1)
	{
		return RGB2HSV(sourceImage, destinationImage);
	}
	else
	{
		return 0;
	}

}
//
int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
		{cout << "\nWrong path\n";system("pause");return 0; }
	int width = sourceImage.cols, height = sourceImage.rows;
	int srcChannels = sourceImage.channels();
	destinationImage = Mat(height, width, CV_8UC1);
	int desChannels = destinationImage.channels();


	for (int y = 0; y < height; y++)
	{
		const uchar* pSrcRow = sourceImage.ptr<uchar>(y);
		uchar* pDesRow = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++, pSrcRow += srcChannels, pDesRow += desChannels)
		{
			uchar B = pSrcRow[0];
			uchar G = pSrcRow[1];
			uchar R = pSrcRow[2];
			uchar gray_val = (uchar)((R + G + B) / 3);
			pDesRow[0] = gray_val;
		}
	}
	//imshow("Gray", destinationImage);
	//waitKey(0);
	//system("pause");
	return 1;
}
//
double* RGB2HSVInPixel(uchar B, uchar R, uchar G)
{
	double rgbMin, rgbMax;
	double h, s, v;
	double* hsv = new double[3];
	double r = R, g = G, b = B;

	rgbMin =  r <  g ? ( r <  b ?  r :  b) : ( g <  b ?  g :  b);
	rgbMax =  r >  g ? ( r >  b ?  r :  b) : ( g >  b ?  g :  b);
	 v = rgbMax;
	//if ( v == 0)
	//{
	//	 h = 0;
	//	 s = 0;
	//	return hsv;
	//}

	 s = 255 * long(rgbMax - rgbMin) /  v;
	/*if ( s == 0)
	{
		 h = 0;
		return hsv;
	}*/

	if (rgbMax ==  r)
		 h = 0 + 43 * ( g -  b) / (rgbMax - rgbMin);
	else if (rgbMax ==  g)
		 h = 60 + 43 * ( b -  r) / (rgbMax - rgbMin);
	else
		 h = 125 + 43 * ( r -  g) / (rgbMax - rgbMin);
	
	if (h < 0) h = 0;
	if (h > 240) h = 240;

	hsv[0] = h;
	hsv[1] = s;
	hsv[2] = v;

	return hsv;
}
//
int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
		{cout << "\nWrong path\n";system("pause");return 0; }
	int width = sourceImage.cols, height = sourceImage.rows;
	int srcChannels = sourceImage.channels();

	destinationImage = Mat(height, width, CV_8UC3, Scalar(0, 0, 0));
	int desChannels = destinationImage.channels();

	for (int y = 0; y < height; y++)
	{
		const uchar* pSrcRow = sourceImage.ptr<uchar>(y);
		uchar* pDesRow = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++, pSrcRow += srcChannels, pDesRow += desChannels)
		{
			uchar B = pSrcRow[0];
			uchar G = pSrcRow[1];
			uchar R = pSrcRow[2];
			
			double* hsv = RGB2HSVInPixel(B, R, G);
			pDesRow[0] = hsv[0];
			pDesRow[1] = hsv[1];
			pDesRow[2] = hsv[2];

			/*uchar gray_val = (uchar)((R + G + B) / 3);
			pDesRow[0] = gray_val;*/
		}
	}
	//cvtColor(destinationImage, destinationImage, COLOR_HSV2BGR);
	//imshow("HSV", destinationImage);
	//waitKey(0);
	return 1;
}


//ColorTransformer
ColorTransformer::ColorTransformer() {}
ColorTransformer::~ColorTransformer() {}
//
int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{
	if (sourceImage.data == NULL)
		{cout << "\nWrong path\n";system("pause");return 0; }
	int width = sourceImage.cols, height = sourceImage.rows;
	destinationImage = Mat(height, width, CV_8UC3);

	int srcChannels = sourceImage.channels();
	int desChannels = destinationImage.channels();

	for (int y = 0; y < height; y++)
	{
		const uchar* pSrcRow = sourceImage.ptr<uchar>(y);
		uchar* pDesRow = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++, pSrcRow += srcChannels, pDesRow += desChannels)
		{
			int B = pSrcRow[0] + b;
			int G = pSrcRow[1] + b;
			int R = pSrcRow[2] + b;

			B < 0 ? B = 0 : B = B;
			G < 0 ? G = 0 : G = G;
			R < 0 ? R = 0 : R = R;

			B > 255 ? B = 255 : B = B;
			G > 255 ? G = 255 : G = G;
			R > 255 ? R = 255 : R = R;

			pDesRow[0] = B;
			pDesRow[1] = G;
			pDesRow[2] = R;
		}
	}
	//imshow("Brightness", destinationImage);
	//waitKey(0);
	//system("pause");
	return 1;
}
int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
{
	if (sourceImage.data == NULL)
		{cout << "\nWrong path\n";system("pause");return 0; }
	int width = sourceImage.cols, height = sourceImage.rows;
	destinationImage = Mat(height, width, CV_8UC3);

	int srcChannels = sourceImage.channels();
	int desChannels = destinationImage.channels();

	c = c * 170;

	float f = (259 * (c + 255)) / (255*(259 - c)); 

	for (int y = 0; y < height; y++)
	{
		const uchar* pSrcRow = sourceImage.ptr<uchar>(y);
		uchar* pDesRow = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++, pSrcRow += srcChannels, pDesRow += desChannels)
		{
			int B = f * (pSrcRow[0] - 128) + 128;
			int G = f * (pSrcRow[1] - 128) + 128;
			int R = f * (pSrcRow[2] - 128) + 128;

			B < 0 ? B = 0 : B = B;
			G < 0 ? G = 0 : G = G;
			R < 0 ? R = 0 : R = R;

			B > 255 ? B = 255 : B = B;
			G > 255 ? G = 255 : G = G;
			R > 255 ? R = 255 : R = R;

			pDesRow[0] = B;
			pDesRow[1] = G;
			pDesRow[2] = R;
		}
	}
	//imshow("Contrast", destinationImage);
	//waitKey(0);
	//system("pause");
	return 1;
}

//histogram
//
//function to take histogram of input image
void imageHistogram(Mat image, int histogram[]) {
	for (int i = 0; i < 256; i++) {
		histogram[i] = 0;
	}
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			histogram[(int)image.at<uchar>(y, x)]++;
		}
	}
}

//function to take calculate cumulative histogram of input image
void cumulativeImageHistogram(int histogram[], int cumulativeHistogram[]) {
	cumulativeHistogram[0] = histogram[0];
	for (int i = 1; i < 256; i++) {
		cumulativeHistogram[i] = histogram[i] + cumulativeHistogram[i - 1];
	}
}
void roundHis(int histogram[], int cumulativeHistogram[], int size)
{
	for (int i = 0; i < 256; i++) {
		cumulativeHistogram[i] = round((float)((255.0 / size) * histogram[i]));
	}
}
void calHis(const Mat& image, int hist[], int size)
{
	int histogramH[256];
	imageHistogram(image, histogramH);
	int histogramT[256];
	cumulativeImageHistogram(histogramH, histogramT);
	roundHis(histogramT, hist, size);
	
}

int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histMatrix)
{
	if (sourceImage.data == NULL)
		{cout << "\nWrong path\n";system("pause");return 0; }

	vector<Mat> bgr_planes;
	split(sourceImage, bgr_planes);

	int hist[3][256];
	int size = sourceImage.rows * sourceImage.cols;

	calHis(bgr_planes[0], hist[0], size);
	calHis(bgr_planes[1], hist[1], size);
	calHis(bgr_planes[2], hist[2], size);

	int col = 256, row = 3;

	histMatrix = Mat(row, col, CV_8UC1);
	int desChannels = histMatrix.channels();

	for (int i = 0; i < row; i++)
	{
		uchar* pDesRow = histMatrix.ptr<uchar>(i);
		for (int y = 0; y < col; y++, pDesRow += desChannels)
		{
			pDesRow[0] = hist[i][y];
		}
	}
	
	return 1;
}

void mapping(Mat image, int hist[], Mat &des) {

	//int size = image.rows * image.cols;
	des = image.clone();
	for(int k = 0; k<256;k++)
	{
		for (int y = 0; y < image.rows; y++) {
			for (int x = 0; x < image.cols; x++) {
				if ((int)image.at<uchar>(y, x) == k)
				{
					des.at<uchar>(y, x) = hist[k];
				}
			}
		}
	}
}

//Hàm Eq theo hệ màu RGB có vẻ ảnh ra không chính xác lắm.
int ColorTransformer::HistogramEqualizationRGB(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
		{cout << "\nWrong path\n";system("pause");return 0; }

	vector<Mat> bgr_planes;
	split(sourceImage, bgr_planes);

	destinationImage = sourceImage.clone();

	int hist[3][256];
	int size = sourceImage.rows * sourceImage.cols;

	calHis(bgr_planes[0], hist[0], size);
	calHis(bgr_planes[1], hist[1], size);
	calHis(bgr_planes[2], hist[2], size);

	vector<Mat> bgr_new;

	Mat temp1, temp2, temp3;

	mapping(bgr_planes[0], hist[0], temp1);
	bgr_new.push_back(temp1);
	mapping(bgr_planes[1], hist[1], temp2);
	bgr_new.push_back(temp2);
	mapping(bgr_planes[2], hist[2], temp3);
	bgr_new.push_back(temp3);

	merge(bgr_new, destinationImage);

	//imshow("histogram", destinationImage);
	//waitKey(0);
	//system("pause");
	return 1;
}

/*Sau khi tìm hiểu rất nhiều tài liệu thì em thấy hệ màu RGB không thể Equalization chính xác được nên em đã code thêm hàm Equalization
 theo hệ màu YCrCb. Em vẫn sẽ giữ lại hàm cũ vừa để so sánh kết quả vừa để rút thêm kinh nghiệm.*/
int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data == NULL)
	{
		cout << "\nWrong path\n"; return 0;
	}
	cvtColor(sourceImage, destinationImage, COLOR_BGR2YCrCb);

	//Split the image into 3 channels; Y, Cr and Cb channels respectively and store it in a std::vector
	vector<Mat> vec_channels;
	split(destinationImage, vec_channels);

	//Equalize the histogram of only the Y channel 
	//equalizeHist(vec_channels[0], vec_channels[0]);

	int hist[256];
	int size = sourceImage.rows * sourceImage.cols;
	calHis(vec_channels[0], hist, size);

	int col = sourceImage.cols, row = 1;
	Mat temp, temp2;
	mapping(vec_channels[0], hist, temp);
	temp2.push_back(temp);
	vec_channels[0] = temp2;

	//Merge 3 channels in the vector to form the color image in YCrCB color space.
	merge(vec_channels, destinationImage);

	//Convert the histogram equalized image from YCrCb to BGR color space again
	cvtColor(destinationImage, destinationImage, COLOR_YCrCb2BGR);

	//imshow("null", destinationImage);
	//waitKey(0);

	//system("pause");
	return 1;
}

/*Em làm hàm này theo hệ màu RGB vì ở hàm calHistogram đề yêu cầu tách ra thành 3 kênh màu RGB.
  Em có làm thêm hệ màu YCrCb ở ngay phía dưới. */
int ColorTransformer::DrawHistogram(const Mat& histMatrix, Mat& histImage)
{
	if (histMatrix.data == NULL)
		{cout << "\nWrong path\n";system("pause");return 0; }

	vector<Mat> b_g_b;

	Mat temp0 = Mat::zeros(256, 1, CV_8UC1), temp1 = Mat::zeros(256, 1, CV_8UC1), temp2 = Mat::zeros(256, 1, CV_8UC1);

	int srcChannels = histMatrix.channels();
	int row = 1, col = 256;
	
	const uchar* pSrcRow = histMatrix.ptr<uchar>(0);	uchar* pDesRow = temp0.ptr<uchar>(0);
	for (int y = 0; y < col; y++, pSrcRow += srcChannels, pDesRow += srcChannels) pDesRow[0] = pSrcRow[0];
	
	const uchar* pSrcRow1 = histMatrix.ptr<uchar>(1);	uchar* pDesRow1 = temp1.ptr<uchar>(0);
	for (int y = 0; y < col; y++, pSrcRow1 += srcChannels, pDesRow1 += srcChannels) pDesRow1[0] = pSrcRow1[0];

	const uchar* pSrcRow2 = histMatrix.ptr<uchar>(2);	uchar* pDesRow2 = temp2.ptr<uchar>(0);
	for (int y = 0; y < col; y++, pSrcRow2 += srcChannels, pDesRow2 += srcChannels) pDesRow2[0] = pSrcRow2[0];

	//cout << temp0.size << endl;

	int hist_w = 512; int hist_h = 400, bin_w = cvRound((double)512 / 256);
	int histSize = 256;
	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound((float)temp0.at<uchar>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound((float)temp0.at<uchar>(i))),
			Scalar(255, 0, 0), 2, 8, 0);

		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound((float)temp1.at<uchar>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound((float)temp1.at<uchar>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound((float)temp2.at<uchar>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound((float)temp2.at<uchar>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	//cout << temp0.at<float>(0);
	//imshow("Histogram", histImage);
	//waitKey(0);
	//system("pause");
	return 1;
}

/*Hàm này tạo ra với mục đích vẽ histogram bằng kênh mày Y của hệ màu YCrCb.
 Trên lớp lý thuyết thầy đã dạy về cách tính histogram của gray scale image nên em quyết định làm thêm 1 hàm này 
 các kênh màu không tỏ ra hiệu quả khi cân màu, trong khi với hệ màu YCrCb lại có kênh Y mang thông tin cường độ, Cr + Cb chứa thông tin màu.
 nên chỉ cần xử lý kênh Y ta có thể cân bằng được histogram. Tuy vậy em vẫn sẽ làm cả trên kênh màu RGB.*/
int ColorTransformer::DrawHistogramYCrCb(const Mat& srcImg, Mat& histImage)
{
	if (srcImg.data == NULL)
		{cout << "\nWrong path\n";system("pause");return 0; }

	Mat YCrCbImg;
	cvtColor(srcImg, YCrCbImg, COLOR_BGR2YCrCb);

	vector<Mat> vec_channels;
	split(YCrCbImg, vec_channels);

	int hist[256];
	int size = srcImg.rows * srcImg.cols;
	calHis(vec_channels[0], hist, size);

	Mat temp;
	temp = Mat(1, 255, CV_8UC1, hist);

	int histSize = 256;
	int hist_w = 512; int hist_h = 400, bin_w = cvRound((double)512 / 256);
	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound((float)temp.at<uchar>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound((float)temp.at<uchar>(i))),
			Scalar(255, 255, 255), 2, 8, 0);
	}
	//cout << temp0.at<float>(0);
	imshow("Histogram", histImage);
	waitKey(0);
	//system("pause");
	return 1;
}

void wannaSave(const Mat& input)
{
	system("cls");

	char save = ' ';
	string OPath;

	cout << "\nWanna save? (y/n): ";
	cin.ignore();
	cin >> save;
	if (save == 'y')
	{
		cout << "Input path: ";
		cin.ignore();
		cin >> OPath;
		imwrite(OPath, input);
	}
	else
	{
		return;
	}
}

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

int main(int argc, char* argv[]) {

	ColorTransformer* imgCTf = new ColorTransformer();
	Converter* imgCv = new Converter();

	if(argc == 4)
	{
		Mat src = imread(argv[2], IMREAD_COLOR); //can only load images in the same folder!

		if(compare(argv[1],"-rgb2gray"))
		{
			Mat des;
			cout << "gray\n";

			bool success = 0;
			success = imgCv->Convert(src, des, 0);
			if (success)
			{
				imwrite(argv[3], des);
				cout << "success";
			}
			else cout << "unSuccess";
		}
		else if(compare(argv[1], "-rgb2hsv"))
		{
			Mat des;
			cout << "-rgb2hsv\n";

			bool success = 0;
			success = imgCv->Convert(src, des, 1);
			if (success)
			{
				imwrite(argv[3], des);
				cout << "success";
			}
			else cout << "unSuccess";
		}
		else if (compare(argv[1], "-drawhist"))
		{
			Mat des, matrix;
			cout << "-drawhist\n";

			bool success = 0;
			imgCTf->CalcHistogram(src, matrix);
			success = imgCTf->DrawHistogram(matrix, des);
			if (success)
			{
				imwrite(argv[3], des);
				cout << "success";
			}
			else cout << "unSuccess";
		}
		else if (compare(argv[1], "-equalhist"))
		{
			Mat des;
			cout << "-equalhist\n";

			bool success = 0;
			success = imgCTf->HistogramEqualization(src, des);
			if (success)
			{
				imwrite(argv[3], des);
				cout << "success";
			}
			else cout << "unSuccess";
		}
		else cout << "Input Error!!!";
	}

	else if(argc == 5)
	{
		Mat src = imread(argv[3], IMREAD_COLOR); //can only load images in the same folder!

		if (compare(argv[1], "-bright"))
		{
			Mat des;
			cout << "bright\n";

			stringstream ss(argv[2]);
			int i;
			if (ss >> i) cout << i << endl;
			else cout << "err";

			bool success = 0;
			success = imgCTf->ChangeBrighness(src, des, i);

			if (success)
			{
				imwrite(argv[4], des);
				cout << "success";
			}
			else cout << "unSuccess";
		}
		else if (compare(argv[1], "-contrast"))
		{
			Mat des;
			cout << "-contrast\n";

			stringstream ss(argv[2]);
			int i;
			if (ss >> i) cout << i << endl;
			else cout << "err";

			bool success = 0;
			success = imgCTf->ChangeContrast(src, des, i);
			if (success)
			{
				imwrite(argv[4], des);
				cout << "success";
			}

			else cout << "unSuccess";
		}

		else cout << "Input Error!!!";

	}
	else cout << "Input Error!!!";










	//string path;
	//cout << "Input path: ";
	//cin >> path;
	//Mat src = imread(path, IMREAD_COLOR); //can only load images in the same folder!
	////Mat src = imread("C:\\Users\\Tung\\Desktop\\New folder\\m1.jpg", IMREAD_COLOR); //Direct link transmission can go anywhere. :V

	//char op = ' ';

	//while (1)
	//{
	//	system("cls");
	//	cin.ignore();
	//	cout << "\nPress 1: convert RGB image to GrayScale image\n";
	//	cout << "Press 2: convert RGB image to HSV image\n";
	//	cout << "Press 3: change brightness of a image\n";
	//	cout << "Press 4: change contrast of a image\n";
	//	cout << "Press 5: equalize histogram of a image (RGB)\n";
	//	cout << "Press 6: equalize histogram of a image (YCrCb)\n";
	//	cout << "Press 7: draw histogram of a image (RGB 3 line)\n";
	//	cout << "Press 8: draw histogram of a image (YCrCb 1 line Y)\n";
	//	cout << "Press 0: exit\n\n";
	//	cout << "Enter option: ";
	//	cin >> op;

	//	
	//	ColorTransformer* imgCTf = new ColorTransformer();
	//	int success = 0;

	//	if (op == '0') break;
	//	else if (op == '1')
	//	{
	//		Converter* imgCv = new Converter();
	//		Mat des;
	//		cout << "\nOption 1 is running, please waiting...!\n";
	//		cout << "Close the preview image window to save the image.\n\n";

	//		success = 0;
	//		success = imgCv->Convert(src, des, 0);
	//		if(success) wannaSave(des);
	//		delete imgCv;
	//		imgCv = NULL;
	//	}
	//	else if (op == '2')
	//	{
	//		Converter* imgCv = new Converter();
	//		Mat des;
	//		cout << "\nOption 2 is running, please waiting...!\n";
	//		cout << "Close the preview image window to save the image.\n\n";

	//		success = 0;
	//		success = imgCv->Convert(src, des, 1);
	//		if (success) wannaSave(des);

	//		delete imgCv;
	//		imgCv = NULL;
	//	}
	//	else if (op == '3')
	//	{
	//		Mat des;
	//		cout << "\nOption 3 is running, please waiting...!\n";
	//		cout << "Close the preview image window to save the image.\n\n";

	//		success = 0;
	//		int lv = 0;
	//		cout << "Enter level [-255,255]: ";
	//		cin >> lv;
	//		success = imgCTf->ChangeBrighness(src, des, lv);
	//		if (success) wannaSave(des);
	//	}
	//	else if (op == '4')
	//	{
	//		Mat des;
	//		cout << "\nOption 4 is running, please waiting...!\n";
	//		cout << "Close the preview image window to save the image.\n\n";

	//		success = 0;
	//		float lv = 0;
	//		cout << "Enter level [-1,1]: ";
	//		cin >> lv;
	//		success = imgCTf->ChangeContrast(src, des, lv);
	//		if (success) wannaSave(des);
	//	}
	//	else if (op == '5')
	//	{
	//		Mat des;
	//		cout << "\nOption 5 is running, please waiting...!\n";
	//		cout << "Close the preview image window to save the image.\n\n";

	//		success = 0;
	//		success = imgCTf->HistogramEqualizationRGB(src, des);
	//		if (success) wannaSave(des);
	//	}
	//	else if (op == '6')
	//	{
	//		Mat des;
	//		cout << "\nOption 6 is running, please waiting...!\n";
	//		cout << "Close the preview image window to save the image.\n\n";

	//		success = 0;
	//		success = imgCTf->HistogramEqualization(src, des);
	//		if (success) wannaSave(des);
	//		system("pause");
	//	}
	//	else if (op == '7')
	//	{
	//		Mat des;
	//		Mat matrix;
	//		cout << "\nOption 7 is running, please waiting...!\n";
	//		cout << "Close the preview image window to save the image.\n\n";

	//		success = 0;
	//		imgCTf->CalcHistogram(src, matrix);
	//		success = imgCTf->DrawHistogram(matrix, des);
	//		if (success) wannaSave(des);
	//	}
	//	else if (op == '8')
	//	{
	//		Mat des;
	//		Mat matrix;
	//		cout << "\nOption 8 is running, please waiting...!\n";
	//		cout << "Close the preview image window to save the image.\n\n";

	//		//imgCTf->CalcHistogram(src, matrix);
	//		success = 0;
	//		success = imgCTf->DrawHistogramYCrCb(src, des);
	//		if (success) wannaSave(des);
	//	}
	//	else cout << "Opp! Wrong option.\n\n";
	//	//system("pause");
	//}


	//without menu here.
/*
	Mat des;
	Mat bgr;
	Mat histMatrix;

	//Lab 1 converter
	if (0) {
		Converter* img = new Converter();
		img->Convert(src, des, 0);
		//imshow("hi", src);
		waitKey(0);
	}

	//Lab 1 colorTransformer
	if (0)
	{
		ColorTransformer* img = new ColorTransformer();
		if (0) img->ChangeBrighness(src, des, -100);		//b in range [-255, 255]
		if (0) img->ChangeContrast(src, des, -1);	// c in range [-1,1]


		if (0)img->CalcHistogram(src, histMatrix);
		if (0)img->HistogramEqualization(src, des);
		if (0)img->DrawHistogram(histMatrix, des);
		if (0)img->DrawHistogramYCrCb(src, des);
		
		//imshow("hi", src);

	}
		waitKey(0);

*/
	return 0;
}
