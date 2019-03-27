#include "stdafx.h"
#include "Filter.h"

Filter::Filter(const Parameter & parameter, const Frame & frame, Log* log) 
	: _parameter(parameter), _frame(frame), _log(log)
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

		_frame.dataPtr[i*nCols + j] = static_cast<float>(rand() % 2);
		k++;
	}
}
