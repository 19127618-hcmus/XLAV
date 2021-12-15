#pragma once
#include "Lib.h"

/*------------------------------------------------------------------------------------------------------
 *                                                                                                     *
 *										GHI CHÚ!!!                                                     *
 * Trong lab này em chọn phương pháp tách class của các phương pháp (trung bình, Gaussian, trung vị)   *
 * ra thành các class con. Những class trên được kế thừa từ class Convolution.                         *
 * Lý do tách: xử lý hàm SetKernel cho từng phương pháp, xử lý riêng hàm DoConvolution của phương      *
 * pháp trung vị.																					   *
 * Thay đổi class Convolution so với bản gốc:														   *
 *		1. Các biến _kernel, _kernelWidth, _kernelHeight từ private -> public.						   *
 *		2. Hàm SetKernel trở thành hàm ảo -> tuỳ biến trong các class con.							   *
 *																									   *
 ------------------------------------------------------------------------------------------------------*/

class Convolution
{
public:
	vector<float> _kernel;// lưu giá trị của ma trận kernel dùng để tích chập
	int _kernelWidth;//chiều rộng kernel
	int _kernelHeight;//chiều dài kernel

	//trả về kernel 
	vector<float> GetKernel();
	//set kernel, chiều rộng, chiều dài kernel phải là số lẻ
	virtual void SetKernel(vector<float> kernel, int kWidth, int kHeight) = 0;

	/*
	Hàm tính convolution của 1 ảnh xám với kernel được xác định trước
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu tính thành công
	1: nếu tính thất bại (không đọc được ảnh input,...)
	*/
	int DoConvolution(const Mat& sourceImage, Mat& destinationImage);
	Convolution();
	~Convolution();
};

/*-------------------------------------------------------------------------------------*/

class AverageKernel : public Convolution
{
public:
	void SetKernel(vector<float> kernel, int kWidth, int kHeight) override;
	AverageKernel();
	~AverageKernel();
};

/*-------------------------------------------------------------------------------------*/

class GaussianKernel:public Convolution
{
public:
	void SetKernel(vector<float> kernel, int kWidth, int kHeight) override;
	GaussianKernel();
	~GaussianKernel();
};

/*-------------------------------------------------------------------------------------*/

class MedianKernel: public Convolution
{
public:
	void SetKernel(vector<float> kernel, int kWidth, int kHeight) override;
	int DoConvolution(const Mat& sourceImage, Mat& destinationImage);
	MedianKernel();
	~MedianKernel();
};