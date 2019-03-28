#include "stdafx.h"
#include "FilterHost.h"
#include <ctime>

FilterHost::FilterHost(const Parameter &parameter, const Frame &frame, Log *log)
	: Filter(parameter, frame, log)
{}

FilterHost::~FilterHost()
{
}

void FilterHost::compute()
{
	auto start = std::chrono::high_resolution_clock::now();
	if (_parameter.mask == Mask::MASK3X3)
	{
		compute3x3();
	}
	else
	{
		compute5x5();
	}
	auto end = std::chrono::high_resolution_clock::now();
	float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0F;
	_log->add("Filter: Host algorithm. Timing: " + std::to_string(duration) + "ms");
}

void FilterHost::quickSort(float* data, int size)
{
	std::sort(data, data + size);
}

void FilterHost::compute3x3()
{
	const int nRows = _frame.nRows - 2;
	const int nCols = _frame.nCols - 2;
	Frame result(nRows, nCols,
		std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

	/*Get median*/
	auto median = [&](int x, int y, const Frame& frame)->float
	{
		const int maskSize = 9;
		/*Indexes from original frame for mask*/
		int indexes[maskSize] = { (y - 1)*frame.nCols + x - 1, (y - 1)*frame.nCols + x, (y - 1)*frame.nCols + x + 1,
						  y*frame.nCols + x - 1, y*frame.nCols + x, y*frame.nCols + x + 1,
						  (y + 1)*frame.nCols + x - 1, (y + 1)*frame.nCols + x, (y + 1)*frame.nCols + x + 1 };

		/*Get submatrix from filter-mask*/
		float matrixForSorting[maskSize];
		for (int i = 0; i < maskSize; i++)
		{
			matrixForSorting[i] = frame.dataPtr[indexes[i]];
		}

		/*Sorting array*/
		quickSort(matrixForSorting, maskSize);

		/*Return median*/
		return matrixForSorting[maskSize/2];
	};

#pragma omp parallel for
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			result.dataPtr[i*nCols + j] = median(j + 1, i + 1, _frame);
		}
	}

	_frame = result;
}

void FilterHost::compute5x5()
{
	const int nRows = _frame.nRows - 4;
	const int nCols = _frame.nCols - 4;
	Frame result(nRows, nCols,
		std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

	/*Get median*/
	auto median = [&](int x, int y, const Frame& frame)->float
	{
		const int maskSize = 25;
		/*Indexes from original frame for mask*/
		int indexes[maskSize] = { (y - 2)*frame.nCols + x - 2,
							(y - 2)*frame.nCols + x - 1,
							(y - 2)*frame.nCols + x,
							(y - 2)*frame.nCols + x + 1,
							(y - 2)*frame.nCols + x + 2,
							(y - 1)*frame.nCols + x - 2,
							(y - 1)*frame.nCols + x - 1,
							(y - 1)*frame.nCols + x,
							(y - 1)*frame.nCols + x + 1,
							(y - 1)*frame.nCols + x + 2,
								  y*frame.nCols + x - 2, 
								  y*frame.nCols + x - 1, 
								  y*frame.nCols + x,
								  y*frame.nCols + x + 1,
								  y*frame.nCols + x + 2,
							(y + 1)*frame.nCols + x - 2,
							(y + 1)*frame.nCols + x - 1,
							(y + 1)*frame.nCols + x,
							(y + 1)*frame.nCols + x + 1,
							(y + 1)*frame.nCols + x + 2,
							(y + 2)*frame.nCols + x - 2,
							(y + 2)*frame.nCols + x - 1,
							(y + 2)*frame.nCols + x,
							(y + 2)*frame.nCols + x + 1,
							(y + 2)*frame.nCols + x + 2 };

		/*Get submatrix from filter-mask*/
		float matrixForSorting[maskSize];
		for (int i = 0; i < maskSize; i++)
		{
			matrixForSorting[i] = frame.dataPtr[indexes[i]];
		}

		/*Sorting array*/
		quickSort(matrixForSorting, maskSize);

		/*Return median*/
		return matrixForSorting[maskSize/2];
	};

#pragma omp parallel for
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			result.dataPtr[i*nCols + j] = median(j + 2, i + 2, _frame);
		}
	}

	_frame = result;
}
