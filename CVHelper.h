#pragma once
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
class CVHelper
{
public:
	CVHelper();
	virtual ~CVHelper();
	void loadImage(std::string path, int flag = IMREAD_COLOR);
	void imageShow();

private:
	cv::Mat _loadedImage;
};

