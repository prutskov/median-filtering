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
}

void CVHelper::imageShow()
{
	imshow("Original image", _loadedImage);
}
