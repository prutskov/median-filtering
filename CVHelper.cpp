#include "stdafx.h"
#include "CVHelper.h"

CVHelper::CVHelper()
{
}


CVHelper::~CVHelper()
{
}

void CVHelper::loadImage(std::string path, int flag)
{
	_loadedImage = imread(path, flag);
	_imagePtr = convertToPtr(_loadedImage);
}

void CVHelper::imageShow(int mode)
{
	namedWindow("Image", mode);
	moveWindow("Image", 0, 0);
	imshow("Image", _loadedImage);
}

void CVHelper::imageShow(std::string name, Frame image, int mode)
{
	namedWindow(name, mode);
	moveWindow(name, 0, 0);
	imshow(name, convertToMat(image));
}

Frame CVHelper::convertToPtr(Mat data)
{
	const int nRows = data.rows;
	const int nCols = data.cols;

	Frame frame(nRows, nCols,
		std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

	Mat_<float> floatData;
	data.convertTo(floatData, CV_32F);

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			frame.dataPtr[i*nCols + j] = floatData.ptr<float>(i)[j] / 255.0F;
		}
	}

	return frame;
}

Mat CVHelper::convertToMat(Frame data)
{
	const int nRows = data.nRows;
	const int nCols = data.nCols;

	cv::Mat_<float> mat(nRows, nCols, data.dataPtr.get());
	
	return mat.clone();
}

Frame CVHelper::getImage()
{
	return this->_imagePtr;
}

void CVHelper::setImage(Frame frame)
{
	
}
