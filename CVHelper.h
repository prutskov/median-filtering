#pragma once
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <memory>

using namespace cv;

struct Frame
{
	Frame() : nRows(0), nCols(0)
	{
		dataPtr = std::shared_ptr<float[]>(nullptr);
	}
	Frame(const int rows, const int cols, std::shared_ptr<float[]> data) : nRows(rows), nCols(cols)
	{
		dataPtr = data;
	}

	/*Pointer to float data in [0..1]*/
	std::shared_ptr<float[]> dataPtr;
	int nRows;
	int nCols;
};

class CVHelper
{
public:
	CVHelper();
	virtual ~CVHelper();
	void loadImage(std::string path, int flag = IMREAD_GRAYSCALE);
	void imageShow();

	Frame convertToPtr(Mat data);
	Mat convertToMat(Frame data);

private:
	Mat _loadedImage;
	Frame _imagePtr;
};



