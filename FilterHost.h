#pragma once
#include "Filter.h"

class FilterHost :	public Filter
{
public:
	FilterHost(const Parameter &parameter, const Frame &frame, Log *log);
	void compute() override;
	virtual ~FilterHost();
private:
	void quickSort(float* data, int size);
	void compute3x3();
	void compute5x5();
};

