#include "stdafx.h"
#include "CVHelper.h"

CVHelper::CVHelper(Log *log)
{
	this->_log = log;
}

CVHelper::~CVHelper()
{
}

void CVHelper::loadImage(std::string path, int flag)
{
	_loadedImage = imread(path, flag);
	_log->add("Image loaded from: " + path);
}

void CVHelper::imageShow(int mode)
{
	std::string nameWindow = "Original image. Size: " + std::to_string(_loadedImage.rows) +
		"x" + std::to_string(_loadedImage.cols);
	namedWindow(nameWindow, mode);
	moveWindow(nameWindow, 0, 0);
	imshow(nameWindow, _loadedImage);
	_log->add("Show original image. Size: " + std::to_string(_loadedImage.rows)
		+ "x" + std::to_string(_loadedImage.cols));
}

void CVHelper::imageShow(std::string name, Mat mat, int mode)
{
	std::string nameWindow = name + " Size: " + std::to_string(mat.rows) +
		"x" + std::to_string(mat.cols);
	namedWindow(nameWindow, mode);
	moveWindow(nameWindow, 0, 0);
	imshow(nameWindow, mat);
	_log->add("Show " + nameWindow);
}

void CVHelper::imageShow(std::string name, Frame image, int mode)
{
	std::string nameWindow = name + " Size: " + std::to_string(image.nRows) +
		"x" + std::to_string(image.nCols);
	namedWindow(nameWindow, mode);
	moveWindow(nameWindow, 0, 0);
	imshow(nameWindow, convertToMat(image));
	_log->add("Show " + nameWindow);
}

bool CVHelper::isNullImage()
{
	return _loadedImage.empty();
}

Frame CVHelper::convertToPtr(Mat data)
{
	const int nRows = data.rows;
	const int nCols = data.cols;

	Frame frame(nRows, nCols,
		std::shared_ptr<uchar[]>(new uchar[nRows*nCols], std::default_delete<uchar[]>()),
		std::shared_ptr<uchar[]>(new uchar[nRows*nCols], std::default_delete<uchar[]>()),
		std::shared_ptr<uchar[]>(new uchar[nRows*nCols], std::default_delete<uchar[]>()));
	
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			frame.dataBPtr[i*nCols + j] = data.at<Vec3b>(i, j)[0];
			frame.dataGPtr[i*nCols + j] = data.at<Vec3b>(i, j)[1];
			frame.dataRPtr[i*nCols + j] = data.at<Vec3b>(i, j)[2];
		}
	}

	return frame;
}

Mat CVHelper::convertToMat(Frame data)
{
	const int nRows = data.nRows;
	const int nCols = data.nCols;

	Mat mat(nRows, nCols, CV_8UC3);

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j ++)
		{
			mat.at<Vec3b>(i, j)[0] = data.dataBPtr[i*nCols + j];
			mat.at<Vec3b>(i, j)[1] = data.dataGPtr[i*nCols + j];
			mat.at<Vec3b>(i, j)[2] = data.dataRPtr[i*nCols + j];
		}
	}

	return mat.clone();
}

Frame CVHelper::getImage()
{
	_imagePtr = convertToPtr(_loadedImage);
	return _imagePtr;
}
