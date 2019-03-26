#include "stdafx.h"
#include "FilterHost.h"
#include <ctime>


FilterHost::FilterHost() : Filter() {}

FilterHost::FilterHost(const Parameter &parameter, const Frame &frame)
	: Filter(parameter, frame)
{}

FilterHost::~FilterHost()
{
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