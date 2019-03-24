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

void CVHelper::imageShow()
{
	namedWindow("Image", WINDOW_AUTOSIZE);
	moveWindow("Image", 0, 0);
	imshow("Image", _loadedImage);
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
