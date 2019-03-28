inline void bubbleSort(float* data, int size)
{
	float temp; 
   // Сортировка массива пузырьком
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (data[j] > data[j + 1]) {
				// меняем элементы местами
				temp = data[j];
				data[j] = data[j + 1];
				data[j + 1] = temp;
			}
		}
	}
}

inline float getMedian(const int x, const int y, const int nRows, const int nCols, const __global float* data)
{
	/*Indexes from original frame for mask*/
	int indexes[9] = { (y - 1)*nCols + x - 1, (y - 1)*nCols + x, (y - 1)*nCols + x + 1,
					  y*nCols + x - 1, y*nCols + x, y*nCols + x + 1,
					  (y + 1)*nCols + x - 1, (y + 1)*nCols + x, (y + 1)*nCols + x + 1 };

	/*Get submatrix from filter-mask*/
	float matrixForSorting[9];
	for (int i = 0; i < 9; i++)
	{
		matrixForSorting[i] = data[indexes[i]];
	}

	/*Sorting array*/
	bubbleSort(matrixForSorting, 9);

	/*Return median*/
	return matrixForSorting[4];
}

__kernel void nativeFilter3x3(const int nRows, const int nCols,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	imageOut[rowIdx*nCols + colIdx] =  getMedian(colIdx + 1, rowIdx + 1, nRows+2, nCols+2, imageIn);
}