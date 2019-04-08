#pragma once
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <memory>
#include "Log.h"

using namespace cv;

struct Frame
{
	Frame() : nRows(0), nCols(0)
	{
		dataRPtr = std::shared_ptr<uchar[]>(nullptr);
		dataGPtr = std::shared_ptr<uchar[]>(nullptr);
		dataBPtr = std::shared_ptr<uchar[]>(nullptr);
	}
	Frame(const int rows, const int cols, std::shared_ptr<uchar[]> dataR,
		std::shared_ptr<uchar[]> dataG, std::shared_ptr<uchar[]> dataB) : nRows(rows), nCols(cols)
	{
		dataRPtr = dataR;
		dataGPtr = dataG;
		dataBPtr = dataB;
	}

	/*Pointer to uchar Red data in [0..255]*/
	std::shared_ptr<uchar[]> dataRPtr;
	std::shared_ptr<uchar[]> dataGPtr;
	std::shared_ptr<uchar[]> dataBPtr;
	int nRows;
	int nCols;
};

class CVHelper
{
public:
	CVHelper(Log *log);
	virtual ~CVHelper();
	void loadImage(std::string path, int flag = IMREAD_GRAYSCALE);
	void imageShow(int mode);
	void imageShow(std::string name, Mat mat, int mode);
	void imageShow(std::string name, Frame image, int mode);
	bool isNullImage();
	Frame convertToPtr(Mat data);
	Mat convertToMat(Frame data);
	Frame getImage();

private:
	Mat _loadedImage;
	Frame _imagePtr;
	Log *_log;
};



