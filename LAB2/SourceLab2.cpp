#include "GeometricTransformer.h"
#include "Lib.h"

PixelInterpolate::PixelInterpolate() {}
PixelInterpolate::~PixelInterpolate(){}

BilinearInterpolate::BilinearInterpolate() {}
BilinearInterpolate::~BilinearInterpolate(){}

NearestNeighborInterpolate::NearestNeighborInterpolate() {}
NearestNeighborInterpolate::~NearestNeighborInterpolate(){}

AffineTransform::AffineTransform()
{
	// tao ma tran don vi 3x3
	_matrixTransform = (Mat_<float>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
}
AffineTransform::~AffineTransform()
{
	_matrixTransform.release();
}

GeometricTransformer::GeometricTransformer() {}
GeometricTransformer::~GeometricTransformer(){}

/*
Lớp nội suy màu theo phương pháp song tuyến tính
*/
uchar BilinearInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels, uchar*pDes)
{
	if(!pSrc) return 0;
	int x = round(tx), y = round(ty);
	if (x < 0 || y < 0) return 0;
	float a = tx - x, b = ty - y;
	
	for (int i = 0; i < nChannels; i++)
	{
		int temp = 0;
		temp = (int)((1 - a) * (1 - b) * ((int)pSrc[x * srcWidthStep + y * nChannels + i]) + a * (1 - b) * ((int)pSrc[(x + 1) * srcWidthStep + y * nChannels + i]) 
			+ b * (1 - a) * ((int)pSrc[x * srcWidthStep + (y + 1) * nChannels + i]) + a * b * ((int)pSrc[(x + 1) * srcWidthStep + (y + 1) * nChannels + i]));

		if (temp > 255) temp = 255;
		if (temp < 0) temp = 0;

		pDes[i] = (uchar)temp;
	}
	return 1;
}

/*
Lớp nội suy màu theo phương pháp láng giềng gần nhất
*/
uchar NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels, uchar* pDes)
{
	if (!pSrc) return 0;
	int x = round(tx), y = round(ty);
	if (x < 0 || y < 0) return 0;
	for (int i = 0; i < nChannels; ++i) {
		uchar  temp = (uchar)(pSrc[x * srcWidthStep + y * nChannels + i]);
		pDes[i] = temp;
	}
	return 1;
}

/*
Lớp biểu diễn pháp biến đổi affine
*/
void AffineTransform::Translate(float dx, float dy)
{
	Mat matrix = (Mat_<float>(3, 3) << 1, 0, 0, 0, 1, 0, dx, dy, 1);
	_matrixTransform *= matrix;
}
void AffineTransform::Rotate(float angle)
{
	float sinx = sin(angle * PI / 180), cosx = cos(angle * PI / 180);
	Mat matrix = (Mat_<float>(3, 3) << cosx, sinx, 0, -sinx, cosx, 0, 0, 0, 1);
	_matrixTransform = _matrixTransform * matrix;
}
void AffineTransform::Scale(float sx, float sy)
{
	Mat matrix = (Mat_<float>(3, 3) << sx, 0, 0, 0, sy, 0, 0, 0, 1);
	_matrixTransform = _matrixTransform * matrix;
}
void AffineTransform::TransformPoint(float& x, float& y)
{
	Mat matrix = (Mat_<float>(1, 3) << x, y, 1);

	Mat result = matrix * _matrixTransform;
	x = result.at<float>(0, 0);
	y = result.at<float>(0, 1);
}
void AffineTransform::flipHorizontally()
{
	Mat matrix = (Mat_<float>(3, 3) << 1, 0, 0, 0, -1, 0, 0, 0, 1);
	_matrixTransform = _matrixTransform * matrix;
}
void AffineTransform::flipVertically()
{
	Mat matrix = (Mat_<float>(3, 3) << -1, 0, 0, 0, 1, 0, 0, 0, 1);
	_matrixTransform = _matrixTransform * matrix;
}
void AffineTransform::Inverse()
{
	_matrixTransform = _matrixTransform.inv();
}

/*
Lớp thực hiện phép biến đổi hình học trên ảnh
*/

int GeometricTransformer::Transform(const Mat& beforeImage, Mat& afterImage, AffineTransform* transformer, PixelInterpolate* interpolator)
{
	if (!beforeImage.data || !afterImage.data) return 0;

	int src_Width = beforeImage.step[0];
	int src_channel = beforeImage.step[1];
	int src_col = beforeImage.cols;
	int src_height = beforeImage.rows;

	int dst_Width = afterImage.step[0];
	int dst_height = afterImage.rows;
	int dst_col = afterImage.cols;

	uchar* srcData = (uchar*)beforeImage.data;
	uchar* dstData = (uchar*)afterImage.data;

	transformer->Inverse();

	for (int i = 0; i < dst_height; ++i, dstData += dst_Width) {
		uchar* pRow = dstData;
		for (int j = 0; j < dst_col; ++j, pRow += src_channel) {
			float y = i, x = j;
			transformer->TransformPoint(y, x);

			if (round(x) >= src_col || round(y) >= src_height) continue;
			if (x < 0 || y < 0) continue;
			interpolator->Interpolate(y, x, srcData, src_Width, src_channel, pRow);
		}
	}
	return 1;
}

