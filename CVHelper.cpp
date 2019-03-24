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
	auto frame = convertToPtr(_loadedImage);
}

void CVHelper::imageShow()
{
	imshow("Original image", _loadedImage);
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
			frame.dataPtr[i*nCols + j] = floatData.ptr<float>(i)[j];
		}
	}

	return frame;
}
