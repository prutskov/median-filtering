#include "stdafx.h"
#include "FilterHost.h"
#include <ctime>


FilterHost::FilterHost()
{
	this->_frame.nCols = 0;
	this->_frame.nRows = 0;
	this->_frame.dataPtr = 0;
	this->_parameter.filterSize = 3;
}

FilterHost::FilterHost(const Parameter &parameter, const Frame &frame)
{
	this->_frame.nCols = frame.nCols;
	this->_frame.nRows = frame.nRows;
	this->_frame.dataPtr = frame.dataPtr;
	this->_parameter = parameter;
}

FilterHost::~FilterHost()
{
}

Frame FilterHost::getFrame()
{
	return _frame;
}

void FilterHost::setFrame(const Frame &frame)
{
	_frame = frame;
}

void FilterHost::compute()
{
	const int nRows = _frame.nRows - 2;
	const int nCols = _frame.nCols - 2;
	Frame result(nRows, nCols,
		std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

#pragma omp parallel for
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			result.dataPtr[i*nCols + j] = medianGet(j + 1, i + 1, _frame);
		}
	}

	_frame = result;
}

void FilterHost::quickSort(float* data, int size)
{
	int i = 0;
	int j = size - 1;
	float p;

	p = data[size >> 1];

	do {
		while (data[i] < p) i++;
		while (data[j] > p) j--;

		if (i <= j) {
			swap(data[i], data[j]);
			i++; j--;
		}
	} while (i <= j);

	if (j > 0) quickSort(data, j);
	if (i < size) quickSort(data + i, size - i);
}

float FilterHost::medianGet(int x, int y, const Frame& frame)
{
	/*Indexes from original frame for mask*/
	int indexes[9] = { (y - 1)*frame.nCols + x - 1, (y - 1)*frame.nCols + x, (y - 1)*frame.nCols + x + 1,
					  y*frame.nCols + x - 1, y*frame.nCols + x, y*frame.nCols + x + 1,
					  (y + 1)*frame.nCols + x - 1, (y + 1)*frame.nCols + x, (y + 1)*frame.nCols + x + 1 };

	/*Get submatrix from filter-mask*/
	float matrixForSorting[9];
	for (int i = 0; i < 9; i++)
	{
		matrixForSorting[i] = frame.dataPtr[indexes[i]];
	}

	/*Sorting array*/
	quickSort(matrixForSorting, 9);

	/*Return median*/
	return matrixForSorting[4];
}

void FilterHost::generateNoise(float percent)
{
	const int nRows = _frame.nRows;
	const int nCols = _frame.nCols;

	const int nNoisedPixels = static_cast<int>(nRows*nCols*percent);
	int k = 0;
	while (k <= nNoisedPixels)
	{
		const int i = rand() % nRows;
		const int j = rand() % nCols;

		_frame.dataPtr[i*nCols + j] = rand() % 2;
		k++;
	}
}