inline void sortPair(uchar *a, uchar *b)
{
	uchar tmp = *a;
	*a = min(tmp, *b);
	*b = max(tmp, *b);
}

inline void bitonicSort(uchar* a, int size)
{

	sortPair(a + 1, a + 2); sortPair(a + 4, a + 5); sortPair(a + 7, a + 8);
	sortPair(a + 0, a + 1); sortPair(a + 3, a + 4); sortPair(a + 6, a + 7);
	sortPair(a + 1, a + 2); sortPair(a + 4, a + 5); sortPair(a + 7, a + 8);
	sortPair(a + 0, a + 3); sortPair(a + 5, a + 8); sortPair(a + 4, a + 7);
	sortPair(a + 3, a + 6); sortPair(a + 1, a + 4); sortPair(a + 2, a + 5);
	sortPair(a + 4, a + 7); sortPair(a + 4, a + 2); sortPair(a + 6, a + 4);
	sortPair(a + 4, a + 2);
}


inline void getMedian(const int x, const int y, const int nRows, const int nCols, const int indexRes,
	__global const uchar* imageRIn, __global const uchar* imageGIn, __global const uchar* imageBIn,
	__global uchar* imageROut, __global uchar* imageGOut, __global uchar* imageBOut)
{
	/*Indexes from original frame for mask*/
	int indexes[9] = { (y - 1)*nCols + x - 1,
					   (y - 1)*nCols + x,
					   (y - 1)*nCols + x + 1,
							 y*nCols + x - 1,
							 y*nCols + x,
							 y*nCols + x + 1,
					   (y + 1)*nCols + x - 1,
					   (y + 1)*nCols + x,
					   (y + 1)*nCols + x + 1 };

	/*Get submatrix from filter-mask*/
	uchar matrixForSortingR[9];
	uchar matrixForSortingG[9];
	uchar matrixForSortingB[9];
	for (int i = 0; i < 9; i++)
	{
		matrixForSortingR[i] = imageRIn[indexes[i]];
		matrixForSortingG[i] = imageGIn[indexes[i]];
		matrixForSortingB[i] = imageBIn[indexes[i]];
	}

	/*Sorting array*/
	bitonicSort(matrixForSortingR, 9);
	bitonicSort(matrixForSortingG, 9);
	bitonicSort(matrixForSortingB, 9);

	imageROut[indexRes] = matrixForSortingR[4];
	imageGOut[indexRes] = matrixForSortingG[4];
	imageBOut[indexRes] = matrixForSortingB[4];
}

__kernel void nativeFilter3x3(const int nRows, const int nCols,
	__global const uchar* imageRIn, __global const uchar* imageGIn, __global const uchar* imageBIn,
	__global uchar* imageROut, __global uchar* imageGOut, __global uchar* imageBOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	getMedian(colIdx + 1, rowIdx + 1, nRows + 2, nCols + 2, rowIdx*nCols + colIdx, imageRIn, imageGIn, imageBIn,
		imageROut, imageGOut, imageBOut);
}