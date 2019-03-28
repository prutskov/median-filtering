__kernel void nativeFilter3x3(const int nRows, const int nCols,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	imageOut[rowIdx*nCols + colIdx] = 1;
}