int GeometricTransformer::RotateKeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	if (!srcImage.data) return 0;

	int src_width = srcImage.cols;
	int src_height = srcImage.rows;

	float cosx = cos(angle * PI / 180.0);
	float sinx = sin(angle * PI / 180.0);

	cosx = abs(cosx);
	sinx = abs(sinx);

	int dst_width = (int)(src_width * cosx + src_height * sinx); 
	int dst_height = (int)(src_width * sinx + src_height * cosx);
	dstImage = Mat(dst_height, dst_width, srcImage.type());

	AffineTransform* transform = new AffineTransform();
	transform->Translate((dst_height / 2) - (src_height / 2), (dst_width / 2) - (src_width / 2));
	transform->Translate(-(dst_height / 2), -(dst_width / 2));
	transform->Rotate(angle);
	transform->Translate(dst_height / 2, dst_width / 2);
	GeometricTransformer::Transform(srcImage, dstImage, transform, interpolator);

	namedWindow("Rotate keep image", WINDOW_AUTOSIZE);
	imshow("Rotate keep image", dstImage);
	waitKey(0);
	return 1;
}

int GeometricTransformer::RotateUnkeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	if (!srcImage.data) return 0;

	int src_width = srcImage.cols;
	int src_height = srcImage.rows;

	dstImage.create(src_height, src_width, srcImage.type());
	AffineTransform* transform = new AffineTransform();

	transform->Translate(-(src_height / 2), -(src_width / 2));
	transform->Rotate(angle);
	transform->Translate(src_height / 2, src_width / 2);
	GeometricTransformer::Transform(srcImage, dstImage, transform, interpolator);

	namedWindow("Rotate unkeep image", WINDOW_AUTOSIZE);
	imshow("Rotate unkeep image", dstImage);
	waitKey(0);
	return 1;
}

int GeometricTransformer::Scale(const Mat& srcImage, Mat& dstImage, float sx, float sy, PixelInterpolate* interpolator)
{
	if (!srcImage.data) return 0;

	float width = srcImage.cols;
	float height = srcImage.rows;

	dstImage.create(width, height, srcImage.type());
	AffineTransform* transform = new AffineTransform();

	transform->Translate(-height / 2, -width / 2);
	transform->Scale(sx, sy);
	transform->Translate(height / 2, width / 2);
	GeometricTransformer::Transform(srcImage, dstImage, transform, interpolator);

	namedWindow("Scale", WINDOW_AUTOSIZE);
	imshow("Scale", dstImage);
	waitKey(0);
	return 1;
}

int GeometricTransformer::Resize(const Mat& srcImage, Mat& dstImage, int newWidth, int newHeight, PixelInterpolate* interpolator)
{
	if (!srcImage.data) return 0;

	int srcH = srcImage.cols, srcW = srcImage.rows;

	float heightScale = newWidth*1.0 / srcH;
	float widthScale = newHeight*1.0 / srcW;

	int width = srcH * widthScale;
	int height = srcW * heightScale;

	dstImage.create(width, height, srcImage.type());
	AffineTransform* transform = new AffineTransform();

	transform->Translate(-height / 2, -width / 2);
	transform->Scale(widthScale, heightScale);
	transform->Translate(widthScale * height / 2, heightScale * width / 2);

	GeometricTransformer::Transform(srcImage, dstImage, transform, interpolator);

	namedWindow("Resize", WINDOW_AUTOSIZE);
	imshow("Resize", dstImage);
	waitKey(0);
	return 1;
}

int GeometricTransformer::Flip(const Mat& srcImage, Mat& dstImage, bool direction, PixelInterpolate* interpolator)
{
	if (!srcImage.data) return 0;

	int width = srcImage.cols;
	int height = srcImage.rows;

	dstImage.create(width, height, srcImage.type());
	AffineTransform* transform = new AffineTransform();

	transform->Translate(-height / 2, -width / 2);
	if (!direction)transform->flipVertically();
	else transform->flipHorizontally();
	transform->Translate(height / 2, width / 2);

	GeometricTransformer::Transform(srcImage, dstImage, transform, interpolator);

	namedWindow("Flip", WINDOW_AUTOSIZE);
	imshow("Flip", dstImage);
	waitKey(0);
	return 1;
}
