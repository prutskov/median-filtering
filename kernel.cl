inline void sortPair(float *a, float *b)
{
	float tmp = *a;
	*a = fmin(tmp, *b);
	*b = fmax(tmp, *b);
}

inline void bitonicSort(float* a, int size)
{

	sortPair(a + 1, a + 2); sortPair(a + 4, a + 5); sortPair(a + 7, a + 8);
	sortPair(a + 0, a + 1); sortPair(a + 3, a + 4); sortPair(a + 6, a + 7);
	sortPair(a + 1, a + 2); sortPair(a + 4, a + 5); sortPair(a + 7, a + 8);
	sortPair(a + 0, a + 3); sortPair(a + 5, a + 8); sortPair(a + 4, a + 7);
	sortPair(a + 3, a + 6); sortPair(a + 1, a + 4); sortPair(a + 2, a + 5);
	sortPair(a + 4, a + 7); sortPair(a + 4, a + 2); sortPair(a + 6, a + 4);
	sortPair(a + 4, a + 2);
}


inline float getMedian(const int x, const int y, const int nRows, const int nCols, const __global float* data)
{
	float temp[9] = {};
	for (int row = 0; row < 3; ++row)
		for (int col = 0; col < 3; ++col)
			temp[row*3 + col] = data[(y - 1 + row)*nCols + x - 1 + col];

	/*Sorting array*/
	bitonicSort(temp, 9);

	/*Return median*/
	return temp[4];
}

__kernel void nativeFilter3x3(const int nRows, const int nCols,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	imageOut[rowIdx*nCols + colIdx] = getMedian(colIdx+1, rowIdx+1, nRows + 2, nCols + 2, imageIn);
}