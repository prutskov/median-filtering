#include "stdafx.h"
#include "FilterHost.h"
#include <ctime>

FilterHost::FilterHost(Log * log) : Filter(log)
{
}

FilterHost::~FilterHost()
{
}

float FilterHost::compute()
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
	return duration;
}


void FilterHost::quickSort(uchar* data, int size)
{
	std::sort(data, data + size);
}

void FilterHost::compute3x3()
{
	Frame result = _frame.clone();
	const int nRows = result.nRows;
	const int nCols = result.nCols;

#pragma omp parallel for
	for (int i = 1; i < nRows - 1; i++)
	{
		for (int j = 1; j < nCols - 1; j++)
		{
			median3x3(j, i, _frame, result, i*nCols + j);
		}
	}

	_frame = result;
}

void FilterHost::median3x3(int x, int y, const Frame& frame, Frame& result, int indexRes)
{
	const int maskSize = 9;
	/*Indexes from original frame for mask*/
	int indexes[maskSize] = { (y - 1)*frame.nCols + x - 1, (y - 1)*frame.nCols + x, (y - 1)*frame.nCols + x + 1,
					  y*frame.nCols + x - 1, y*frame.nCols + x, y*frame.nCols + x + 1,
					  (y + 1)*frame.nCols + x - 1, (y + 1)*frame.nCols + x, (y + 1)*frame.nCols + x + 1 };

	/*Get submatrix from filter-mask*/
	uchar matrixForSortingR[maskSize];
	uchar matrixForSortingG[maskSize];
	uchar matrixForSortingB[maskSize];

	for (int i = 0; i < maskSize; i++)
	{
		matrixForSortingR[i] = frame.dataRPtr[indexes[i]];
		matrixForSortingG[i] = frame.dataGPtr[indexes[i]];
		matrixForSortingB[i] = frame.dataBPtr[indexes[i]];
	}

	/*Sorting array*/
	quickSort(matrixForSortingR, maskSize);
	quickSort(matrixForSortingG, maskSize);
	quickSort(matrixForSortingB, maskSize);

	result.dataRPtr[indexRes] = matrixForSortingR[4];
	result.dataGPtr[indexRes] = matrixForSortingG[4];
	result.dataBPtr[indexRes] = matrixForSortingB[4];
}

void FilterHost::median5x5(int x, int y, const Frame& frame, Frame& result, int indexRes)
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
	uchar matrixForSortingR[maskSize];
	uchar matrixForSortingG[maskSize];
	uchar matrixForSortingB[maskSize];

	for (int i = 0; i < maskSize; i++)
	{
		matrixForSortingR[i] = frame.dataRPtr[indexes[i]];
		matrixForSortingG[i] = frame.dataGPtr[indexes[i]];
		matrixForSortingB[i] = frame.dataBPtr[indexes[i]];
	}

	/*Sorting array*/
	quickSort(matrixForSortingR, maskSize);
	quickSort(matrixForSortingG, maskSize);
	quickSort(matrixForSortingB, maskSize);

	result.dataRPtr[indexRes] = matrixForSortingR[12];
	result.dataGPtr[indexRes] = matrixForSortingG[12];
	result.dataBPtr[indexRes] = matrixForSortingB[12];
}

void FilterHost::compute5x5()
{
	Frame result = _frame.clone();
	const int nRows = result.nRows;
	const int nCols = result.nCols;

#pragma omp parallel for
	for (int i = 2; i < nRows - 2; i++)
	{
		for (int j = 2; j < nCols - 2; j++)
		{
			median5x5(j, i, _frame, result, i*nCols + j);
		}
	}

	_frame = result;
}
