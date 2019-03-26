#pragma once
#include "Filter.h"

class FilterHost :	public Filter
{
public:
	FilterHost();
	FilterHost(const Parameter &parameter, const Frame &frame);
	void compute() override;
	virtual ~FilterHost();
private:
	void quickSort(float* data, int size);
	float medianGet(int xCenterMask, int yCenterMask, const Frame& frame);
};

