#pragma once
#include "Filter.h"

class FilterHost :	public Filter
{
public:
	FilterHost(Log *log);
	void compute() override;
	void setParameter(Parameter parameter);
	virtual ~FilterHost();
private:
	void quickSort(uchar* data, int size);
	void compute3x3();
	void median3x3(int x, int y, const Frame& frame, Frame& result, int indexRes);
	void compute5x5();
	Parameter _parameter;
};

