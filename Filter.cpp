#include "stdafx.h"
#include "Filter.h"


Filter::Filter(Log * log) : _log(log)
{
}

Frame Filter::getFrame()
{
	return _frame;
}

void Filter::setFrame(const Frame & frame)
{
	_frame = frame;
}

void Filter::generateNoise(float percent)
{
	const int nRows = _frame.nRows;
	const int nCols = _frame.nCols;

	const int nNoisedPixels = static_cast<int>(nRows*nCols*percent);
	int k = 0;
	while (k <= nNoisedPixels)
	{
		const int i = rand() % nRows;
		const int j = rand() % nCols;

		_frame.dataRPtr[i*nCols + j] = static_cast<uchar>(rand() % 2 * 255);
		_frame.dataBPtr[i*nCols + j] = static_cast<uchar>(rand() % 2 * 255);
		_frame.dataGPtr[i*nCols + j] = static_cast<uchar>(rand() % 2 * 255);
		k++;
	}
}

std::vector<std::string> Filter::getDevices()
{
	std::vector<std::string> devices = { "degault" };
	return devices;
}
