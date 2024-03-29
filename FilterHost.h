#pragma once
#include "Filter.h"

class FilterHost :	public Filter
{
public:
	FilterHost(Log *log);
	void compute(bool isLogging) override;
	void setParameter(Parameter parameter);
	virtual ~FilterHost();
private:
	void quickSort(uchar* data, int size);
	void median3x3(int x, int y, const Frame& frame, Frame& result, int indexRes);
	void median5x5(int x, int y, const Frame& frame, Frame& result, int indexRes);
	void compute3x3();
	void compute5x5();
	Parameter _parameter;
};

